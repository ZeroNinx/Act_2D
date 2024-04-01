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
 * 玩家技能类
 */
UCLASS(BlueprintType, Blueprintable)
class ACT_2D_API UPlayerSkill :public UObject
{
	GENERATED_BODY()

public:

	//构造函数
	UPlayerSkill() {};

	//击中的Actor
	UPROPERTY(BlueprintReadOnly)
	TSet<AActor*> ActorsAppliedHit;

	//攻击属性
	UPROPERTY(BlueprintReadOnly)
	FAttackProperty AttackProperty;

	UFUNCTION(BlueprintCallable)
	APlayerCharacter* GetPlayerCharacter();

	//获取击中的Actor
	virtual TArray<AActor*> GetHitActors();

	//攻击判定
	virtual void ExecuteAttackJudge(APlayerCharacter* Player, AActor* AMonster);

	//前摇执行一次
	virtual void OnAttackBegin() {};

	//前摇每帧执行
	virtual void TickBeforeAttackJudge() {};

	//攻击开始执行一次
	virtual void OnAttackJudgeBegin();

	//攻击时每帧执行
	virtual void TickOnAttackJudge() {};

	//攻击结束时执行一次
	virtual void OnAttackJudgeEnd() {};

	//后摇每帧执行
	virtual void TickAfterAttackJudge() {};

};

/**
 * AttackI
 */
UCLASS()
class ACT_2D_API US_AttackI :public UPlayerSkill
{
	GENERATED_BODY()

public:

	US_AttackI();

};

/**
 * AttackII
 */
UCLASS()
class ACT_2D_API US_AttackII :public UPlayerSkill
{
	GENERATED_BODY()

public:

	US_AttackII();

};


/**
 * AttackIII
 */
UCLASS()
class ACT_2D_API US_AttackIII:public UPlayerSkill
{
	GENERATED_BODY()

public:

	US_AttackIII();
	
	void OnAttackJudgeBegin() override;

	void TickOnAttackJudge() override;
	
};


/**
 * JumpAttack
 */
UCLASS()
class ACT_2D_API US_AttackJump :public UPlayerSkill
{
	GENERATED_BODY()

public:

	US_AttackJump();

};


/**
 * DashJump
 */
UCLASS()
class ACT_2D_API US_AttackDash :public UPlayerSkill
{
	GENERATED_BODY()

public:

	US_AttackDash();

	void OnAttackJudgeBegin() override;

	void TickOnAttackJudge() override;
};
