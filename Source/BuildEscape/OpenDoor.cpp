// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"


// Sets default values for this component's properties this is a change hello my name is jeff hi test101 test?
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

	
	if (PressurePlate && PressurePlate -> IsOverlappingActor(ActorThatOpens))
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