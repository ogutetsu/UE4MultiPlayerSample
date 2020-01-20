// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSubsystem.h"
#include "MenuSystem/MainMenu.h"
#include "MenuSystem/MenuWidget.h"


UMyGameInstance::UMyGameInstance(const FObjectInitializer & ObjectInitializer)
{
	
	ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (!ensure(MenuBPClass.Class != nullptr)) return;

	MenuClass = MenuBPClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBPClass(TEXT("/Game/MenuSystem/WBP_InGameMenu"));
	if (!ensure(InGameMenuBPClass.Class != nullptr)) return;

	InGameMenuClass = InGameMenuBPClass.Class;
	
}

void UMyGameInstance::Init()
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if(Subsystem != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found subsystem %s"), *Subsystem->GetSubsystemName().ToString());
		IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("Found session interface %s"), *Subsystem->GetSubsystemName().ToString());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Found no subsystem"));
	}

	//UE_LOG(LogTemp, Warning, TEXT("Found class %s"), *MenuClass->GetName());
}

void UMyGameInstance::LoadMenu()
{
	if (!ensure(MenuClass != nullptr)) return;

	Menu = CreateWidget<UMainMenu>(this, MenuClass);

	if (!ensure(Menu != nullptr)) return;

	Menu->Setup();
	Menu->SetMenuInterface(this);
}

void UMyGameInstance::InGameLoadMenu()
{
	if (!ensure(MenuClass != nullptr)) return;

	UMenuWidget* Menu = CreateWidget<UMenuWidget>(this, InGameMenuClass);

	if (!ensure(Menu != nullptr)) return;

	Menu->Setup();
	Menu->SetMenuInterface(this);
}

void UMyGameInstance::Host()
{
	if(Menu != nullptr)
	{
		Menu->Teardown();
	}
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	World->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");

}

void UMyGameInstance::Join(const FString & Address)
{
	if (Menu != nullptr)
	{
		Menu->Teardown();
	}
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void UMyGameInstance::LoadMainMenu()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel("/Game/MenuSystem/MainMenu", ETravelType::TRAVEL_Absolute);

}


