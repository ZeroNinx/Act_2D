// Fill out your copyright notice in the Description page of Project Settings.


#include "Slime.h"

//构造函数
ASlime::ASlime():AMonster()
{
	bFacingRight = false;

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

	//设定行为树
	BTComponent = CreateDefaultSubobject<USlimeBTComponent>(TEXT("BTComponent"));


	OnEffectPlayFinishedDelegate.BindDynamic(this, &ASlime::RemoveEffect);
	OnDamagedDelegate.BindDynamic(this, &ASlime::FinishDamaged);
}

//被击中
void ASlime::Hit(FAttackProperty AttackProperty)
{
	bThreadSleeped = false;
	bMoved = false;

	//设定受到攻击
	StateMachine->SetState(EState::UnderAttack);
	SpeedScale = 1.0f;

	//播放受击动画
	UPaperFlipbook* HitFlipbook = LoadObject<UPaperFlipbook>(this, TEXT("PaperFlipbook'/Game/Paper2D/Monster/Slime/Slime_Hit.Slime_Hit'"));
	if (!HitFlipbook)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Load Hit Flipbook Failed"));
		return;
	}
	if (GetSprite()->GetFlipbook() == HitFlipbook)
		GetSprite()->PlayFromStart();
	GetSprite()->SetFlipbook(HitFlipbook);
	GetSprite()->SetLooping(false);
	GetSprite()->OnFinishedPlaying.Add(OnDamagedDelegate);

	//播放特效
#pragma region Play Effect

	//清理没用的特效
	if (Effect)
	{
		Effect->Destroy();
	}
	Effect = GetWorld()->SpawnActor<APaperFlipbookActor>(APaperFlipbookActor::StaticClass(),
		GetActorLocation() + FVector(-5.0f, 1.0f,10.0f), GetActorRotation());

	if (!Effect)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Create Effect Failed"));
		return;
	}

	UPaperFlipbook* EffectFlipbook = LoadObject<UPaperFlipbook>(this, TEXT("PaperFlipbook'/Game/Paper2D/Monster/Slime/Slime_Effict.Slime_Effict'"));
	if (!EffectFlipbook)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Load Effect Failed"));
		return;
	}

	Effect->GetRenderComponent()->SetFlipbook(EffectFlipbook);
	Effect->GetRenderComponent()->SetLooping(false);
	Effect->GetRenderComponent()->OnFinishedPlaying.Add(OnEffectPlayFinishedDelegate);
	Effect->GetRenderComponent()->Play();
#pragma endregion

	

}

//tick函数
void ASlime::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (StateMachine->GetState() == EState::UnderAttack)
	{
		MoveBack();
		if (Effect->GetRenderComponent()->GetPlaybackPositionInFrames() == 1)
		{
			if (!bThreadSleeped)
			{
				bThreadSleeped = true;
				FPlatformProcess::Sleep(0.1f);
			}
		}
	}
	else
	{
		UpdateState();
		UpdateAnimation();
	}

}

//更新状态
void ASlime::UpdateState()
{
	//获取速度
	FVector Velocy = GetCharacterMovement()->GetLastUpdateVelocity();

	//判断速度
	if (UKismetMathLibrary::Abs(Velocy.Z) > 0)
	{
		StateMachine->SetState(EState::Jumping);
	}
	else
	{
		StateMachine->SetState(EState::Idle);
	}
}

//更新动画
void ASlime::UpdateAnimation()
{
	//动画路径
	FString AnimationFlipbook;

	//根据不同状态调整动画
	switch (StateMachine->GetState())
	{
	case EState::Idle:
		AnimationFlipbook = "PaperFlipbook'/Game/Paper2D/Monster/Slime/Slime_Idle.Slime_Idle'";
		break;
	case EState::Jumping:
		AnimationFlipbook = "PaperFlipbook'/Game/Paper2D/Monster/Slime/Slime_Jump.Slime_Jump'";
		break;;
	case EState::Sleeping:
		AnimationFlipbook = "PaperFlipbook'/Game/Paper2D/Monster/Slime/Slime_Sleep.Slime_Sleep'";
		break;
	}

	//设定动画
	UPaperFlipbook* NewAnimation = LoadObject<UPaperFlipbook>(GetWorld(), *AnimationFlipbook);
	if (!NewAnimation)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Load Slime Animation Flipbook Failed"));
		return;
	}

	GetSprite()->SetFlipbook(NewAnimation);
}

//移除特效
void ASlime::RemoveEffect()
{
	StateMachine->SetState(EState::Idle);
	Effect->Destroy();
	Effect->GetRenderComponent()->OnFinishedPlaying.Remove(OnEffectPlayFinishedDelegate);
}

//后退
void ASlime::MoveBack()
{
	//根据速度逐步减少加速度
	FVector Velocy = GetCharacterMovement()->GetLastUpdateVelocity();
	UKismetSystemLibrary::PrintString(GetWorld(), FString::FromInt(Velocy.X));

	float DirectMark = bFacingRight ? -1.0f : 1.0f;

	if (!bMoved || Velocy.X * DirectMark > 0)//防止角色倒退
	{
		bMoved = true;
		AddMovementInput(FVector(1.5f, 0, 0), DirectMark * SpeedScale);
		SpeedScale -= 0.1f;
	}
}

//完成受击
void ASlime::FinishDamaged()
{
	UpdateState();
	GetSprite()->SetLooping(true);
	GetSprite()->Play();
}

