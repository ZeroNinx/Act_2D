// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//自定义
#include "Utils/ActorInterface.h"
#include "Utils/StateMachine.h"
#include "Skill/Monster/MonsterSkill.h"

//UE4
#include "CoreMinimal.h"
#include "PaperFlipbook.h"
#include "PaperZD/Public/PaperZDCharacter.h"
#include "PaperFlipbookComponent.h"
#include "HAL/PlatformProcess.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Monster.generated.h"


/**
 *	怪物类
 */
UCLASS()
class ACT_2D_API AMonster : public APaperZDCharacter,public IActorInterface
{
	GENERATED_BODY()

public:
	//构造函数
	AMonster();
	
	//获取是否存活
	UFUNCTION(BlueprintCallable)
	bool IsAlive();

	//获取当前状态
	UFUNCTION(BlueprintCallable)
	EState GetState();

	//设置当前状态
	UFUNCTION(BlueprintCallable)
	void SetState(EState State);

	//获取HP
	UFUNCTION(BlueprintCallable)
	int GetHealthPoint();

	//受击动画播放完成时
	UFUNCTION(BlueprintCallable)
	virtual void OnHitAnimationPlayComplete();

	//是否朝右
	bool bFacingRight;

protected:

	// 受击时
	UFUNCTION(BlueprintCallable)
	virtual void OnHit(AActor* Attacker, FSkillProperty HitAttackProperty);

	// 死亡时
	UFUNCTION(BlueprintCallable)
	virtual void OnDead();

protected:

	//碰撞胶囊
	UPROPERTY(EditAnywhere)
	UCapsuleComponent* RealCapsule;

	//状态机
	UStateMachine* StateMachine;

	//攻击特性
	UPROPERTY(BlueprintReadOnly)
	FSkillProperty AttackProperty;

	//受击效果组件
	UPROPERTY(BlueprintReadOnly)
	UPaperFlipbookComponent* HitEffectComponent;

	//HP
	UPROPERTY(EditAnywhere)
	int HealthPoint = 5;

	//死亡闪烁计数器
	int DeathFlashCounter = 0;

	//死亡效果定时器句柄
	FTimerHandle DeathEffectTimerHandle;

protected:

	//游戏开始执行
	virtual void BeginPlay() override;

	//每帧执行
	virtual void Tick(float DeltaTime) override;

	//更新朝向
	virtual void UpdateFacingDirection();

	//更新状态
	virtual void UpdateState();

	//受击函数
	virtual void Hit_Implementation(AActor* Attacker, FSkillProperty HitAttackProperty) override;
	
	//播放死亡动画
	virtual void PlayDeathEffect();

	//受击特效
	UPaperFlipbook* HitEffectFlipbook;

};
