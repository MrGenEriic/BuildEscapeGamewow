// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties TETSTSTSTS THIS IS THE FINAL TEST PL
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	float TotalMassOfActors() const;

private:



float InitialYaw;
float CurrentYaw;
float ClosedYaw;
float CurrentClosedYaw;

UPROPERTY(EditAnywhere)
float TargetYaw = 90.0f;

UPROPERTY(EditAnywhere)
ATriggerVolume* PressurePlate;

UPROPERTY(EditAnywhere)
AActor* ActorThatOpens;

float TimeWhenDoorOpened = 0.f;
UPROPERTY(EditAnywhere)
float CloseDoorDelay = 0.3f;

float CurrentTime = 0.f;


};
