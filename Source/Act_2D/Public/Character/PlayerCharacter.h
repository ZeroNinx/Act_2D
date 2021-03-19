// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

//第三方
#include<cmath>

//自定义
#include "PlayerStateMachine.h"
#include "PlayerAttackComponent.h"
#define UUID_ATTACK_INPUT_DELAY 0x1
#define UUID_ATTACK_RESTORE 0x2

#define eps 1e-7//浮点数误差

//UE4
#include "CoreMinimal.h"
#include "TimerManager.h"
#include "PaperFlipbookComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PlayerCharacter.generated.h"

/**
 * 玩家类
 */

//攻击完成代理
DECLARE_DYNAMIC_DELEGATE(FOnAttackFinishedDelegate);

UCLASS()
class ACT_2D_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// 构造函数
	APlayerCharacter();

	//取得状态
	UFUNCTION(BlueprintCallable)
	ECharacterState GetState();

	//取得动画组件
	UFUNCTION(BlueprintCallable)
	UPaperFlipbookComponent* GetFlipbookComponent();

	//取得状态机
	UFUNCTION(BlueprintCallable)
	UPlayerStateMachine* GetStateMachine();

protected:

	//角色动画
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	UPaperFlipbookComponent* FlipbookComponent;
	
	//状态机
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	UPlayerStateMachine* StateMachine;

	//攻击组件
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	UPlayerAttackComponent* AttackComponent;

	//角色方向是否朝右
	UPROPERTY(BlueprintReadOnly)
	bool bFacingRight;

	//攻击延迟定时器句柄
	UPROPERTY(BlueprintReadOnly)
	FTimerHandle AttackDelayHandle;

	//延迟输入间隔（秒）
	UPROPERTY(BlueprintReadOnly)
	float AttackInputDuration = 0.05f;

	//是否等待延迟输入
	UPROPERTY(BlueprintReadOnly)
	bool bWaitingInput;

	//攻击动画播放完成代理
	FOnAttackFinishedDelegate OnAttackFinishedDelegate;

	// 游戏开始执行
	virtual void BeginPlay() override;
	


	//输入绑定
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//角色移动
	UFUNCTION()
	void MoveRight(float AxisValue);
	UFUNCTION()
	void MoveUp(float AxisValue);

	//攻击键
	UFUNCTION(BlueprintCallable)
	void AttackPresssed();
	UFUNCTION(BlueprintCallable)
	void AttackReleased();

	//特殊键
	UFUNCTION(BlueprintCallable)
	void SpecialPresssed();
	UFUNCTION(BlueprintCallable)
	void SpecialReleased();

	//扳机键
	UFUNCTION(BlueprintCallable)
	void TriggerPresssed();
	UFUNCTION(BlueprintCallable)
	void TriggerReleased();

	//跳跃键
	UFUNCTION(BlueprintCallable)
	void JumpPressed();
	UFUNCTION(BlueprintCallable)
	void JumpReleased();

	//添加攻击输入
	UFUNCTION(BlueprintCallable)
	void AddAttackInput();


	//Tick函数
	virtual void Tick(float DeltaTime) override;

	//调整方向
	UFUNCTION(BlueprintCallable)
	void UpdateDirection();

	//调整状态
	UFUNCTION()
	void UpdateState();

	//调整动画
	UFUNCTION()
	void UpdateAnimation();

	//攻击
	UFUNCTION()
	void Attack();

	//攻击恢复
	UFUNCTION()
	void AttackRestore();

};
