// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GlobalBlueprintFunctionLibrary.generated.h"

/**
 * ȫ����ͼ������
 */
UCLASS()
class ACT_2D_API UGlobalBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	static void SetGetGameInstance(UGameInstance* GameInstance);

	UFUNCTION(BlueprintCallable)
	static UGameInstance* GetGameInstance();

	UFUNCTION(BlueprintCallable)
	static void LogWarning(FString LogText);

	UFUNCTION(BlueprintCallable)
	static UObject* LoadAssetByClassPath(FSoftObjectPath ObjectPath);

	UFUNCTION(BlueprintCallable)
	static void SetGlobalDelay(float TimeFlowRate, float Duration);

	UFUNCTION(BlueprintCallable)
	static APlayerCharacter* GetPlayerCharacter();

	UFUNCTION(BlueprintCallable)
	static void SetPlayerCharacter(APlayerCharacter* NewPlayerCharacter);

	UFUNCTION(BlueprintCallable)
	static UUserWidget* GetMainUI();

	UFUNCTION(BlueprintCallable)
	static void SetMainUI(UUserWidget* NewUI);

private:

	static UGameInstance* TempGameInstance;

};