# TinyQuest — Play Mode System

Documentation for the core gameplay classes that drive third-person player control in TinyQuest: `ATQPlayerCharacter`, `ATQPlayerController`, and `ATQPlayGameMode`.

## Overview

Three classes work together to get a controllable character into the world:

| Class | Role |
|---|---|
| `ATQPlayGameMode` | Tells Unreal which controller class to spawn for players. |
| `ATQPlayerController` | Owns Enhanced Input setup, binds input actions, and forwards input events to the possessed pawn. |
| `ATQPlayerCharacter` | The pawn itself — owns the spring-arm/camera hierarchy, converts raw input vectors into movement, camera look, and jumping. |

```
GameMode
   └─ spawns → PlayerController
                   ├─ adds Input Mapping Context (on possess)
                   ├─ binds Move / Look / Jump actions (on input setup)
                   └─ forwards triggered input → PlayerCharacter
                                                     ├─ MoveHandler()
                                                     ├─ LookHandler()
                                                     ├─ JumpHandler()
                                                     └─ JumpingStopHandler()
```

## `ATQPlayGameMode`

Minimal game mode whose only job is to wire in the custom controller.

- **Constructor**: sets `PlayerControllerClass = ATQPlayerController::StaticClass()`.
- **`BeginPlay()`**: calls `Super::BeginPlay()`; no additional logic yet.

## `ATQPlayerController`

Handles Enhanced Input setup and routes input events to the possessed character.

### Configurable properties (set in Blueprint/editor)

| Property | Type | Purpose |
|---|---|---|
| `Player3rdPersonIMC` | `UInputMappingContext*` | Input Mapping Context applied to the local player. |
| `MoveAction` | `UInputAction*` | Action asset for movement (2D axis). |
| `LookAction` | `UInputAction*` | Action asset for camera look (2D axis). |
| `JumpAction` | `UInputAction*` | Action asset for jump (digital/bool). |
| `MappingPriority` | `int32` | Priority used when adding the mapping context (default `0`). |

### Lifecycle

- **`BeginPlay()`** — sets input mode to `FInputModeGameOnly`, hides the mouse cursor, logs the currently possessed pawn name.
- **`OnPossess(APawn* InPawn)`** — once a pawn is possessed, fetches the `UEnhancedInputLocalPlayerSubsystem` from the local player and adds `Player3rdPersonIMC` at `MappingPriority`.
- **`SetupInputComponent()`** — casts `InputComponent` to `UEnhancedInputComponent` and binds:
  - `MoveAction` → `Triggered` → `HandleMovement`
  - `LookAction` → `Triggered` → `HandleLook`
  - `JumpAction` → `Triggered` → `HandleJumpStart`
  - `JumpAction` → `Completed` → `HandleJumpEnd`

### Input handlers

Each handler casts `GetPawn()` to `ATQPlayerCharacter` and forwards the value:

```cpp
void ATQPlayerController::HandleMovement(const FInputActionValue& Value)
{
    const FVector2D MovementVector = Value.Get<FVector2D>();
    if (TObjectPtr<ATQPlayerCharacter> TargetCharacter = Cast<ATQPlayerCharacter>(GetPawn()))
    {
        TargetCharacter->MoveHandler(MovementVector);
    }
}
```

`HandleLook` mirrors this for `LookAction`. `HandleJumpStart` / `HandleJumpEnd` call `JumpHandler()` / `JumpingStopHandler()` respectively, with no payload needed since jump is a digital action.

### Enhanced Input data assets

Four data assets back the system, matching the properties on `ATQPlayerController`:

| Asset | Type |
|---|---|
| `Player_3rd_Person_IMC` | Input Mapping Context |
| `Move_IA` | Input Action |
| `Look_IA` | Input Action |
| `Jump_IA` | Input Action |

### `Player_3rd_Person_IMC` mappings

**`Move_IA`** — keyboard, WASD:

| Key | Modifiers |
|---|---|
| `W` | Swizzle Input Axis Values (Order: YXZ) |
| `S` | Swizzle Input Axis Values (Order: YXZ) → Negate (X, Y, Z) |
| `D` | — none — |
| `A` | Negate (X, Y, Z) |

