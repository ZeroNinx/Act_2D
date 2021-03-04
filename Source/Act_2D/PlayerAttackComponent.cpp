#include "PlayerAttackComponent.h"

//构造函数
UPlayerAttackComponent::UPlayerAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	//缩放设置
	SetRelativeScale3D(FVector(5.0f, 5.0f, 5.0f));

}


//游戏开始时执行
void UPlayerAttackComponent::BeginPlay()
{
	Super::BeginPlay();
}

//Tick函数
void UPlayerAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

