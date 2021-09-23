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

	//当开始控制时
	UFUNCTION(BlueprintCallable)
	void OnPossess(APawn* InPawn) override;

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
	float AttackInputDuration = 0.03f;

	//是否等待延迟输入
	UPROPERTY(BlueprintReadOnly)
	bool bWaitingInput;

	//初始化输入函数
	void SetupInputComponent() override;

	//Tick
	void Tick(float DeltaSeconds) override;


	//攻击键是否按下
	UPROPERTY(BlueprintReadOnly)
	bool bAttackPressed = false;

	//特殊键是否按下
	UPROPERTY(BlueprintReadOnly)
	bool bSpecialPressed = false;

	//扳机键是否按下
	UPROPERTY(BlueprintReadOnly)
	bool bTriggerPressed = false;

	//跳跃键是否按下
	UPROPERTY(BlueprintReadOnly)
	bool bJumpPressed = false;

	//上方向键是否按下
	UPROPERTY(BlueprintReadOnly)
	bool bUpPressed = false;

	//下方向键是否按下
	UPROPERTY(BlueprintReadOnly)
	bool bDownPressed = false;

	//左方向键是否按下
	UPROPERTY(BlueprintReadOnly)
	bool bLeftPressed = false;

	//右方向键是否按下
	UPROPERTY(BlueprintReadOnly)
	bool bRightPressed = false;

	//输入组合
	UPROPERTY(BlueprintReadOnly)
	FKeyCombination NextkKeyCombation;

	//上键
	UFUNCTION()
	void UpPressed();
	UFUNCTION()
	void UpReleased();

	//下键
	UFUNCTION()
	void DownPressed();
	UFUNCTION()
	void DownReleased();

	//左键
	UFUNCTION()
	void LeftPressed();
	UFUNCTION()
	void LeftReleased();

	//右键
	UFUNCTION()
	void RightPressed();
	UFUNCTION()
	void RightReleased();

	//攻击键
	UFUNCTION(BlueprintCallable)
	void AttackPressed();
	UFUNCTION(BlueprintCallable)
	void AttackReleased();

	//特殊键
	UFUNCTION(BlueprintCallable)
	void SpecialPressed();
	UFUNCTION(BlueprintCallable)
	void SpecialReleased();

	//扳机键
	UFUNCTION(BlueprintCallable)
	void TriggerPressed();
	UFUNCTION(BlueprintCallable)
	void TriggerReleased();

	//跳跃键
	UFUNCTION(BlueprintCallable)
	void JumpPressed();
	UFUNCTION(BlueprintCallable)
	void JumpReleased();

	//是否允许移动
	bool IsAllowMove();

	//记录输入组合
	UFUNCTION(BlueprintCallable)
	void RecordKeyCombination();

	//准备攻击
	UFUNCTION(BlueprintCallable)
	void PrepareAttack();

	//添加攻击输入
	UFUNCTION(BlueprintCallable)
	void AddAttackInput();

};
