// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GoKartMovementComponent.h"
#include "GoKart.generated.h"



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
	
	void ClearAcknowledgeMoves(FGoKartMove LastMove);

	
	void MoveForward(float Value);
	void MoveRight(float Value);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SendMove(FGoKartMove Move);


	UPROPERTY(ReplicatedUsing = OnRep_ServerState)
	FGoKartState ServerState;

	UFUNCTION()
	void OnRep_ServerState();

	UPROPERTY(Replicated)
	FRotator ReplicatedRotation;


	TArray<FGoKartMove> UnacknowledgedMoves;

	UPROPERTY(EditAnywhere)
	UGoKartMovementComponent* MovementComponent;

};
