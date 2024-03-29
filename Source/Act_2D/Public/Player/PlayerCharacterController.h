// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//自定义
#include "Act_2DTypes.h"

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

protected:

	//从攻击中恢复
	UFUNCTION()
	void RestoreFromAttack();

protected:

	//玩家指针
	APlayerCharacter* PlayerCharacter;

	//攻击组件指针
	UPlayerAttackComponent* AttackComponent;

	//攻击延迟定时器句柄
	FTimerHandle AttackDelayHandle;

	//延迟输入间隔（秒）
	float AttackInputDuration = 0.03f;

	//是否等待延迟输入
	bool bWaitingInput;

	//攻击键是否按下
	bool bAttackPressed = false;

	//特殊键是否按下
	bool bSpecialPressed = false;

	//扳机键是否按下
	bool bTriggerPressed = false;

	//跳跃键是否按下
	bool bJumpPressed = false;

	//上方向键是否按下
	bool bUpPressed = false;

	//下方向键是否按下
	bool bDownPressed = false;

	//左方向键是否按下
	bool bLeftPressed = false;

	//右方向键是否按下
	bool bRightPressed = false;

	//输入组合
	FKeyCombination NextkKeyCombation;

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

	//记录输入组合
	void RecordKeyCombination();

	//准备攻击
	void PrepareAttack();

	//添加攻击输入
	void AddAttackInput();

};
