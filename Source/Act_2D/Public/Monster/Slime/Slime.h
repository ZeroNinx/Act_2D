// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//自定义
#include "SlimeBTComponent.h"

//UE4
#include "CoreMinimal.h"
#include "PaperFlipbookComponent.h"
#include "Monster/Monster.h"
#include "Components/ArrowComponent.h"
#include "Slime.generated.h"

//收到攻击代理
DECLARE_DYNAMIC_DELEGATE(FOnDamagedDelegate);

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
	void Hit(FAttackProperty HitAttackProperty);

	//准备受击
	UFUNCTION(BlueprintCallable)
	void PrepareHit();
	

protected:
	
	//收到攻击时代理
	FOnDamagedDelegate OnDamagedDelegate;

	//受击动画
	UPROPERTY()
	UPaperFlipbook* HitFlipbook;

	//特效
	UPROPERTY()
	UPaperFlipbookComponent* Effect;

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

	//完成受击
	UFUNCTION()
	void FinishDamaged();
};
