// Copyright stuff here

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void FindAudioComponent();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


private:
	void OpenDoor(float DeltaTime);
	void PlayDoorSound();
	void CloseDoor(float DeltaTime);
	float TotalMassOfActors() const;
	void RotateDoor(float StartingYaw, float EndingYaw, float DeltaTime, float DoorSwingSpeed);
	bool HasShutDelayExpired() const;

private:
	UPROPERTY(EditAnywhere) float OpenAngle = 90.f;
	UPROPERTY(EditAnywhere) float DoorShutDelay = 2.f;
	UPROPERTY(EditAnywhere) float DoorOpenSpeed = 1.f;
	UPROPERTY(EditAnywhere) float DoorCloseSpeed = 2.f;
	UPROPERTY(EditAnywhere) float MassToOpenDoor = 60.f;
	UPROPERTY(EditAnywhere) ATriggerVolume* PressurePlate = nullptr;
	UPROPERTY() UAudioComponent* AudioComponent = nullptr;

	float InitialYaw = 0.f;
	float CurrentYaw = 0.f;
	float DoorLastOpened = 0.f;
	bool DoorSoundPlayed = false;
};
