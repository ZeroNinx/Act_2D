// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//自定义


//UE4
#include "CoreMinimal.h"
#include "Monster/Monster.h"
#include "Components/ArrowComponent.h"
#include "RedGrass.generated.h"

/**
 * 红草
 */
UCLASS()
class ACT_2D_API ARedGrass : public AMonster
{
	GENERATED_BODY()
	
public:

	//构造函数
	ARedGrass();

	//攻击
	UFUNCTION()
	void Attack();

	//受击函数
	void Hit_Implementation(AActor* Attacker, FAttackProperty HitAttackProperty) override;

protected:

	//每帧执行
	void Tick(float DeltaTime) override;

	//初始化动画
	void InitAnimation();

	//更新动画
	void UpdateAnimation();

	//单帧动画播放完成时
	UFUNCTION()
	void OnFlipookFinishedPlaying();

};
