// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//自定义
#include "ActorInterface.h"
#include "StateMachine.h"
#include "MonsterSkill.h"

//UE4
#include "CoreMinimal.h"
#include "PaperFlipbook.h"
#include "PaperCharacter.h"
#include "PaperFlipbookComponent.h"
#include "HAL/PlatformProcess.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Monster.generated.h"


/**
 *	怪物类
 */
UCLASS()
class ACT_2D_API AMonster : public APaperCharacter ,public IActorInterface
{
	GENERATED_BODY()

public:
	//构造函数
	AMonster();
	
	//获取HP
	UFUNCTION()
	int& GetHealthPoint();

	//是否朝右
	bool bFacingRight;

protected:

	//碰撞胶囊
	UPROPERTY(BlueprintReadOnly)
	UCapsuleComponent* RealCapsule;

	//状态机
	UPROPERTY(BlueprintReadOnly)
	UStateMachine* StateMachine;

	//攻击特性
	UPROPERTY(BlueprintReadOnly)
	FAttackProperty AttackProperty;

	//受击效果组件
	UPROPERTY(BlueprintReadOnly)
	UPaperFlipbookComponent* HitEffectComponent;

	//HP
	int HealthPoint = 5;

	//死亡闪烁计数器
	int DeathFlashCounter = 0;

	//死亡效果定时器句柄
	FTimerHandle DeathEffectTimerHandle;

	//游戏开始执行
	virtual void BeginPlay() override;

	//每帧执行
	virtual void Tick(float DeltaTime) override;

	//更新朝向
	virtual void UpdateFacingDirection();

	//更新状态
	virtual void UpdateState();

	//更新动画
	virtual void UpdateAnimation() {};

	//标记一次攻击是否命中了玩家
	bool bAttacked = false;

	//单帧动画播放完成时
	virtual void OnFlipookFinishedPlaying() {};


	/**
	* 动画资源区
	*/

	//空闲动画
	UPROPERTY();
	UPaperFlipbook* IdleFlipbook;

	//跳跃动画
	UPROPERTY()
	UPaperFlipbook* JumpingFlipbook;

	//着地动画
	UPROPERTY()
	UPaperFlipbook* FalledFlipbook;

	//受击动画
	UPROPERTY()
	UPaperFlipbook* HitFlipbook;

	//受击特效
	UPROPERTY()
	UPaperFlipbook* HitEffectFlipbook;

	//初始化动画资源
	virtual void InitAnimation() {};

};
