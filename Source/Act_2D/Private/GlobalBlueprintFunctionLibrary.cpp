// Fill out your copyright notice in the Description page of Project Settings.


#include "GlobalBlueprintFunctionLibrary.h"
#include "Engine/GameInstance.h"
#include "Act_2DGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

UGameInstance* UGlobalBlueprintFunctionLibrary::TempGameInstance = nullptr;

void UGlobalBlueprintFunctionLibrary::SetGetGameInstance(UGameInstance* InGameInstance)
{
	TempGameInstance = InGameInstance;
}

UGameInstance* UGlobalBlueprintFunctionLibrary::GetGameInstance()
{
	return TempGameInstance;
}

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

void UGlobalBlueprintFunctionLibrary::SetGlobalDelay(float TimeFlowRate, float Duration)
{
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		UWorld* World = GetGameInstance()->GetWorld();
		if (World)
		{
			UGameplayStatics::SetGlobalTimeDilation(GameInstance, TimeFlowRate);
			static FTimerHandle DelayRestoreTimerHandle;
			auto RestoreFunction = [&]()->void
			{
				UGameInstance* MewGameInstance = UGlobalBlueprintFunctionLibrary::GetGameInstance();
				UGameplayStatics::SetGlobalTimeDilation(MewGameInstance, 1.0f);
				MewGameInstance->GetWorld()->GetTimerManager().ClearTimer(DelayRestoreTimerHandle);
			};
			auto RestoreDelegate = FTimerDelegate::CreateLambda(RestoreFunction);
			World->GetTimerManager().SetTimer(DelayRestoreTimerHandle, RestoreDelegate, TimeFlowRate * Duration, false);
		}
	}
}

APlayerCharacter* UGlobalBlueprintFunctionLibrary::GetPlayerCharacter(UObject* WorldContext)
{
	UAct_2DGameInstance* GameInstance = Cast<UAct_2DGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		return GameInstance->PlayerCharacter;
	}

	return nullptr;
}

void UGlobalBlueprintFunctionLibrary::SetPlayerCharacter(UObject* WorldContext, APlayerCharacter* NewPlayerCharacter)
{
	UAct_2DGameInstance* GameInstance = Cast<UAct_2DGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		GameInstance->PlayerCharacter = NewPlayerCharacter;
	}
}

UUserWidget* UGlobalBlueprintFunctionLibrary::GetMainUI(UObject* WorldContext)
{
	UAct_2DGameInstance* GameInstance = Cast<UAct_2DGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		return GameInstance->MainUI;
	}

	return nullptr;
}

void UGlobalBlueprintFunctionLibrary::SetMainUI(UObject* WorldContext, UUserWidget* NewUI)
{
	UAct_2DGameInstance* GameInstance = Cast<UAct_2DGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		GameInstance->MainUI = NewUI;
	}

}
