// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"

//构造函数
AMonster::AMonster()
{

	//启用tick
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	//调整自定义碰撞体
	RealCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RealCapsuleComponent"));
	RealCapsule->SetupAttachment(RootComponent);
	RealCapsule->SetCollisionResponseToAllChannels(ECR_Ignore);
	RealCapsule->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	RealCapsule->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);

}


//获取行为树
UBehaviorTree* AMonster::GetBehaviorTree()
{
	return BTComponent->GetCurrentTree();
}

//被击中函数
void AMonster::Hit()
{
	FString Name;
	GetName(Name);
	UKismetSystemLibrary::PrintString(GetWorld(), Name+FString(" Hit"));
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

