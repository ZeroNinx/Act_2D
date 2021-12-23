#pragma once

//自定义
#include "ActorInterface.h"

//UE4
#include "CoreMinimal.h"
#include "SceneActor.generated.h"

/**
 * 场景Actor基类
 */
UCLASS()
class ACT_2D_API ASceneActor : public AActor, public IActorInterface
{
	GENERATED_BODY()

public:

	//构造函数
	ASceneActor();

protected:

	//游戏开始执行
	virtual void BeginPlay() override;

	//每帧执行
	virtual void Tick(float DeltaTime) override;

	//初始化动画资源
	virtual void InitAnimation() {};

};
