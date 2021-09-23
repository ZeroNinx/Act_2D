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
	UPROPERTY(BlueprintReadOnly)
	int AttackID;

	//初始化参数
	UFUNCTION(BlueprintCallable)
	void Setup(APlayerCharacter* NewCharacter);

	//是否可以移动
	UFUNCTION(BlueprintCallable)
	bool IsMovable();

	//接收攻击键组合
	UFUNCTION(BlueprintCallable)
	void SetKeyCombination(FKeyCombination KeyCombation);

	//重置攻击
	UFUNCTION(BlueprintCallable)
	void ResetAttack();

	//玩家结束攻击
	UFUNCTION(BlueprintCallable)
	void PlayerFinishAttack();

protected:

	//玩家指针
	UPROPERTY(BlueprintReadOnly)
	APlayerCharacter* PlayerCharacter;

	//当前动作攻击帧
	UPROPERTY(BlueprintReadOnly)
	int AttackFrame;
	
	//可移动帧
	UPROPERTY(BlueprintReadOnly)
	int MovableFrame; 

	//跳跃攻击标记
	UPROPERTY(BlueprintReadOnly)
	bool bJumpingAttack;

	//攻击判定标记
	UPROPERTY(BlueprintReadOnly)
	bool bShouldJudge;
	
	//攻击停顿标记
	UPROPERTY(BlueprintReadOnly)
	bool bThreadSleeped = false;

	//当前技能
	UPROPERTY(BlueprintReadOnly)
	USkill* Skill;

	//下一次按键组合
	UPROPERTY(BlueprintReadOnly)
	FKeyCombination NextKeyCombation;

	//连续技列表
	UPROPERTY(BlueprintReadOnly)
	TMap<int, int> ComboMap;

	//载入数据库
	sqlite3* LoadDB();

	//攻击
	UFUNCTION(BlueprintCallable)
	void Attack(int ID);

	//选择攻击
	UFUNCTION(BlueprintCallable)
	void SwitchAttack();

	//初始化攻击
	UFUNCTION()
	void SetupAttack();

	//初始化连续技
	UFUNCTION()
	void SetupCombo();

	//攻击判定
	UFUNCTION()
	void AttackJudge();

	//获得当前攻击动画播放位置
	UFUNCTION(BlueprintCallable)
	int GetAnimationPosition();

	//Tick函数
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};
