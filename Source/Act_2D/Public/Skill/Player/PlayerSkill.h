#pragma once

//自定义
#include "Utils/ActorInterface.h"

//UE4
#include "CoreMinimal.h"
#include "PaperSprite.h"
#include "Utils/Types.h"
#include "AnimSequences/PaperZDAnimSequence.h"
#include "PlayerSkill.generated.h"

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
	
	//攻击属性
	UPROPERTY(EditDefaultsOnly, meta = (DisplayName = "技能属性"))
	FSkillProperty SkillProperty;

	UPROPERTY(EditDefaultsOnly, meta = (DisplayName = "技能覆盖动画"))
	UPaperZDAnimSequence* AttackOverrideSequence;
	
	UPROPERTY(EditDefaultsOnly, meta = (DisplayName = "技能判定Sprite"))
	UPaperSprite* AttackJudgeSprite;

	UPROPERTY(EditDefaultsOnly, meta = (DisplayName = "技能衔接配置"))
	TMap<FKeyCombination, TSubclassOf<UPlayerSkill>> ComboConfig;

	//执行命中判定
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ExecuteHit(AActor* HitActor);

	//获取击中的Actor
	UFUNCTION(BlueprintCallable)
	TArray<AActor*> GetHitActors();

	//技能开始时
	UFUNCTION(BlueprintNativeEvent)
	void OnSkillBegin();

	//前摇每帧执行
	UFUNCTION(BlueprintNativeEvent)
	void TickBeforeSkillJudge();

	//技能判定开始时
	UFUNCTION(BlueprintNativeEvent)
	void OnSkillJudgeBegin();

	//技能判定时每帧执行
	UFUNCTION(BlueprintNativeEvent)
	void TickOnSkillJudge();

	//技能判定结束时
	UFUNCTION(BlueprintNativeEvent)
	void OnSkillJudgeEnd();

	//后摇每帧执行
	UFUNCTION(BlueprintNativeEvent)
	void TickAfterSkillJudge();

	//技能结束时
	UFUNCTION(BlueprintNativeEvent)
	void OnSkillEnd();
};

