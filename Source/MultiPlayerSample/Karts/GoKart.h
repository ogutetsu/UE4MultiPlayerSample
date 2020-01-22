// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GoKart.generated.h"


USTRUCT()
struct FGoKartMove
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	float Throttle;
	UPROPERTY()
	float SteeringThrow;

	UPROPERTY()
	float DeltaTime;
	UPROPERTY()
	float Time;

};


USTRUCT()
struct FGoKartState
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FTransform Transform;
	UPROPERTY()
	FVector Velocity;

	UPROPERTY()
	FGoKartMove LastMove;
};



UCLASS()
class MULTIPLAYERSAMPLE_API AGoKart : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGoKart();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	FString GetEnumText(ENetRole Role);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


private:
	FVector GetAirResistance();
	FVector GetRollingResistance();

	void UpdateLocationFromVelocity(float DeltaTime);
	void ApplyRotation(float DeltaTime);

	UPROPERTY(EditAnywhere)
	float Mass = 1000;

	UPROPERTY(EditAnywhere)
	float MaxDrivingForce = 10000;


	UPROPERTY(EditAnywhere)
	float MinTurningRadius = 10;


	UPROPERTY(EditAnywhere)
	float DragCoefficient = 16;

	UPROPERTY(EditAnywhere)
	float RollingCoefficient = 0.015;

	void MoveForward(float Value);
	void MoveRight(float Value);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_MoveForward(float Value);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_MoveRight(float Value);

	UPROPERTY(Replicated)
	FVector Velocity;

	UPROPERTY(ReplicatedUsing= OnRep_ReplicatedTransform)
	FTransform ReplicatedTransform;
	UFUNCTION()
	void OnRep_ReplicatedTransform();

	UPROPERTY(Replicated)
	FRotator ReplicatedRotation;

	UPROPERTY(Replicated)
	float Throttle;
	UPROPERTY(Replicated)
	float SteeringThrow;


};
