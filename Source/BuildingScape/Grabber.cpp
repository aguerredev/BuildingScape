// Copyright Sebastian Aguerre 2020.
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	//Check for the Physics Handle Component
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
		/* Physics is found */
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Physics Handle Component found on %s!"), *(GetOwner()->GetName()));
	}
	
	//Check for Input Component
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Input Component Found"), *(GetOwner()->GetName()));
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Input Component MISSING"), *(GetOwner()->GetName()));
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed"));
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get Player viewpoint
	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewpointLocation, 
		OUT PlayerViewpointRotation
	);

	FVector LineTracedEnd = PlayerViewpointLocation + PlayerViewpointRotation.Vector() * Reach;

	DrawDebugLine(
		GetWorld(),
		PlayerViewpointLocation,
		LineTracedEnd,
		FColor(0, 255, 0),
		false,
		0.f,
		0,
		5.f
	);

	FHitResult Hit;
	// Ray-cast out to a certain distance (Reach)
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewpointLocation,
		LineTracedEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	AActor* ActorHit = Hit.GetActor();

	if(ActorHit)
	{
		UE_LOG(LogTemp, Error, TEXT("Line Trace has hit: %s"), *(ActorHit->GetName()));
	}

	// See what it hits
}

