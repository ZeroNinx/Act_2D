// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//自定义
#include "Act_2DTypes.h"
#include "PlayerCharacter.h"

//UE4
#include "CoreMinimal.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/PlayerController.h"
#include "PlayerCharacterController.generated.h"

/**
 * 玩家控制器类
 */

 //攻击完成代理
DECLARE_DYNAMIC_DELEGATE(FOnAttackFinishedDelegate);

UCLASS()
class ACT_2D_API APlayerCharacterController : public APlayerController
{
	GENERATED_BODY()
	
public:

	//构造函数
	APlayerCharacterController();

	//初始化控制器
	UFUNCTION(BlueprintCallable)
	void Setup();

protected:

	//玩家指针
	UPROPERTY(BlueprintReadOnly,EditAnywhere)
	APlayerCharacter* PlayerCharacter;

	//攻击组件指针
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UPlayerAttackComponent* AttackComponent;

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

	//初始化输入函数
	void SetupInputComponent() override;


	//攻击键是否按下
	UPROPERTY(BlueprintReadOnly)
	bool bAttackPressed;

	//特殊键是否按下
	UPROPERTY(BlueprintReadOnly)
	bool bSpecialPressed;

	//扳机键是否按下
	UPROPERTY(BlueprintReadOnly)
	bool bTriggerPressed;

	//跳跃键是否按下
	UPROPERTY(BlueprintReadOnly)
	bool bJumpPressed;

	//上方向键是否按下
	UPROPERTY(BlueprintReadOnly)
	bool bUpPressed;

	//下方向键是否按下
	UPROPERTY(BlueprintReadOnly)
	bool bDownPressed;

	//左方向键是否按下
	UPROPERTY(BlueprintReadOnly)
	bool bLeftPressed;

	//右方向键是否按下
	UPROPERTY(BlueprintReadOnly)
	bool bRightPressed;

	//输入组合
	UPROPERTY(BlueprintReadOnly)
	FKeyCombination NextkKeyCombation;

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

	//记录输入组合
	UFUNCTION(BlueprintCallable)
	void RecordKeyCombination();

	//添加攻击输入
	UFUNCTION(BlueprintCallable)
	void AddAttackInput();

	//攻击恢复
	UFUNCTION()
	void AttackRestore();

};
