// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class ACT_2D_API AttackingFunction
{
public:
	AttackingFunction();

	//攻击前执行
	UFUNCTION()
	virtual void BeforeAttack() {};

	//攻击命中时自身效果
	UFUNCTION()
	virtual void SelfEffect() {};

	//攻击命中时敌方效果
	UFUNCTION()
	virtual void EnemyEffect() {};

	//攻击结束后执行
	UFUNCTION()
	virtual void AfterAttack() {};

};
