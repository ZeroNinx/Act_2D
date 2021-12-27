// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

//自定义
#include "StateMachine.h"
#include "ActorInterface.h"

#define eps 1e-7//浮点数误差

//UE4
#include "CoreMinimal.h"
#include "TimerManager.h"
#include "PaperCharacter.h"
#include "PaperFlipbookComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerCharacter.generated.h"

class UPlayerAttackComponent;

/**
 * 玩家类
 */

UCLASS()
class ACT_2D_API APlayerCharacter : public APaperCharacter, public IActorInterface
{
	GENERATED_BODY()

public:
	// 构造函数
	APlayerCharacter();

	//角色方向是否朝右
	bool bFacingRight;

	//设置状态
	void SetState(EState NewState);

	//取得状态
	EState GetState();

	//是否在状态中
	bool IsInState(EState InState);

	//调整状态
	void UpdateState();

	//获得攻击组件
	UPlayerAttackComponent* GetAttackComponent();

	//受击函数
	void Hit_Implementation(AActor* Attacker, FAttackProperty AttackProperty) override;

	//全局时间函数
	void SetGlobalDelay(float Delation, float DelayDuration);

protected:

	//单帧动画完成时
	UFUNCTION()
	void OnFlipookFinishedPlaying();

protected:

	//弹簧臂组件
	USpringArmComponent* SpringArmComponent;

	//相机组件
	UCameraComponent* CameraComponent;
	
	//状态机
	UStateMachine* StateMachine;

	//攻击组件
	UPlayerAttackComponent* AttackComponent;

	//打击感延时定时器句柄
	FTimerHandle HitDelayTimerHandle;
	
	//默认动画
	UPaperFlipbook* IdleFlipbook;

	//奔跑动画
	UPaperFlipbook* RunningFlipbook;

	//跳跃动画
	UPaperFlipbook* JumpingFlipbook;

	//下落动画
	UPaperFlipbook* FallingFlipbook;

	//受击动画
	UPaperFlipbook* HitFlipbook;

protected:

	// 游戏开始执行
	virtual void BeginPlay() override;
	
	//Tick函数
	virtual void Tick(float DeltaTime) override;

	//调整方向
	void UpdateDirection();

	//初始化动画资源
	void InitAnimation();

	//调整动画
	void UpdateAnimation();



};
