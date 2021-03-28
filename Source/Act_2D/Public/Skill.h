#pragma once

//自定义
#include "Monster.h"

//UE4
#include "CoreMinimal.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Skill.generated.h"

/**
 * 攻击函数
 */

class APlayerCharacter;

UCLASS()
class ACT_2D_API USkill :public UObject
{
	GENERATED_BODY()

public:

	//构造函数
	USkill() {};

	//攻击前执行
	virtual void BeforeAttack(APlayerCharacter* Player) {};

	//攻击命中时自身效果
	virtual void InAttack(APlayerCharacter* Player, AMonster* Monster) {};

	//攻击结束后执行
	virtual void AfterAttack(APlayerCharacter* Player, AMonster* Monster) {};
};


//AttackIII
UCLASS()
class ACT_2D_API US_AttackIII:public USkill
{
	GENERATED_BODY()

public:

	void BeforeAttack(APlayerCharacter* Player) override;

};
