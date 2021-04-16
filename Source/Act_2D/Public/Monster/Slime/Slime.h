// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//UE4
#include "CoreMinimal.h"
#include "PaperSprite.h"
#include "PaperSpriteComponent.h"
#include "PaperFlipbookComponent.h"
#include "Monster/Monster.h"
#include "Components/ArrowComponent.h"
#include "Slime.generated.h"

/**
 * 史莱姆
 */
UCLASS()
class ACT_2D_API ASlime : public AMonster
{
	GENERATED_BODY()
	
public:

	//构造函数
	ASlime();

	//史莱姆跳跃（攻击）
	UFUNCTION(BlueprintCallable)
	void JumpAttack();

	//被击中
	UFUNCTION(BlueprintCallable)
	void Hit(FAttackProperty HitAttackProperty);

	//准备受击
	UFUNCTION(BlueprintCallable)
	void PrepareHit();
	

protected:

	//攻击组件
	UPROPERTY()
	UPaperSpriteComponent* AttackCompnent;

	//受击动画
	UPROPERTY()
	UPaperFlipbook* HitFlipbook;

	//受击特效
	UPROPERTY()
	UPaperFlipbookComponent* Effect;

	//攻击
	UPROPERTY()
	UMonsterSkill* Skill;

	//跳跃攻击定时器句柄
	FTimerHandle JumpAttackHandle;

	//跳跃准备时间
	float JumpReadyTime = 0.05f;

	//攻击标记
	bool bAttacked = false;

	//下落标记
	bool bFalled = false;

	//tick函数
	void Tick(float DeltaTime) override;

	//更新方向
	void UpdateDirection() override;

	//更新状态
	UFUNCTION()
	void UpdateState();

	//更新动画
	UFUNCTION()
	void UpdateAnimation();

	//攻击组件重叠时
	UFUNCTION()
	void OnAttackComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//单帧动画完成时
	UFUNCTION()
	void OnFlipookFinishedPlaying();
};