The swizzle on `W`/`S` remaps the key's raw axis output from X onto Y, so `W`/`S` drive the Y component and `A`/`D` drive the X component of the resulting `FVector2D` — consistent with `MoveHandler()` reading `MovementVector.Y` as forward/back and `MovementVector.X` as strafe. `S` and `A` then negate their axis to produce backward/left movement.

**`Look_IA`** — `Mouse XY 2D-Axis`, with 2 modifiers (not expanded in the captured screenshot — worth confirming whether one of these is a sensitivity scalar, since `LookHandler()` applies the raw value with no scaling of its own).

**`Jump_IA`** — `Space Bar`, no triggers or modifiers (digital on/off, matching the `Triggered`/`Completed` binding in `SetupInputComponent()`).

## `ATQPlayerCharacter`

The pawn that actually moves, looks, and jumps. It owns the spring-arm/camera hierarchy in C++.

### Components

| Property | Type | Category |
|---|---|---|
| `CameraBoom` | `USpringArmComponent*` | `TQ\|Camera` |
| `FollowCamera` | `UCameraComponent*` | `TQ\|Camera` |

Both are `VisibleAnywhere, BlueprintReadOnly` — visible in the editor hierarchy but not reassignable from Blueprint.

### Construction

- `PrimaryActorTick.bCanEverTick = false` — tick is disabled by default since none of the current logic needs per-frame work.
- `bUseControllerRotationPitch/Yaw/Roll = false` — the character does not snap its mesh rotation to the controller; instead:
  - `CharacterMovementComponent->bOrientRotationToMovement = true` — the character rotates to face its movement direction.
  - `RotationRate = FRotator(0, 500, 0)` — controls how fast that turn happens (yaw only).
- **`CameraBoom`** — created as a default subobject and attached to `RootComponent`:
  - `TargetArmLength = 350.0f` — keeps the camera roughly 3.5 m behind the character.
  - `bUsePawnControlRotation = true` — the arm follows the controller's view rotation (mouse look pivots the camera around the character).
  - `bEnableCameraLag = true` / `CameraLagSpeed = 10.0f` — adds a small positional lag so the camera trails the character smoothly rather than snapping.
  - `bDoCollisionTest = true` — the arm shortens automatically when geometry sits between the character and the camera.
