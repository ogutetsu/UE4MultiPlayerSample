// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"

#include "MenuSystem/MenuInterface.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSAMPLE_API UMyGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

public:

	UMyGameInstance(const FObjectInitializer & ObjectInitializer);


	virtual void Init();

	UFUNCTION(BlueprintCallable)
	void LoadMenu();
	UFUNCTION(BlueprintCallable)
	void InGameLoadMenu();


	UFUNCTION(Exec)
	void Host() override;

	UFUNCTION(Exec)
	void Join(const FString& Address) override;


	void LoadMainMenu() override;

private:
	TSubclassOf<class UUserWidget> MenuClass;
	TSubclassOf<class UUserWidget> InGameMenuClass;

	class UMainMenu* Menu;

	IOnlineSessionPtr SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void OnFindSessionsComplete(bool Success);


	void CreateSession();

};
