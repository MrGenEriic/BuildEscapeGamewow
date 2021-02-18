// Fill out your copyright notice in the Description page of Project Settings.


#include "TheGrabber.h"
#include "DrawDebugHelpers.h"
#include "CollisionQueryParams.h"
#include "Engine/EngineTypes.h" // Keep this??????????????
#include "Components/InputComponent.h"
#include "GameFramework/PlayerController.h"

#define OUT

// Sets default values for this component's properties
UTheGrabber::UTheGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UTheGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandle();
	GrabFunction();

	// DEBUG CODE
	//UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty!"));

}


void UTheGrabber::GrabFunction()
{
	InputComponent = GetOwner() -> FindComponentByClass<UInputComponent>();

	if (InputComponent) //testing this change
	{
		UE_LOG(LogTemp, Warning, TEXT("Found the input component on: %s"), *GetOwner() -> GetName())
		InputComponent -> BindAction("Grab", IE_Pressed, this, &UTheGrabber::Grab);
		InputComponent -> BindAction("Grab", IE_Released, this, &UTheGrabber::Release);
	}	
}


void UTheGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner() -> FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle)
	{
		// Doing nothing, if it exists then no need for anything to happen this is simply error logging to easily find which ones don't have it attached
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Error No Physics Handle Component Found on %s"), *GetOwner() -> GetName());
	}
}


void UTheGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber: Pressed"));
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld() -> GetFirstPlayerController() -> GetPlayerViewPoint(
		OUT PlayerViewPointLocation, 
		OUT PlayerViewPointRotation
		);

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;


	FHitResult HitResult = GetPhysicsBodyForGrab();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	// Attach Physics Handle (if we hit something)

	if (HitResult.GetActor())
	{
		PhysicsHandle -> GrabComponentAtLocation(ComponentToGrab, NAME_None, LineTraceEnd);
	}
		


}

void UTheGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber: Released"));
	// Release Physics Handle
	PhysicsHandle -> ReleaseComponent();

	
}



// Called every frame
void UTheGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld() -> GetFirstPlayerController() -> GetPlayerViewPoint(
		OUT PlayerViewPointLocation, 
		OUT PlayerViewPointRotation
		);

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	if (PhysicsHandle -> GrabbedComponent)
	{
		PhysicsHandle -> SetTargetLocation(LineTraceEnd);
	}
	

	// If physics handle is attach
		// Move the object we are holding

}

FHitResult UTheGrabber::GetPhysicsBodyForGrab() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld() -> GetFirstPlayerController() -> GetPlayerViewPoint(
		OUT PlayerViewPointLocation, 
		OUT PlayerViewPointRotation
		);

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
	//FRotator(Reach,0.f,0.f)

	FHitResult ObjectHit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	GetWorld() -> LineTraceSingleByObjectType(
		OUT ObjectHit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	AActor* ObjectHitName = ObjectHit.GetActor();

	if (ObjectHitName)
	{
		UE_LOG(LogTemp, Warning, TEXT("Object Hit is Called: %s"), *(ObjectHitName -> GetName()));
	}

	return ObjectHit;


	/*DrawDebugLine(
	GetWorld(),
	PlayerViewPointLocation,
	LineTraceEnd,
	FColor(0, 255, 0),
	false,
	0.f,
	0,
	5.f
	);*/

	//UE_LOG(LogTemp, Warning, TEXT("%s, %s"), *PlayerViewPointLocation.ToString(), *PlayerViewPointRotation.ToString());
}