#pragma once

#include "CoreMinimal.h"
#include "PaperSprite.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "Components/ActorComponent.h"
#include "PlayerAttackComponent.generated.h"

//攻击动作
USTRUCT(BlueprintType)
struct FAttackAction
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	UPaperFlipbook* Flipbook;//攻击动画

	UPROPERTY(BlueprintReadWrite)
	int ReadyFrames;//前摇帧数

	UPROPERTY(BlueprintReadWrite)
	int AttackFrames;//攻击判定帧数

	UPROPERTY(BlueprintReadWrite)
	int EndFrames;//后摇帧数	

	FAttackAction()
	{
		ReadyFrames = 0;
		AttackFrames = 0;
		EndFrames = 0;
	}

	FAttackAction(int Ready, int Attack, int End)
	{
		ReadyFrames = Ready;
		AttackFrames = Attack;
		EndFrames = End;
	}
};

//攻击组件
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACT_2D_API UPlayerAttackComponent : public UPaperFlipbookComponent
{
	GENERATED_BODY()

public:	
	
	//构造函数
	UPlayerAttackComponent();

protected:

	//游戏开始执行
	virtual void BeginPlay() override;

public:	

	//Tick函数
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
};
