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

	//史莱姆跳跃（攻击）
	void JumpAttack();

	//被击中
	virtual void OnHit(AActor* Attacker, FSkillProperty HitAttackProperty) override;

protected:

	//攻击组件重叠时
	UFUNCTION()
	void OnAttackComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:

	//攻击组件
	UPROPERTY(EditDefaultsOnly)
	UPaperSpriteComponent* AttackCompnent;

	//攻击技能
	UMonsterSkill* Skill;

	//跳跃攻击定时器句柄
	FTimerHandle JumpAttackHandle;

	//跳跃准备时间
	float JumpReadyTime = 0.05f;

	//标记是否刚着地
	bool bFalled = false;

	//攻击判定
	void AttackJudge(AActor* HitActor);

};
