// Copyright stuff here

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:
	UGrabber();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;


private:
	UPROPERTY(EditAnywhere) float Reach = 100.0f;
	UPROPERTY() UInputComponent* InputComponent = nullptr;
	UPROPERTY() UPhysicsHandleComponent* PhysicsHandle = nullptr;
	FVector PlayerViewPointPosition;
	FRotator PlayerViewPointRotation;

private:
	void SetupInputComponent();
	void FindPhysicsHandle();
	void Grab();
	void Release();
	FHitResult GetFirstPhysicsBodyInReach();
	FVector GetPlayerReach();
};
