#pragma once

//自定义
#include "Player/PlayerCharacter.h"

//UE4
#include "CoreMinimal.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MonsterSkill.generated.h"

class APlayerCharacter;
class AMonster;

/**
 * 怪物技能类
 */
UCLASS()
class ACT_2D_API UMonsterSkill :public UObject
{
	GENERATED_BODY()

public:

	//构造函数
	UMonsterSkill() {};

	//攻击属性
	UPROPERTY(BlueprintReadOnly)
	FSkillProperty AttackProperty;

	//攻击全程执行
	virtual void InAttack(AMonster* Monster) {};

	//攻击前执行
	virtual void BeforeJudge(AMonster* Monster) {};

	//攻击命中时自身效果
	virtual void InJudge(AMonster* Monster, APlayerCharacter* Player);

};

/**
 * 史莱姆攻击
 */
UCLASS()
class ACT_2D_API UMS_SlimeAttack :public UMonsterSkill
{
	GENERATED_BODY()

public:

	UMS_SlimeAttack();

};
