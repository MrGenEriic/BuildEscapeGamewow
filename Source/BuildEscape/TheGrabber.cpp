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

	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Error No Physics Handle Component Found on %s"), *GetOwner() -> GetName());
	}

}

FVector UTheGrabber::LineTraceEndCalc() const
{
	//UE_LOG(LogTemp, Warning, TEXT("Grabber: Pressed"));
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld() -> GetFirstPlayerController() -> GetPlayerViewPoint(
		OUT PlayerViewPointLocation, 
		OUT PlayerViewPointRotation
		);

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
	return LineTraceEnd;
}


FVector UTheGrabber::PlayerLocation() const
{

	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld() -> GetFirstPlayerController() -> GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	return PlayerViewPointLocation;


}


void UTheGrabber::Grab()
{

	FHitResult HitResult = GetPhysicsBodyForGrab();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent(); // No need for nullprt since it is being assigned to something else
	AActor* ActorHit = HitResult.GetActor();
	// Attach Physics Handle (if we hit something)

	if (ActorHit)
	{
		if(!PhysicsHandle) {return;} // Removes nullprt errors, since if the Physics handle doesn't exist but we continue down the pointer line wack errors will be thrown because its pointing from something that doesn't exist
		PhysicsHandle -> GrabComponentAtLocation(ComponentToGrab, NAME_None, LineTraceEndCalc());
	}
		


}

void UTheGrabber::Release()
{
	//UE_LOG(LogTemp, Warning, TEXT("Grabber: Released"));
	// Release Physics Handle
	if(!PhysicsHandle) {return;} // Removes nullprt errors, since if the Physics handle doesn't exist but we continue down the pointer line wack errors will be thrown because its pointing from something that doesn't exist
	PhysicsHandle -> ReleaseComponent(); // Without the above fix, if we didn't have a Physics handle for the component and tried to release it, could cash a crash since nothing to release

	
}



// Called every frame
void UTheGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	if(!PhysicsHandle) {return;} // This will take us out of the tick function completely
	if (PhysicsHandle -> GrabbedComponent)
	{
		PhysicsHandle -> SetTargetLocation(LineTraceEndCalc());
	}
	

	// If physics handle is attach
		// Move the object we are holding

}

FHitResult UTheGrabber::GetPhysicsBodyForGrab() const
{

	//FRotator(Reach,0.f,0.f)

	FHitResult ObjectHit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());


	// No need for a pointer protection here, if the object doesn't have a world with this pointer we have bigger issues xD  Unlike the above ones where it could very possible happen without major components of game not existing
	GetWorld() -> LineTraceSingleByObjectType(
		OUT ObjectHit,
		PlayerLocation(),
		LineTraceEndCalc(),
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