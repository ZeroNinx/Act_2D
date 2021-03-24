// Fill out your copyright notice in the Description page of Project Settings.


#include "Slime.h"

//构造函数
ASlime::ASlime():AMonster()
{
	//设置变换
	RealCapsule->SetRelativeLocation(FVector(0, 0, -57.0f));
	RealCapsule->SetRelativeRotation(FRotator(90.0f,0,0));
	RealCapsule->SetCapsuleHalfHeight(44.0f);
	RealCapsule->SetCapsuleRadius(33.0f);
	GetSprite()->SetRelativeLocation(FVector(0, 0, -10.0f));

	//载入动画
	UPaperFlipbook* IdleFlipbook = LoadObject<UPaperFlipbook>(this, TEXT("PaperFlipbook'/Game/Paper2D/Monster/Slime/Slime_Idle.Slime_Idle'"));
	if(!IdleFlipbook)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), "Load Failed");
		return;
	}
	GetSprite()->SetFlipbook(IdleFlipbook);
}

//tick函数
void ASlime::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

