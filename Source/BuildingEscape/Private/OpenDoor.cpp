// Copyright stuff here

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/PlayerController.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	OpenAngle += InitialYaw;
	FindAudioComponent();

	if (!PressurePlate)
		UE_LOG(LogTemp, Error, TEXT("%s has no pressure plate"), *GetOwner()->GetName());
}

void UOpenDoor::FindAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();

	if (!AudioComponent)
		UE_LOG(LogTemp, Error, TEXT("%s has no audio component"), *GetOwner()->GetName());
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PressurePlate) return;

	if (TotalMassOfActors() > MassToOpenDoor)
		OpenDoor(DeltaTime);
	else if (HasShutDelayExpired())
		CloseDoor(DeltaTime);

	DoorSoundPlayed = false;
}

inline bool UOpenDoor::HasShutDelayExpired() const
{
	return (GetWorld()->GetTimeSeconds() - DoorLastOpened) > DoorShutDelay;
}

inline void UOpenDoor::OpenDoor(float DeltaTime)
{
	RotateDoor(CurrentYaw, OpenAngle, DeltaTime, DoorOpenSpeed);
	DoorLastOpened = GetWorld()->GetTimeSeconds();
	PlayDoorSound();
}

inline void UOpenDoor::CloseDoor(float DeltaTime)
{
	RotateDoor(CurrentYaw, InitialYaw, DeltaTime, DoorCloseSpeed);
	PlayDoorSound();
}

inline void UOpenDoor::PlayDoorSound()
{
	if (!AudioComponent || DoorSoundPlayed)
		return;

	AudioComponent->Play();
	DoorSoundPlayed = true;
}

float UOpenDoor::TotalMassOfActors() const
{
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has no pressure plate"), *GetOwner()->GetName());
		return 0.f;
	}

	float TotalMass = 0.f;
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	for (const AActor* actor : OverlappingActors)
	{
		UPrimitiveComponent* Component = actor->FindComponentByClass<UPrimitiveComponent>();

		if (Component)
			TotalMass += Component->GetMass();
	}

	return TotalMass;
}

void UOpenDoor::RotateDoor(
	float StartingYaw,
	float EndingYaw,
	float DeltaTime,
	float DoorSwingSpeed
) {
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	CurrentYaw = FMath::Lerp(StartingYaw, EndingYaw, DeltaTime * DoorSwingSpeed);
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);
}

