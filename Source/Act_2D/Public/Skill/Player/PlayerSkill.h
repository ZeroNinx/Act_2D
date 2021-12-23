#pragma once

//自定义
#include "ActorInterface.h"

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

	//Player
	UPROPERTY(BlueprintReadOnly)
	APlayerCharacter* PlayerCharacter;

	//击中的Actor
	UPROPERTY(BlueprintReadOnly)
	TSet<AActor*> HitActors;

	//击中的Actor
	UPROPERTY(BlueprintReadOnly)
	TSet<AActor*> ActorsAppliedHit;


	//攻击属性
	UPROPERTY(BlueprintReadOnly)
	FAttackProperty AttackProperty;

	//检测击中的Actor
	virtual void ScanHitActors();

	//攻击判定
	virtual void ExecuteAttackJudge(APlayerCharacter* Player, AActor* AMonster);

	//前摇执行一次
	virtual void JudgeAtBeginAttack(APlayerCharacter* Player) {};

	//攻击开始执行一次
	virtual void JudgeAtStartAttack(APlayerCharacter* Player);

	//攻击结束时执行一次
	virtual void JudgeAtFinishAttack(APlayerCharacter* Player) {};

	//前摇每帧执行
	virtual void JudgeDuringBeforeAttack(APlayerCharacter* Player) {};

	//攻击时每帧执行
	virtual void JudgeDuringAttack(APlayerCharacter* Player) {};

	//攻击结束时执行一次
	virtual void JudgeDuringFinishAttack(APlayerCharacter* Player) {};


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
	
	void JudgeAtStartAttack(APlayerCharacter* Player) override;

	void JudgeDuringAttack(APlayerCharacter* Player) override;
	
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

	void JudgeAtStartAttack(APlayerCharacter* Player) override;

	void JudgeDuringAttack(APlayerCharacter* Player) override;
};
