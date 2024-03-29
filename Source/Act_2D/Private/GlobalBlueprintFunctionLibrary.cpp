// Fill out your copyright notice in the Description page of Project Settings.


#include "GlobalBlueprintFunctionLibrary.h"
#include "Engine/GameInstance.h"
#include "Act_2DGameInstance.h"
#include "Blueprint/UserWidget.h"

void UGlobalBlueprintFunctionLibrary::LogWarning(FString LogText)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *LogText);
}

UObject* UGlobalBlueprintFunctionLibrary::LoadAssetByClassPath(UObject* WorldContext, FSoftObjectPath ObjectPath)
{
	if (!WorldContext)
	{
		return nullptr;
	}
	return LoadObject<UObject>(WorldContext, *ObjectPath.ToString());
}

APlayerCharacter* UGlobalBlueprintFunctionLibrary::GetPlayerCharacter(UWorld* World)
{
	UGameInstance* GotGameInstance = World->GetGameInstance();
	UAct_2DGameInstance* GameInstance = Cast<UAct_2DGameInstance>(GotGameInstance);
	if (GameInstance)
	{
		return GameInstance->PlayerCharacter;
	}
	return nullptr;
}

void UGlobalBlueprintFunctionLibrary::UpdatePlayerCharacter(UWorld* World, APlayerCharacter* NewPlayerCharacter)
{
	UGameInstance* GotGameInstance = World->GetGameInstance();
	UAct_2DGameInstance* GameInstance = Cast<UAct_2DGameInstance>(GotGameInstance);
	if (GameInstance)
	{
		GameInstance->PlayerCharacter = NewPlayerCharacter;
	}
}

UUserWidget* UGlobalBlueprintFunctionLibrary::GetMainUI(UWorld* World)
{
	UGameInstance* GotGameInstance = World->GetGameInstance();
	UAct_2DGameInstance* GameInstance = Cast<UAct_2DGameInstance>(GotGameInstance);
	if (GameInstance)
	{
		return GameInstance->MainUI;
	}
	return nullptr;
}

void UGlobalBlueprintFunctionLibrary::UpdateMainUI(UWorld* World, UUserWidget* NewUI)
{
	UGameInstance* GotGameInstance = World->GetGameInstance();
	UAct_2DGameInstance* GameInstance = Cast<UAct_2DGameInstance>(GotGameInstance);
	if (GameInstance)
	{
		GameInstance->MainUI = NewUI;
	}
}
