#pragma once

//第三方
#include "sqlite3.h"

//自定义
#include "PlayerStateMachine.h"
#define Combo1 0x001
#define Combo2 0x010
#define Combo3 0x100
#define Combo4 0x101
#define Combo5 0x110
#define Combo6 0x011
#define Combo7 0x111

//UE4
#include "Misc/Paths.h"
#include "CoreMinimal.h"
#include "PaperSprite.h"
#include "PaPerFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "PaperSpriteComponent.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PlayerAttackComponent.generated.h"

//组合键结构体
USTRUCT(BlueprintType)
struct FKeyCombination
{
	GENERATED_BODY()

	bool AttackKey=false;
	bool SpecialKey = false;
	bool TriggerKey = false;
	bool JumpKey = false;
	bool UpKey=false;
	bool DownKey=false;
	bool LeftKey=false;
	bool RighKey=false;

	//构造函数
	FKeyCombination() {};
	FKeyCombination
	(
		bool isAttackPressed,
		bool isSpecialPressed,
		bool isTriggerPressed,
		bool isJumpPressed,
		bool isUpPressed,
		bool isDownPressed, 
		bool isLeftPressed, 
		bool isRightPressed
	)
	{
		AttackKey = isAttackPressed;
		SpecialKey = isSpecialPressed;
		TriggerKey = isTriggerPressed;
		JumpKey = isJumpPressed;
		UpKey = isUpPressed;
		DownKey = isDownPressed;
		LeftKey = isLeftPressed;
		RighKey = isRightPressed;
	}

	//判断是否为空
	bool IsAttackEmpty()
	{
		return !(AttackKey | SpecialKey | TriggerKey);
	}

	//获取哈希
	int GetHash()
	{
		return (TriggerKey << 8) + (SpecialKey << 4) + AttackKey;
	}
};

//攻击组件
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACT_2D_API UPlayerAttackComponent : public UPaperSpriteComponent
{
	GENERATED_BODY()

public:	
	
	//构造函数
	UPlayerAttackComponent();

	//攻击键是否按下
	UPROPERTY(BlueprintReadOnly)
	bool bAttackPressed;
	
	//特殊键是否按下
	UPROPERTY(BlueprintReadOnly)
	bool bSpecialPressed;
	
	//扳机键是否按下
	UPROPERTY(BlueprintReadOnly)
	bool bTriggerPressed;

	//跳跃键是否按下
	UPROPERTY(BlueprintReadOnly)
	bool bJumpPressed;

	//上方向键是否按下
	UPROPERTY(BlueprintReadOnly)
	bool bUpPressed;

	//下方向键是否按下
	UPROPERTY(BlueprintReadOnly)
	bool bDownPressed;

	//左方向键是否按下
	UPROPERTY(BlueprintReadOnly)
	bool bLeftPressed;

	//右方向键是否按下
	UPROPERTY(BlueprintReadOnly)
	bool bRightPressed;

	//初始化所有者
	UFUNCTION(BlueprintCallable)
	void Setup(UPaperFlipbookComponent *NewFlipbookComponent,UPlayerStateMachine* NewStateMachine);

	//是否可以移动
	UFUNCTION(BlueprintCallable)
	bool IsMovable();

	//返回是否接受输入
	UFUNCTION(BlueprintCallable)
	bool IsAcceptInput();

	//记录下一次攻击组合
	UFUNCTION(BlueprintCallable)
	void RecordKeyCombination();

	//攻击
	UFUNCTION(BlueprintCallable)
	void Attack(int ID = 1);

	//重置攻击
	UFUNCTION(BlueprintCallable)
	void ResetAttack();

protected:

	//动画组件
	UPROPERTY(BlueprintReadOnly)
	UPaperFlipbookComponent* FlipbookComponent;

	//状态机
	UPROPERTY(BlueprintReadOnly)
	UPlayerStateMachine* StateMachine;

	//当前攻击ID
	UPROPERTY(BlueprintReadOnly)
	int AttackID;

	//当前动作攻击帧
	UPROPERTY(BlueprintReadOnly)
	int AttackFrame;
	
	//可移动帧
	UPROPERTY(BlueprintReadOnly)
	int MovableFrame; 

	//攻击判定标记
	UPROPERTY(BlueprintReadOnly)
	bool bShouldJudge;

	//下一次按键组合
	UPROPERTY(BlueprintReadOnly)
	FKeyCombination NextkKeyCombation;

	//连续技列表
	UPROPERTY(BlueprintReadOnly)
	TMap<int, int> ComboMap;

	//载入数据库
	sqlite3* LoadDB();

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
