#pragma once

//第三方
#include "sqlite3.h"

//自定义
#include "PlayerSkill.h"
#include "Act_2DTypes.h"

//UE4
#include "Misc/Paths.h"
#include "CoreMinimal.h"
#include "PaperSprite.h"
#include "PaPerFlipbook.h"
#include "PaperSpriteComponent.h"
#include "PaperFlipbookComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/ActorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerAttackComponent.generated.h"

//互相包含
class AMonster;
class UStateMachine;
class APlayerCharacter;

/**
 * 攻击组件类
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACT_2D_API UPlayerAttackComponent : public UPaperSpriteComponent
{
	GENERATED_BODY()

public:	
	
	//构造函数
	UPlayerAttackComponent();

	//当前攻击ID
	int AttackID = 0;

	//初始化参数
	void Setup(APlayerCharacter* NewCharacter);

	//是否可以移动
	bool IsMovable();

	//接收攻击键组合
	void SetKeyCombination(FKeyCombination KeyCombation);

	//重置攻击
	void ResetAttack();

protected:

	//玩家指针
	APlayerCharacter* PlayerCharacter;

	//当前动作攻击开始帧
	int AttackStartFrame = 0;
	
	//当前动作攻击结束帧
	int AttackFinishFrame = 0;

	//攻击前判定标记
	bool bBeforeAttackShouldJudge = true;

	//攻击中判定标记
	bool bInAttackShouldJudge = true;

	//攻击后判定标记
	bool bAfterAttackShouldJudge = true;

	//当前技能
	UPROPERTY()
	USkill* Skill;

	//下一次按键组合
	FKeyCombination NextKeyCombation;

	//连续技列表
	TMap<int, int> ComboMap;

	//载入数据库
	sqlite3* LoadDB();

	//攻击
	void Attack(int ID);

	//选择攻击
	void SwitchAttack();

	//初始化攻击
	void SetupAttack();

	//初始化连续技
	void SetupCombo();

	//获得当前攻击动画播放位置
	int GetAnimationPosition();

	//Tick函数
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};
