#pragma once

//自定义
#include "Monster.h"

//UE4
#include "CoreMinimal.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PlayerSkill.generated.h"

class APlayerCharacter;
class AMonster;

/**
 * 技能类
 */
UCLASS()
class ACT_2D_API USkill :public UObject
{
	GENERATED_BODY()

public:

	//构造函数
	USkill() {};

	//攻击属性
	UPROPERTY(BlueprintReadOnly)
	FAttackProperty AttackProperty;

	//攻击全程执行
	virtual void InAttack(APlayerCharacter* Player) {};

	//攻击前执行
	virtual void BeforeJudge(APlayerCharacter* Player) {};

	//攻击命中时自身效果
	virtual void InJudge(APlayerCharacter* Player, AMonster* Monster);


protected:

	bool bShouldJudge = true;
};

/**
 * AttackI
 */
UCLASS()
class ACT_2D_API US_AttackI :public USkill
{
	GENERATED_BODY()

public:

	US_AttackI();

};

/**
 * AttackII
 */
UCLASS()
class ACT_2D_API US_AttackII :public USkill
{
	GENERATED_BODY()

public:

	US_AttackII();

};


/**
 * AttackIII
 */
UCLASS()
class ACT_2D_API US_AttackIII:public USkill
{
	GENERATED_BODY()

public:

	US_AttackIII();
	

	//攻击前
	void BeforeJudge(APlayerCharacter* Player) override;
	
};


/**
 * JumpAttack
 */
UCLASS()
class ACT_2D_API US_AttackJump :public USkill
{
	GENERATED_BODY()

public:

	US_AttackJump();

};


/**
 * DashJump
 */
UCLASS()
class ACT_2D_API US_AttackDash :public USkill
{
	GENERATED_BODY()

public:

	US_AttackDash();

	//攻击前
	void BeforeJudge(APlayerCharacter* Player) override;
};
