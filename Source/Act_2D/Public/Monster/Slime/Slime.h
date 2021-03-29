// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//自定义
#include "SlimeBTComponent.h"

//UE4
#include "CoreMinimal.h"
#include "PaperFlipbookActor.h"
#include "Monster/Monster.h"
#include "Slime.generated.h"

//收到攻击代理
DECLARE_DYNAMIC_DELEGATE(FOnDamagedDelegate);

//特效播放完成代理
DECLARE_DYNAMIC_DELEGATE(FOnEffectPlayFinishedDelegate);

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

	//被击中
	UFUNCTION(BlueprintCallable)
	void Hit(FAttackProperty AttackProperty);

protected:
	
	bool bThreadSleeped = false;
	bool bMoved = false;
	float SpeedScale = 1.0f;

	//特效播放完成代理
	FOnEffectPlayFinishedDelegate OnEffectPlayFinishedDelegate;
	
	//收到攻击时代理
	FOnDamagedDelegate OnDamagedDelegate;

	//特效
	APaperFlipbookActor* Effect;

	//tick函数
	void Tick(float DeltaTime) override;

	//更新状态
	UFUNCTION()
	void UpdateState();

	//更新动画
	UFUNCTION()
	void UpdateAnimation();

	//结束播放
	UFUNCTION()
	void RemoveEffect();

	//结束播放
	UFUNCTION()
	void MoveBack();

	//完成受击
	UFUNCTION()
	void FinishDamaged();
};
