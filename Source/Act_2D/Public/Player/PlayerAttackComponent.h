#pragma once
#include "CoreMinimal.h"
#include "PaperSpriteComponent.h"
#include "Utils/Types.h"
#include "Skill/Player/PlayerSkill.h"
#include "PlayerAttackComponent.generated.h"

class APlayerCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerAttackBegin);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerAttackJudgeBegin);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerAttackJudgeEnd);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerAttackEnd);

/**
 * 攻击组件类
 */
UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACT_2D_API UPlayerAttackComponent : public UPaperSpriteComponent
{
	GENERATED_BODY()

public:	
	
	//构造函数
	UPlayerAttackComponent();

	//是否正在攻击
	UFUNCTION(BlueprintCallable)
	bool IsAttacking();

	//是否可以移动
	bool IsMovable();

	//重置攻击
	void ResetAttack();

	//设置玩家攻击判定开始
	UFUNCTION(BlueprintCallable)
	void SetPlayerAttackJudgeBegin();

	//设置玩家攻击判断结束
	UFUNCTION(BlueprintCallable)
	void SetPlayerAttackJudgeEnd();

	//设置玩家攻击结束
	UFUNCTION(BlueprintCallable)
	void SetPlayerAttackEnd();

public:

	// 玩家攻击开始事件
	UPROPERTY(BlueprintAssignable)
	FOnPlayerAttackBegin OnPlayerAttackBegin;

	// 攻击判定开始事件
	UPROPERTY(BlueprintAssignable)
	FOnPlayerAttackJudgeBegin OnPlayerAttackJudgeBegin;

	// 攻击判定结束事件
	UPROPERTY(BlueprintAssignable)
	FOnPlayerAttackJudgeEnd OnPlayerAttackJudgeEnd;

	// 攻击结束事件
	UPROPERTY(BlueprintAssignable)
	FOnPlayerAttackEnd OnPlayerAttackEnd;

protected:

	//攻击开始时
	void PlayerAttackBegin();

	//攻击判定开始时
	void PlayerAttackJudgeBegin();

	//攻击判定结束时
	void PlayerAttacJudgeEnd();

	//攻击结束时
	void PlayerAttackEnd();

protected:

	//玩家指针
	APlayerCharacter* PlayerCharacter;

	//攻击判断是否已开始
	bool bPlayerAttackJudgeBegin = false;

	//攻击判断是否已结束
	bool bPlayerAttackJudgeEnd = false;

	UPROPERTY(EditDefaultsOnly, meta = (DisplayName = "基础技能配置"))
	TMap<FKeyCombination, TSubclassOf<UPlayerSkill>> BasicSkillConfig;

	UPROPERTY(EditDefaultsOnly, meta = (DisplayName = "跳跃技能配置"))
	TMap<FKeyCombination, TSubclassOf<UPlayerSkill>> JumpSkillConfig;

	//当前技能
	UPROPERTY()
	UPlayerSkill* CurrentSkill;

	//获取下一个技能
	UPlayerSkill* GetNextSkill(FKeyCombination KeyCombination);

	//攻击
	void Attack();

	//初始化攻击
	void SetupAttack();

	//Tick函数
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 获取下一个组合键
	FKeyCombination GetNextKeyCombination();

};
