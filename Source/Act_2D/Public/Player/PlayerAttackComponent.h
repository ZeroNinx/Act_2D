#pragma once

//自定义
#include "Skill/Player/PlayerSkill.h"
#include "Utils/Types.h"

//UE4
#include "Misc/Paths.h"
#include "CoreMinimal.h"
#include "PaperSprite.h"
#include "PaperSpriteComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/ActorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerAttackComponent.generated.h"

//互相包含
class AMonster;
class UStateMachine;
class APlayerCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerAttackBegin);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerAttackJudgeBegin);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerAttackJudgeEnd);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerAttackEnd);

/**
 * 攻击组件类
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class ACT_2D_API UPlayerAttackComponent : public UPaperSpriteComponent
{
	GENERATED_BODY()

public:	
	
	//构造函数
	UPlayerAttackComponent();

	//当前攻击ID
	UPROPERTY(BlueprintReadWrite)
	int AttackID = 0;

	//初始化参数
	UFUNCTION(BlueprintCallable)
	void Setup(APlayerCharacter* NewCharacter);

	UFUNCTION(BlueprintCallable)
	bool IsAttacking();

	//是否可以移动
	bool IsMovable();

	//接收攻击键组合
	void SetKeyCombination(FKeyCombination KeyCombation);

	//重置攻击
	void ResetAttack();

	//设置玩家攻击判定开始
	UFUNCTION(BlueprintCallable)
	void SetPlayerAttackJudgeBegin();

	//设置玩家攻击判断结束
	UFUNCTION(BlueprintCallable)
	void SetPlayerAttackJudgeEnd();

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

	//玩家指针
	APlayerCharacter* PlayerCharacter;

	//攻击判断是否已开始
	bool bPlayerAttackJudgeBegin = false;

	//攻击判断是否已结束
	bool bPlayerAttackJudgeEnd = false;

	UPROPERTY(EditDefaultsOnly, meta = (DisplayName = "基础技能配置"))
	TMap<FKeyCombination, TSubclassOf<UPlayerSkill>> BasicSkillConfig;

	//当前技能
	UPROPERTY()
	UPlayerSkill* Skill;

	//下一次按键组合
	FKeyCombination NextKeyCombation;

	//连续技列表
	UPROPERTY(BlueprintReadWrite)
	TMap<int, int> ComboMap;

	//攻击
	void Attack(int ID);

	//初始化攻击
	void SetupAttack();

	UFUNCTION(BlueprintImplementableEvent)
	class UPaperZDAnimSequence* GetAttackSequence();

	UFUNCTION(BlueprintImplementableEvent)
	class UPaperSprite* GetAttackSprite();

	//攻击开始时
	void PlayerAttackBegin();

	//攻击判定开始时
	void PlayerAttackJudgeBegin();

	//攻击判定结束时
	void PlayerAttacJudgeEnd();

	//攻击结束时
	void PlayerAttackEnd();

	//Tick函数
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 获取下一个组合键
	FKeyCombination GetNextKeyCombination();

};
