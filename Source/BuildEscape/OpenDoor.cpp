// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"
#include "Components/AudioComponent.h"
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


	ActorThatOpens = GetWorld() -> GetFirstPlayerController() -> GetPawn(); // Not currently being used, was being used in the IF statement to determine if the door should open when the Actor is on it

	FindAudioComponent();

	//FRotator CurrentRotation = GetOwner() -> GetActorRotation();
	//CurrentRotation.Yaw = 90.0f;
	//FRotator OpenDoor = {0.f, 90.0f, 0.f};
	//GetOwner() -> SetActorRotation(OpenDoor);	
}

void UOpenDoor::FindPressurePlate()
{
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has open door component on it, but no pressure plate"), *GetOwner() -> GetName());
	}
}



void UOpenDoor::FindAudioComponent()
{
	AudioComponent = GetOwner() -> FindComponentByClass<UAudioComponent>();

	if(!AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("No Audio Component Attached to %s"), *GetOwner() -> GetName())

	}

}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CurrentTime = GetWorld() -> GetTimeSeconds();

	//PressurePlate && PressurePlate -> IsOverlappingActor(ActorThatOpens) THIS CODE IS FOR CHECKING ONLY FOR PLAYER ACTOR
		//To my knowledge, it is saying if the pressureplate exists in and of itself, AND the pressureplate is overlapping with the Actor then it would be TRUE
	if (TotalMassOfActors() > RequiredWeight)
	{
		UOpenDoor::OpenDoor(DeltaTime);
		TimeWhenDoorOpened = GetWorld() -> GetTimeSeconds();
	}
	else
	{
		if (CurrentTime - TimeWhenDoorOpened > CloseDoorDelay) // CloseDoorDelay is not the time it takes it to close, its the time you want to wait for 
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



	if(!AudioComponent) {return;}
	if (!OpenDoorSoundPlayed)
	{
		AudioComponent -> Play();
		CloseDoorSoundPlayed = false;
	}
	

	OpenDoorSoundPlayed = true;

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


	if(!AudioComponent) {return;}
	if (!CloseDoorSoundPlayed)
	{
		AudioComponent -> Play();
		OpenDoorSoundPlayed = false;
	}


	CloseDoorSoundPlayed = true;

}


float UOpenDoor::TotalMassOfActors() const
{
	float TotalMass = 0.f;
	TArray<AActor*> OverlappingActors;
	if(!PressurePlate) {return TotalMass;}  //  This function needs to return a value that is a float, so can't just return; in the case of the nullptr since the function won't like its returning nothing
	PressurePlate -> GetOverlappingActors(OUT OverlappingActors);

	// For this code: AActor* Actor; it is saying based on the type (AActor*) it will be pointing to an Actor in memory so *Actor is the result where Actor is looking in the memory for Actors

	// So below, the Actor is setting the initial value to essentially 0 on the initial code execution pointing to no specfiic actor 


	// OverlappingActors is dynamically setting the length of the array to determine how many times it needs to run through it.  So as more get added the array gets bigger which changes the for loop conditions to let it loop through
	for (AActor* Actor : OverlappingActors ) // This Syntax removes some flexibility from the other form of for loops.  The AActor* Actor is grabbing the first actor in the Array, no ability to change which one it begins looping through.  
	{
		if(!Actor) {return TotalMass;} // This one may be overkill because the code won't get to this portion if the TArray has no Actors inside of it since they are themselves pointers and the condition to run is OverlappingActors
		TotalMass += Actor -> FindComponentByClass<UPrimitiveComponent>() -> GetMass();
	}
	





	return TotalMass;



}