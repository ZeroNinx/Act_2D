// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//自定义
#include "Monster/Monster.h"

//UE4
#include "CoreMinimal.h"
#include "PaperSprite.h"
#include "PaperSpriteComponent.h"
#include "PaperFlipbookComponent.h"
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

	UFUNCTION(BlueprintCallable)
	void OnJumpReadyAmimEnd();

	//被击中
	virtual void OnHit(AActor* Attacker, FSkillProperty HitAttackProperty) override;

protected:

	// 获取攻击技能
	UFUNCTION(BlueprintImplementableEvent)
	UMonsterSkill* GetAttackSkill();

	// 进入攻击状态
	void EnterAttackState();

	// 跳跃状态改变时
	virtual void OnJumpStateChanged() override;

	//攻击函数
	virtual void Attack_Implementation() override;

};
