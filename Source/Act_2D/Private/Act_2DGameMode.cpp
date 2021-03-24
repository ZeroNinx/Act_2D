// Fill out your copyright notice in the Description page of Project Settings.


#include "Act_2DGameMode.h"

//构造函数
AAct_2DGameMode::AAct_2DGameMode()
{
	//设定默认类
	PlayerControllerClass = APlayerCharacterController::StaticClass();
	DefaultPawnClass = APlayerCharacter::StaticClass();
}

//游戏开始时执行
void AAct_2DGameMode::BeginPlay()
{
	ResetPlayer();
}

//重置玩家
void AAct_2DGameMode::ResetPlayer()
{
	//消灭玩家
	APlayerCharacterController* Controller = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (Controller->GetPawn())
	{
		Controller->GetPawn()->Destroy();
	}

	//遍历加载重生地点
	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);
	for (int i = 0; i < PlayerStarts.Num(); i++)
	{
		PlayerStart = Cast<APlayerStart>(PlayerStarts[i]);
		if (PlayerStart && PlayerStart->PlayerStartTag.IsEqual(FName("Default")))
		{
			PlayerStartTransform = PlayerStart->GetTransform();
			break;
		}
	}

	//检验重生地
	if (!PlayerStart)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), "PlayerStart Not Found");
		return;
	}

	//生成角色
	PlayerCharacter = GetWorld()->SpawnActor<APlayerCharacter>(APlayerCharacter::StaticClass(), PlayerStartTransform);
	if (!PlayerCharacter)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), "Player Spawn Failed");
	}

	//控制玩家
	Controller->Possess(PlayerCharacter);
	Controller->Setup();
}
