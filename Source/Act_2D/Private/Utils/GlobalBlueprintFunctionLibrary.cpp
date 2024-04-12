// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/GlobalBlueprintFunctionLibrary.h"
#include "Engine/GameInstance.h"
#include "Act_2DGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Skill/Monster/MonsterSkill.h"
#include "GameFramework/CharacterMovementComponent.h"

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

UObject* UGlobalBlueprintFunctionLibrary::LoadAssetByClassPath(FSoftObjectPath ObjectPath)
{
	UAct_2DGameInstance* GameInstance = Cast<UAct_2DGameInstance>(GetGameInstance());
	if (!GameInstance)
	{
		return nullptr;
	}
	return LoadObject<UObject>(GameInstance, *ObjectPath.ToString());
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

APlayerCharacter* UGlobalBlueprintFunctionLibrary::GetPlayerCharacter()
{
	UAct_2DGameInstance* GameInstance = Cast<UAct_2DGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		return GameInstance->PlayerCharacter;
	}

	return nullptr;
}

void UGlobalBlueprintFunctionLibrary::SetPlayerCharacter(APlayerCharacter* NewPlayerCharacter)
{
	UAct_2DGameInstance* GameInstance = Cast<UAct_2DGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		GameInstance->PlayerCharacter = NewPlayerCharacter;
	}
}

class UMonsterSkill* UGlobalBlueprintFunctionLibrary::CreateMonsterSkill(UObject* Outer, TSubclassOf<UMonsterSkill> SkillClass)
{
	return NewObject<UMonsterSkill>(Outer, SkillClass);
}

void UGlobalBlueprintFunctionLibrary::AddPlayetrHorizontalVelocity(float Velocity, bool bFollowFacing)
{
	APlayerCharacter* PlayerCharacter = GetPlayerCharacter();
	if (!PlayerCharacter)
	{
		LogWarning("UGlobalBlueprintFunctionLibrary::AddPlayetrHorizontalVelocity PlayerCharacter Invalid");
		return;
	}

	UCharacterMovementComponent* Movementcomponent = PlayerCharacter->GetCharacterMovement();
	if (!Movementcomponent)
	{
		LogWarning("UGlobalBlueprintFunctionLibrary::AddPlayetrHorizontalVelocity Movementcomponent Invalid");
		return;
	}

	float VelocityXFix = Velocity * (bFollowFacing && PlayerCharacter->bFacingRight ? 1.0 : -1.0);
	FVector NewVelocity = 
	{
		Movementcomponent->Velocity.X + VelocityXFix,
		Movementcomponent->Velocity.Y,
		Movementcomponent->Velocity.Z,
	};

	Movementcomponent->Velocity = NewVelocity;
}

UUserWidget* UGlobalBlueprintFunctionLibrary::GetMainUI()
{
	UAct_2DGameInstance* GameInstance = Cast<UAct_2DGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		return GameInstance->MainUI;
	}

	return nullptr;
}

void UGlobalBlueprintFunctionLibrary::SetMainUI(UUserWidget* NewUI)
{
	UAct_2DGameInstance* GameInstance = Cast<UAct_2DGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		GameInstance->MainUI = NewUI;
	}

}
