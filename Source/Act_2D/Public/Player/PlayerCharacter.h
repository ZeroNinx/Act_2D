// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

//自定义
#include "StateMachine.h"
#include "PlayerAttackComponent.h"
#define UUID_ATTACK_INPUT_DELAY 0x1
#define UUID_ATTACK_RESTORE 0x2

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

/**
 * 玩家类
 */

UCLASS()
class ACT_2D_API APlayerCharacter : public APaperCharacter
{
	GENERATED_BODY()

public:
	// 构造函数
	APlayerCharacter();

	//角色方向是否朝右
	UPROPERTY(BlueprintReadOnly)
	bool bFacingRight;

	//角色是否正在跳跃
	UPROPERTY(BlueprintReadOnly)
	bool bJumping;


	//设置状态
	UFUNCTION(BlueprintCallable)
	void SetState(EState NewState);

	//取得状态
	UFUNCTION(BlueprintCallable)
	EState GetState();

	//调整状态
	UFUNCTION()
	void UpdateState();

	//获得攻击组件
	UFUNCTION(BlueprintCallable)
	UPlayerAttackComponent* GetAttackComponent();

protected:

	//弹簧臂组件
	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	USpringArmComponent* SpringArmComponent;

	//相机组件
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UCameraComponent* CameraComponent;
	
	//状态机
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UStateMachine* StateMachine;

	//攻击组件
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UPlayerAttackComponent* AttackComponent;

	// 游戏开始执行
	virtual void BeginPlay() override;
	
	//Tick函数
	virtual void Tick(float DeltaTime) override;

	//调整方向
	UFUNCTION(BlueprintCallable)
	void UpdateDirection();

	//调整动画
	UFUNCTION()
	void UpdateAnimation();

};
