#pragma once

//自定义
#include "Monster.h"

//UE4
#include "CoreMinimal.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Skill.generated.h"

class APlayerCharacter;

/**
 * 攻击函数
 */

UCLASS()
class ACT_2D_API USkill :public UObject
{
	GENERATED_BODY()

public:

	//构造函数
	USkill() {};

	//攻击全程
	virtual void InAttack(APlayerCharacter* Player) {};

	//攻击前执行
	virtual void BeforeJudge(APlayerCharacter* Player) {};

	//攻击命中时自身效果
	virtual void InJudge(APlayerCharacter* Player, AMonster* Monster) {};
};


//AttackIII
UCLASS()
class ACT_2D_API US_AttackIII:public USkill
{
	GENERATED_BODY()

public:

	US_AttackIII();

	float SpeedScale = 1.0f;

	void InAttack(APlayerCharacter* Player) override;

};
