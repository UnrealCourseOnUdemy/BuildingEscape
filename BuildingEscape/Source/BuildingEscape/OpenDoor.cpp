// Copyright Ben Tristem 2016.

#include "BuildingEscape.h"
#include "OpenDoor.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"

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

	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has no attached TriggerVolume!"), *GetOwner()->GetName());
	}

	Owner = GetOwner();
	if (!Owner)
		UE_LOG(LogTemp, Error, TEXT("OpenDoor component without owner??"));
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// If the total mass on plate is greater than a threshold, open the door
	if (GetTotalMassOfActorsOnPlate() > TriggerMass)
	{
		OnOpen.Broadcast();
	}
	else
	{
		OnClose.Broadcast();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate() const
{
	float TotalMass = 0.f;

	// Find all overlapping actors
	TArray<AActor*> OverlappingActors;

	if (!PressurePlate) return TotalMass;

	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	// Iterate through them adding all the masses

	for (const auto& Actor : OverlappingActors)
	{
		// Look for primitive component, that contains the mass
		UPrimitiveComponent* PrimitiveComponent = Actor->FindComponentByClass<UPrimitiveComponent>();

		if (PrimitiveComponent)
		{
			TotalMass += PrimitiveComponent->CalculateMass();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("%s has no Primitive Component defined"), *Actor->GetName());
		}
	}

	return TotalMass;
}
