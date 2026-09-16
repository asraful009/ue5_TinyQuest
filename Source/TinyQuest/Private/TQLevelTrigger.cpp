// Fill out your copyright notice in the Description page of Project Settings.


#include "TQLevelTrigger.h"

#include "TQPlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATQLevelTrigger::ATQLevelTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;
	TriggerBox->SetBoxExtent(FVector(100.0f, 100.0f, 100.0f));

	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriggerBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	TriggerBox->SetGenerateOverlapEvents(true);

	TriggerBox->OnComponentBeginOverlap.AddDynamic(
		this,
		&ATQLevelTrigger::OnTriggerBeginOverlap
	);

}

// Called when the game starts or when spawned
void ATQLevelTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATQLevelTrigger::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	if (!OtherActor)
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("TQLevelTrigger: OtherActor is NULL")
        );

        return;
    }

    // --------------------------------------------------
    // Debug Actor Name
    // --------------------------------------------------

    UE_LOG(
        LogTemp,
        Warning,
        TEXT("TQLevelTrigger: Actor entered = %s"),
        *OtherActor->GetName()
    );

    // --------------------------------------------------
    // Check Pawn
    // --------------------------------------------------

    if (!OtherActor->IsA<APawn>())
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("TQLevelTrigger: Actor is NOT a Pawn")
        );

        return;
    }

    UE_LOG(
        LogTemp,
        Warning,
        TEXT("TQLevelTrigger: Pawn detected")
    );

    // --------------------------------------------------
    // Check Next Level
    // --------------------------------------------------

    if (NextLevel.IsNull())
    {
        UE_LOG(
            LogTemp,
            Error,
            TEXT("TQLevelTrigger: NextLevel is NOT assigned")
        );

        return;
    }

    // --------------------------------------------------
    // Get Map Package Name
    // --------------------------------------------------

    const FString PackageName =
        NextLevel.ToSoftObjectPath().GetLongPackageName();

    if (PackageName.IsEmpty())
    {
        UE_LOG(
            LogTemp,
            Error,
            TEXT("TQLevelTrigger: Level package name is empty")
        );

        return;
    }

    // --------------------------------------------------
    // Debug Level
    // --------------------------------------------------

    UE_LOG(
        LogTemp,
        Warning,
        TEXT("TQLevelTrigger: Loading level = %s"),
        *PackageName
    );

    // --------------------------------------------------
    // Open Level
    // --------------------------------------------------

    UGameplayStatics::OpenLevel(
        this,
        FName(*PackageName)
    );
	
}


// Called every frame
void ATQLevelTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

