#pragma once

//UE4
#include "CoreMinimal.h"
#include "PaperFlipbook.h"
#include "PaperSpriteActor.h"
#include "SceneActor.generated.h"

/**
 * 场景Actor基类
 */
UCLASS()
class ACT_2D_API ASceneActor : public APaperSpriteActor
{
	GENERATED_BODY()

public:

	//构造函数
	ASceneActor();

	/** 受击函数 */
	virtual void Hit();

protected:

	//游戏开始执行
	virtual void BeginPlay() override;

	//每帧执行
	virtual void Tick(float DeltaTime) override;

	//初始化动画资源
	virtual void InitAnimation() {};

};
