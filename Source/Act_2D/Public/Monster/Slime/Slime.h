// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//自定义
#include "Monster.h"

//UE4
#include "CoreMinimal.h"
#include "PaperSprite.h"
#include "PaperSpriteComponent.h"
#include "PaperFlipbookComponent.h"
#include "Kismet/GameplayStatics.h"
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
	UFUNCTION()
	void JumpAttack();

	//被击中
	UFUNCTION()
	void Hit(FAttackProperty HitAttackProperty);

protected:

	//攻击组件
	UPROPERTY()
	UPaperSpriteComponent* AttackCompnent;

	//攻击技能
	UPROPERTY()
	UMonsterSkill* Skill;

	//跳跃攻击定时器句柄
	FTimerHandle JumpAttackHandle;

	//跳跃准备时间
	float JumpReadyTime = 0.05f;

	//标记是否刚着地
	bool bFalled = false;

	//初始化动画资源
	UFUNCTION()
	void InitAnimation() override;

	//tick函数
	void Tick(float DeltaTime) override;

	//更新方向
	void UpdateDirection() override;

	//更新状态
	void UpdateState() override;

	//更新动画
	UFUNCTION()
	void UpdateAnimation() override;

	//攻击组件重叠时
	UFUNCTION()
	void OnAttackComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//攻击判定
	UFUNCTION()
	void AttackJudge(APlayerCharacter* Player);

	//单帧动画完成时
	UFUNCTION()
	void OnFlipookFinishedPlaying() override;
};
