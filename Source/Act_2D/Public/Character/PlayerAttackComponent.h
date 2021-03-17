#pragma once

//第三方
#include "sqlite3.h"

//自定义类
#include "PlayerStateMachine.h"

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
	bool JumpKey = false;
	bool UpKey=false;
	bool DownKey=false;
	bool LeftKey=false;
	bool RighKey=false;


	FKeyCombination() {};
	FKeyCombination(bool isAttackPressed,bool isJumpPressed, bool isUpPressed, bool isDownPressed, bool isLeftPressed, bool isRightPressed)
	{
		AttackKey = isUpPressed;
		JumpKey = isJumpPressed;
		UpKey = isUpPressed;
		DownKey = isDownPressed;
		LeftKey = isLeftPressed;
		RighKey = isRightPressed;
	}

	bool IsEmpty()
	{
		return AttackKey | JumpKey | UpKey | DownKey | LeftKey | RighKey;
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

	//返回是否接受输入
	UFUNCTION(BlueprintCallable)
	bool IsAcceptInput();

	//记录下一次攻击组合
	UFUNCTION(BlueprintCallable)
	void RecordKeyCombination();

	//攻击
	UFUNCTION(BlueprintCallable)
	void Attack(int AttackID = 1);

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

	//当前动作攻击帧
	UPROPERTY(BlueprintReadOnly)
	int AttackFrame;
	
	//攻击判定标记
	UPROPERTY(BlueprintReadOnly)
	bool bShouldJudge;

	//下一次按键组合
	UPROPERTY(BlueprintReadOnly)
	FKeyCombination NextkKeyCombation;

	//设定攻击
	UFUNCTION()
	void SetupAttack(FString FlipbookReference, FString SpriteReference, int SkillAttackFrame);

	//攻击判定
	UFUNCTION()
	void AttackJudge();

	//获得当前攻击动画播放位置
	UFUNCTION(BlueprintCallable)
	int GetAnimationPosition();

	//Tick函数
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};
