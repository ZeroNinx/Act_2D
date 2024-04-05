// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

//自定义
#include "Utils/StateMachine.h"
#include "Utils/ActorInterface.h"

#define eps 1e-7//浮点数误差

//UE4
#include "CoreMinimal.h"
#include "PaperZD/Public/PaperZDCharacter.h"
#include "PaperFlipbookComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerCharacter.generated.h"

class UPlayerAttackComponent;
class UPaperZDAnimSequence;

/**
 * 玩家类
 */

UCLASS()
class ACT_2D_API APlayerCharacter : public APaperZDCharacter, public IActorInterface
{
	GENERATED_BODY()

public:
	// 构造函数
	APlayerCharacter();

	//角色方向是否朝右
	UPROPERTY(BlueprintReadOnly)
	bool bFacingRight;

	// 是否落地
	UPROPERTY(BlueprintReadOnly)
	bool IsMovingOnGround;

	//HP
	int HealthPoint = 5;

	//设置状态
	UFUNCTION(BlueprintCallable)
	void SetState(EState NewState);

	//取得状态
	UFUNCTION(BlueprintCallable)
	EState GetState();

	//覆盖播放动画
	UFUNCTION(BlueprintCallable)
	void PlayOverrideAnim(UPaperZDAnimSequence* InAnimSquence);

	//获得攻击组件
	UFUNCTION(BlueprintCallable)
	UPlayerAttackComponent* GetAttackComponent();

	//受击函数
	void Hit_Implementation(AActor* Attacker, FSkillProperty AttackProperty) override;

	// 从攻击中恢复
	UFUNCTION(BlueprintCallable)
	void RestoreFromAttack();

protected:

	//弹簧臂组件
	UPROPERTY(EditAnywhere)
	USpringArmComponent* SpringArmComponent;

	//相机组件
	UPROPERTY(EditAnywhere)
	UCameraComponent* CameraComponent;
	
	//状态机
	UStateMachine* StateMachine;

protected:

	// 游戏开始执行
	virtual void BeginPlay() override;
	
	//Tick函数
	virtual void Tick(float DeltaTime) override;

	//更新状态
	UFUNCTION(BlueprintCallable)
	void UpdateState();

	//更新朝向
	void UpdateDirection();

	// 更新跳跃状态
	void UpdateJumpState();

	// 当跳跃改变状态时
	void OnJumpStateChanged();

	// 播放受击动画
	UFUNCTION(BlueprintImplementableEvent)
	void PlayHitAnimation();

};
