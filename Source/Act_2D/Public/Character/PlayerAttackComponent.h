#pragma once

#include "CoreMinimal.h"
#include "PaperSprite.h"
#include "PaperFlipbook.h"
#include "PaperSpriteComponent.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PlayerAttackComponent.generated.h"

//攻击动作
USTRUCT(BlueprintType)
struct FAttackAction
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	UPaperSprite* AttackSprite;//攻击动画

	UPROPERTY(BlueprintReadWrite)
	int AttackFrames;//攻击判定帧数

};

//攻击组件
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACT_2D_API UPlayerAttackComponent : public UPaperSpriteComponent
{
	GENERATED_BODY()

public:	
	
	//构造函数
	UPlayerAttackComponent();

	//攻击开始
	UFUNCTION(BlueprintCallable)
	void BeginAttack();

protected:



	
};