- **`FollowCamera`** — created as a default subobject and attached to `CameraBoom` at `USpringArmComponent::SocketName` (the arm's tip socket):
  - `bUsePawnControlRotation = false` — the camera inherits rotation from the boom; it does not additionally rotate with the controller. This is the standard third-person setup: the boom rotates, the camera stays fixed on the end of the boom.

### `BeginPlay()` / `Tick()`

Standard overrides; `BeginPlay` logs entry, `Tick` just calls `Super::Tick()`.

### Movement & look

```cpp
void ATQPlayerCharacter::MoveHandler(const FVector2D& MovementVector)
```
Builds forward/right basis vectors from the **controller's current yaw** (camera-relative movement, not character-relative), then calls `AddMovementInput` twice — once for forward/back (`MovementVector.Y`) and once for strafe (`MovementVector.X`).

```cpp
void ATQPlayerCharacter::LookHandler(const FVector2D& LookVector)
```
Directly applies `AddControllerYawInput` / `AddControllerPitchInput` from the look vector — no sensitivity scaling is applied at this layer (expected to be configured on the Input Action / IMC modifiers instead). Because `CameraBoom->bUsePawnControlRotation = true`, rotating the controller yaw/pitch here also pivots the camera arm.

### Jumping

`JumpHandler()` / `JumpingStopHandler()` simply call the built-in `ACharacter::Jump()` / `StopJumping()`.

## Notes & things to double check

- **Tick is disabled** on the character (`bCanEverTick = false`) but `Tick()` is still overridden and bound — harmless, but it's dead code unless re-enabled later.
- **No null checks on `MoveAction`/`LookAction` before binding individual ones** — `MoveAction` and `JumpAction` are guarded with `if`, `LookAction`'s bind isn't wrapped in braces (works fine due to single-statement `if`, but is inconsistent style with the others).
- **Look sensitivity / mouse vs. gamepad scaling** isn't handled in code — confirm this is intentional and configured via Input Modifiers on the `LookAction` asset.
- **Camera pitch clamping** — `AddControllerPitchInput` is unbounded here; if the arm can clip through the ground on a steep downward look, consider setting `CameraBoom->ProbeSize` or clamping pitch via `PlayerCameraManager->ViewPitchMin/Max` (configurable in the PlayerController or Camera Manager class defaults).
- **`A` key's Negate modifier includes Z**, but `Move_IA` is only ever consumed as an `FVector2D` — harmless, just inconsistent with `S`'s modifier stack which negates explicitly after a swizzle.

## Appendix: Full Source

***TQPlayerCharacter.h***

```cpp
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TQPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class TINYQUEST_API ATQPlayerCharacter : public ACharacter
{
    GENERATED_BODY()

protected:
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="TQ|Camera")
    TObjectPtr<USpringArmComponent> CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="TQ|Camera")
    TObjectPtr<UCameraComponent> FollowCamera;
    
public:
    // Sets default values for this character's properties
    ATQPlayerCharacter();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    
    void MoveHandler(const FVector2D& MovementVector);
    void LookHandler(const FVector2D& LookVector);
    void JumpHandler();
    void JumpingStopHandler();
    
};
```

***TQPlayerCharacter.cpp***

```cpp
// Fill out your copyright notice in the Description page of Project Settings.


#include "TQPlayerCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ATQPlayerCharacter::ATQPlayerCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    if (const TObjectPtr<UCharacterMovementComponent> MoveComp = GetCharacterMovement())
    {
        MoveComp->bOrientRotationToMovement = true; // Character automatically faces moving vector
        MoveComp->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // Smooth turning rate
    }
    
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);

    CameraBoom->TargetArmLength = 350.0f;
    CameraBoom->bUsePawnControlRotation = true;
    CameraBoom->bEnableCameraLag = true;
    CameraBoom->CameraLagSpeed = 10.f;
    CameraBoom->bDoCollisionTest = true;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

}

// Called when the game starts or when spawned
void ATQPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogTemp, Warning, TEXT("ATQPlayerCharacter::BeginPlay()"));
}

// Called every frame
void ATQPlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ATQPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ATQPlayerCharacter::MoveHandler(const FVector2D& MovementVector)
{
    if (Controller != nullptr)
    {
        // Find which way is forward relative to camera viewing angles
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        // Inject directional physical velocity vectors straight into character movement component
        AddMovementInput(ForwardDirection, MovementVector.Y);
        AddMovementInput(RightDirection, MovementVector.X);
    }
}

void ATQPlayerCharacter::LookHandler(const FVector2D& LookVector)
{
    AddControllerYawInput(LookVector.X);
    AddControllerPitchInput(LookVector.Y);
}

void ATQPlayerCharacter::JumpHandler()
{
    Jump();
}

void ATQPlayerCharacter::JumpingStopHandler()
{
    StopJumping();
}
```

***TQPlayerController.h***

```cpp
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TQPlayerController.generated.h"

class UInputMappingContext;
struct FInputActionValue;
class UInputAction;
/**
 * 
 */
UCLASS()
class TINYQUEST_API ATQPlayerController : public APlayerController
{
    GENERATED_BODY()

protected:
    /** Input asset configurations mapped with modern UE5 TObjectPtr */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TQ|Input", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputMappingContext> Player3rdPersonIMC;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TQ|Input", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> MoveAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TQ|Input", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> LookAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TQ|Input", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> JumpAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TQ|Input", meta = (AllowPrivateAccess = "true"))
    int32 MappingPriority = 0;

public:
    ATQPlayerController();

protected:
    virtual void BeginPlay() override;
    virtual void OnPossess(APawn* InPawn) override;

    virtual void SetupInputComponent() override;

    UFUNCTION()
    void HandleMovement(const FInputActionValue& Value);
    
    UFUNCTION()
    void HandleLook(const FInputActionValue& Value);

    UFUNCTION()
    void HandleJumpStart(const FInputActionValue& Value);
    
    UFUNCTION()
    void HandleJumpEnd(const FInputActionValue& Value);
    
};
```

***TQPlayerController.cpp***

```cpp
// Fill out your copyright notice in the Description page of Project Settings.


#include "TQPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "TQPlayerCharacter.h"

ATQPlayerController::ATQPlayerController()
{
    bShowMouseCursor = false;
}

void ATQPlayerController::BeginPlay()
{
    Super::BeginPlay();
    const FInputModeGameOnly InputMode;
    SetInputMode(InputMode);
    bShowMouseCursor = false;
    UE_LOG(LogTemp, Warning, TEXT("ATQPlayerController::BeginPlay()"));
    UE_LOG(LogTemp, Warning, TEXT("Controller: %s"),
        *GetNameSafe(GetPawn()));
}

void ATQPlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    if (const TObjectPtr<ULocalPlayer> LocalPlayer = Cast<ULocalPlayer>(Player))
    {
        if (const TObjectPtr<UEnhancedInputLocalPlayerSubsystem> Subsystem 
            = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
        {
            if (Player3rdPersonIMC)
            {
                Subsystem->AddMappingContext(Player3rdPersonIMC, MappingPriority);
            }
        }
    }
}

void ATQPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    if (const TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = 
        Cast<UEnhancedInputComponent>(InputComponent))
    {
        if (MoveAction)
        {
            EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATQPlayerController::HandleMovement);
        }
        if (LookAction)
            EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATQPlayerController::HandleLook);

        if (JumpAction)
        {
            // Bind both Pressed (Triggered) and Released (Completed) structural events
            EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ATQPlayerController::HandleJumpStart);
            EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ATQPlayerController::HandleJumpEnd);
        }
    }
}

void ATQPlayerController::HandleMovement(const FInputActionValue& Value)
{
    
    const FVector2D MovementVector = Value.Get<FVector2D>();
    if (TObjectPtr<ATQPlayerCharacter> TargetCharacter = Cast<ATQPlayerCharacter>(GetPawn()))
    {
        TargetCharacter->MoveHandler(MovementVector);
    }
}

void ATQPlayerController::HandleLook(const FInputActionValue& Value)
{
    const FVector2D MovementVector = Value.Get<FVector2D>();
    if (TObjectPtr<ATQPlayerCharacter> TargetCharacter = Cast<ATQPlayerCharacter>(GetPawn()))
    {
        TargetCharacter->LookHandler(MovementVector);
    }
}


void ATQPlayerController::HandleJumpStart(const FInputActionValue& Value)
{
    if (TObjectPtr<ATQPlayerCharacter> TargetCharacter = Cast<ATQPlayerCharacter>(GetPawn()))
    {
        TargetCharacter->JumpHandler();
    }
}

void ATQPlayerController::HandleJumpEnd(const FInputActionValue& Value)
{
    if (TObjectPtr<ATQPlayerCharacter> TargetCharacter = Cast<ATQPlayerCharacter>(GetPawn()))
    {
        TargetCharacter->JumpingStopHandler();
    }
}
```

***TQPlayGameMode.h***

```cpp
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TQPlayGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TINYQUEST_API ATQPlayGameMode : public AGameModeBase
{
    GENERATED_BODY()
    
    
public:
    ATQPlayGameMode();

protected:
    virtual void BeginPlay() override;
};
```

***TQPlayGameMode.cpp***

```cpp
// Fill out your copyright notice in the Description page of Project Settings.


#include "TQPlayGameMode.h"
#include "TQPlayerController.h"

ATQPlayGameMode::ATQPlayGameMode()
{
    PlayerControllerClass = ATQPlayerController::StaticClass();
}

void ATQPlayGameMode::BeginPlay()
{
    Super::BeginPlay();
}
```