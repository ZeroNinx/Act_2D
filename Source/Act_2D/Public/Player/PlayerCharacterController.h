// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//自定义
#include "Utils/Types.h"

//UE4
#include "CoreMinimal.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/PlayerController.h"
#include "PlayerCharacterController.generated.h"

class APlayerCharacter;
class UPlayerAttackComponent;

/**
 * 玩家控制器类
 */

UCLASS()
class ACT_2D_API APlayerCharacterController : public APlayerController
{
	GENERATED_BODY()
	
public:

	//构造函数
	APlayerCharacterController();

	// 获取当前按键组合
	UFUNCTION(BlueprintCallable)
	FKeyCombination GetNextKeyCombation();

protected:

	// 按键按下映射
	TMap<EGameKeyType, bool> KeyPressMap;

	// 输入队列
	TQueue<FKeyCombination> InputCombinationQueue;

	//玩家指针
	APlayerCharacter* PlayerCharacter;

	//攻击组件指针
	UPlayerAttackComponent* AttackComponent;

protected:

	//上键
	void UpPressed();
	void UpReleased();

	//下键
	void DownPressed();
	void DownReleased();

	//左键
	void LeftPressed();
	void LeftReleased();

	//右键
	void RightPressed();
	void RightReleased();

	//攻击键
	void AttackPressed();
	void AttackReleased();

	//特殊键
	void SpecialPressed();
	void SpecialReleased();

	//扳机键
	void TriggerPressed();
	void TriggerReleased();

	//跳跃键
	void JumpPressed();
	void JumpReleased();

protected:

	virtual void BeginPlay() override;

	//初始化主UI
	void InitializeMainUI();

	//当开始控制时
	void OnPossess(APawn* InPawn) override;

	//初始化输入函数
	void SetupInputComponent() override;

	//Tick
	void Tick(float DeltaSeconds) override;

	//是否允许移动
	bool IsAllowMove();

	//添加输入组合
	void AddKeyCombination();

};
