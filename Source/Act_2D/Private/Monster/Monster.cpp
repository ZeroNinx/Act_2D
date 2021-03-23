// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"

//构造函数
AMonster::AMonster()
{
	//启用tick
	PrimaryActorTick.bCanEverTick = true;

	//动画组件
	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("FlipbookComponent"));
	FlipbookComponent->SetupAttachment(RootComponent);
	FlipbookComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 5.0f));
	FlipbookComponent->SetRelativeScale3D(FVector(5.0f, 5.0f, 5.0f));

}

//游戏开始执行
void AMonster::BeginPlay()
{
	Super::BeginPlay();
	
}

//每帧执行
void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

