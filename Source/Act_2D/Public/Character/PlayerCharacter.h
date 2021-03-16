// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerStateMachine.h"
#include "PlayerInputComponent.h"
#include "PlayerAttackComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PlayerCharacter.generated.h"

class UPlayerInputComponent;

/**
 * 玩家类
 */

UCLASS()
class ACT_2D_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// 构造函数
	APlayerCharacter();

	//角色移动
	UFUNCTION()
		void MoveRight(float AxisValue);

	//按下攻击键
	UFUNCTION(BlueprintCallable)
		void AttackPresssed();

	//从攻击中恢复
	UFUNCTION()
		void AttackRestore();

	//按下跳跃
	UFUNCTION(BlueprintCallable)
		void JumpPressed();

protected:

	//角色动画
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	UPaperFlipbookComponent* Flipbook;

	//输入组件
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	UPlayerInputComponent* MyPlayerInputComponent;
	
	//状态机
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	UPlayerStateMachine* StateMachine;

	//攻击组件
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	UPlayerAttackComponent* AttackComponent;

	//角色方向是否朝右
	UPROPERTY(BlueprintReadOnly)
	bool bFacingRight;

	// 游戏开始执行
	virtual void BeginPlay() override;

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

	//输入绑定
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
