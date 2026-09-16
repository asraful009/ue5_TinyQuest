// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TQLevelTrigger.generated.h"

class UBoxComponent;

UCLASS()
class TINYQUEST_API ATQLevelTrigger : public AActor
{
	GENERATED_BODY()
	
protected:
	
	UPROPERTY(VisibleAnywhere, Category = "Trigger")
	TObjectPtr<UBoxComponent> TriggerBox;
	
	UPROPERTY(EditAnywhere, Category = "Level")
	TSoftObjectPtr<UWorld> NextLevel;
	
public:	
	// Sets default values for this actor's properties
	ATQLevelTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnTriggerBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
