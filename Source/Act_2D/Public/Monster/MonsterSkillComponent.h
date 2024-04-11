// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperSpriteComponent.h"
#include "Skill/Monster/MonsterSkill.h"
#include "MonsterSkillComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkillBegin);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkillJudgeBegin);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkillJudgeEnd);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkillEnd);

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACT_2D_API UMonsterSkillComponent : public UPaperSpriteComponent
{
	GENERATED_BODY()

public:	

	//构造函数
	UMonsterSkillComponent();

	//释放技能
	UFUNCTION(BlueprintCallable)
	void UseSkill(UMonsterSkill* NewSkill);

	//重置技能
	void ResetSkill();

	//设置技能判定开始
	UFUNCTION(BlueprintCallable)
	void SetSkillJudgeBegin();

	//设置技能判断结束
	UFUNCTION(BlueprintCallable)
	void SetSkillJudgeEnd();

	//设置技能结束（持续性技能不受影响）
	UFUNCTION(BlueprintCallable)
	void SetSkillEnd();

	//强制技能结束
	UFUNCTION(BlueprintCallable)
	void ForceSkillEnd();

public:

	// 技能开始事件
	UPROPERTY(BlueprintAssignable)
	FOnSkillBegin OnSkillBegin;

	// 技能判定开始事件
	UPROPERTY(BlueprintAssignable)
	FOnSkillJudgeBegin OnSkillJudgeBegin;

	// 技能判定结束事件
	UPROPERTY(BlueprintAssignable)
	FOnSkillJudgeEnd OnSkillJudgeEnd;

	// 技能结束事件
	UPROPERTY(BlueprintAssignable)
	FOnSkillEnd OnSkillEnd;

protected:

	//技能开始时
	void SkillBegin();

	//技能判定开始时
	void SkillJudgeBegin();

	//技能判定结束时
	void SkillJudgeEnd();

	//技能结束时
	void SkillEnd();

protected:

	class AMonster* OwingMonster;

	//技能判定是否已开始
	bool bSkillJudgeBegin = false;

	//技能判定是否已结束
	bool bSkillJudgeEnd = false;

	UPROPERTY()
	UMonsterSkill* CurrentSkill;

	//技能列表
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<UMonsterSkill*> SkillList;

	//初始化技能
	void SetupSkill();

	//Tick函数
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
		
};
