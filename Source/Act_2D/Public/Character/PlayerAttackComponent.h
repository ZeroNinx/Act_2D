#pragma once

//第三方
#include <string>
#include "sqlite3.h"


//UE4
#include "Misc/Paths.h"
#include "CoreMinimal.h"
#include "PaperSprite.h"
#include "PaperFlipbook.h"
#include "PaperSpriteComponent.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PlayerAttackComponent.generated.h"


//攻击组件
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACT_2D_API UPlayerAttackComponent : public UPaperSpriteComponent
{
	GENERATED_BODY()

public:	
	
	//构造函数
	UPlayerAttackComponent();

	//攻击
	UFUNCTION(BlueprintCallable)
	void Attack(int AttackID = 1);


protected:

	//下一个攻击帧
	UPROPERTY(BlueprintReadOnly)
	int NextAttackFrame;
	
	//攻击判定标记
	UPROPERTY(BlueprintReadOnly)
	bool bShouldJudge;

	//设定攻击范围
	UFUNCTION()
	void SetupAttack(FString resource, int AttackFrame);

	//攻击判定
	UFUNCTION()
	void AttackJudge();

	//获得当前攻击动画播放位置
	UFUNCTION(BlueprintCallable)
	int GetCurrentActionFrame();

	//Tick函数
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};
