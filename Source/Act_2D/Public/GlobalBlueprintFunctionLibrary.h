// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GlobalBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ACT_2D_API UGlobalBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	static void LogWarning(FString LogText);

	UFUNCTION(BlueprintCallable)
	static UObject* LoadAssetByClassPath(UObject* WorldContext, FSoftObjectPath ObjectPath);

	UFUNCTION(BlueprintCallable)
	static APlayerCharacter* GetPlayerCharacter(UWorld* World);

	UFUNCTION(BlueprintCallable)
	static void UpdatePlayerCharacter(UWorld* World, APlayerCharacter* NewPlayerCharacter);

	UFUNCTION(BlueprintCallable)
	static UUserWidget* GetMainUI(UWorld* World);

	UFUNCTION(BlueprintCallable)
	static void UpdateMainUI(UWorld* World, UUserWidget* NewUI);
};
