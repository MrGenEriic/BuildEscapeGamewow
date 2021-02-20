// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

#define OUT


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	InitialYaw = GetOwner() -> GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	TargetYaw += InitialYaw; // Same as TargetYaw = TargetYaw + InitialYaw.  Target Yaw has the value of 90 in the header file so adds it to itself plus the initial yaw



	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has open door component on it, but no pressure plate"), *GetOwner() -> GetName());
	}

	ActorThatOpens = GetWorld() -> GetFirstPlayerController() -> GetPawn();




	
	//FRotator CurrentRotation = GetOwner() -> GetActorRotation();
	//CurrentRotation.Yaw = 90.0f;
	//FRotator OpenDoor = {0.f, 90.0f, 0.f};
	//GetOwner() -> SetActorRotation(OpenDoor);	
}



// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CurrentTime = GetWorld() -> GetTimeSeconds();

	//PressurePlate && PressurePlate -> IsOverlappingActor(ActorThatOpens) THIS CODE IS FOR CHECKING ONLY FOR PLAYER ACTOR
		//To my knowledge, it is saying if the pressureplate exists in and of itself, AND the pressureplate is overlapping with the Actor then it would be TRUE
	if (TotalMassOfActors() > 50.f)
	{
		UOpenDoor::OpenDoor(DeltaTime);
		TimeWhenDoorOpened = GetWorld() -> GetTimeSeconds();
	}
	else
	{
		if (CurrentTime - TimeWhenDoorOpened > CloseDoorDelay)
		{
			UOpenDoor::CloseDoor(DeltaTime);
		}
	}
	


}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	CurrentYaw = FMath::FInterpTo(CurrentYaw, TargetYaw, DeltaTime, 2.0f);
	FRotator DoorRotation = GetOwner() -> GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner() -> SetActorRotation(DoorRotation);


	//UE_LOG(LogTemp, Warning, TEXT("%s"), *GetOwner() -> GetActorRotation().ToString());
	//UE_LOG(LogTemp, Warning, TEXT("%f"), GetOwner() -> GetActorRotation().Yaw);
	//float CurrentYaw = GetOwner() -> GetActorRotation().Yaw;
	//Rotator OpenDoor = {0.f, 0.f, 0.f};
	//OpenDoor.Yaw = FMath::FInterpTo(CurrentYaw, TargetYaw, DeltaTime, 2.0f);
	//GetOwner() -> SetActorRotation(OpenDoor);

}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	//CurrentYaw = GetOwner() -> GetActorRotation().Yaw;

	CurrentYaw = FMath::FInterpTo(CurrentYaw, InitialYaw, DeltaTime, 4.0f);
	FRotator CloseRotation = GetOwner() -> GetActorRotation();
	CloseRotation.Yaw = CurrentYaw;
	GetOwner() -> SetActorRotation(CloseRotation);

}


float UOpenDoor::TotalMassOfActors() const
{
	float TotalMass = 0.f;
	TArray<AActor*> OverlappingActors;
	PressurePlate -> GetOverlappingActors(OUT OverlappingActors);

	// For this code: AActor* Actor; it is saying based on the type (AActor*) it will be pointing to an Actor in memory so *Actor is the result where Actor is looking in the memory for Actors

	// So below, the Actor is setting the initial value to essentially 0 on the initial code execution pointing to no specfiic actor 



	for (AActor* Actor : OverlappingActors ) // First part is same as saying int32 ActorIndex = 0;, the second part is saying the condition to execute is if any Overlapping actors exist
	{
		TotalMass += Actor -> FindComponentByClass<UPrimitiveComponent>() -> GetMass();
	}
	





	return TotalMass;



}