// Fill out your copyright notice in the Description page of Project Settings.


#include "Slime.h"

//构造函数
ASlime::ASlime():AMonster()
{
	bFacingRight = false;

	//设置变换
	GetCapsuleComponent()->SetCapsuleHalfHeight(34.0f);
	RealCapsule->SetRelativeRotation(FRotator(90.0f,0,0));
	RealCapsule->SetCapsuleHalfHeight(44.0f);
	RealCapsule->SetCapsuleRadius(33.0f);
	GetSprite()->SetRelativeLocation(FVector(0, 0, 44.0f));
	GetArrowComponent()->SetRelativeRotation(FRotator(0, 180.0f, 0));

	//设置摩擦力
	GetCharacterMovement()->BrakingDecelerationWalking = 200.0f;
	GetCharacterMovement()->GroundFriction = 2.0f;

	//载入动画
	UPaperFlipbook* IdleFlipbook = LoadObject<UPaperFlipbook>(this, TEXT("PaperFlipbook'/Game/Paper2D/Monster/Slime/Slime_Idle.Slime_Idle'"));
	if(!IdleFlipbook)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), "Load Failed");
		return;
	}
	GetSprite()->SetFlipbook(IdleFlipbook);

	//设定特效
	UPaperFlipbook* EffectFlipbook = LoadObject<UPaperFlipbook>(this, TEXT("PaperFlipbook'/Game/Paper2D/Monster/Slime/Slime_Effict.Slime_Effict'"));
	if (!EffectFlipbook)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Load Effect Failed"));
		return;
	}
	Effect = CreateDefaultSubobject<UPaperFlipbookComponent>("Effect");
	Effect->SetupAttachment(RootComponent);
	Effect->SetFlipbook(EffectFlipbook);
	Effect->SetLooping(false);

	//设定受击动画
	HitFlipbook = LoadObject<UPaperFlipbook>(this, TEXT("PaperFlipbook'/Game/Paper2D/Monster/Slime/Slime_Hit.Slime_Hit'"));
	if (!HitFlipbook)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Load Hit Flipbook Failed"));
		return;
	}

	//设定行为树
	BTComponent = CreateDefaultSubobject<USlimeBTComponent>(TEXT("BTComponent"));

	//绑定代理
	OnDamagedDelegate.BindDynamic(this, &ASlime::FinishDamaged);
	GetSprite()->OnFinishedPlaying.Add(OnDamagedDelegate);
}

//被击中
void ASlime::Hit(FAttackProperty HitAttackProperty)
{
	AttackProperty = HitAttackProperty;
	PrepareHit();
	Effect->PlayFromStart();

	//添加瞬时速度
	float DirectMark = bFacingRight ? -1.0f : 1.0f;
	float LightVelocyX = 200.0f * DirectMark;
	float HeavyVelocyX = 500.0f * DirectMark;

	if (AttackProperty.HarmfulType == EAttackHarmfulType::HeavyAttack)
	{
		GetCharacterMovement()->Velocity = FVector(HeavyVelocyX, 0, 0);
	}
	else
	{
		GetCharacterMovement()->Velocity = FVector(LightVelocyX, 0, 0);
	}
}

//准备受击
void ASlime::PrepareHit()
{
	//改变状态
	StateMachine->SetState(EState::UnderAttack);

	//初始化受击动画
	GetSprite()->SetLooping(false);
	GetSprite()->SetFlipbook(HitFlipbook);
	GetSprite()->PlayFromStart();
}

//tick函数
void ASlime::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (StateMachine->GetState() == EState::UnderAttack)
	{
		FVector Velocy = GetCharacterMovement()->GetLastUpdateVelocity();
		UKismetSystemLibrary::PrintString(GetWorld(), FString::FromInt(Velocy.X));
	}
	else
	{
		UpdateDirection();
		UpdateState();
		UpdateAnimation();
	}

}

//更新方向
void ASlime::UpdateDirection()
{
	float PlayerLocationX = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation().X;
	if (PlayerLocationX < GetActorLocation().X)
	{
		bFacingRight = false;
		GetSprite()->SetRelativeRotation(FRotator(0, 0, 0));
		Effect->SetRelativeLocation(FVector(-5.0f, 1.0f, 50.0f));
	}
	else
	{
		bFacingRight = true;
		GetSprite()->SetRelativeRotation(FRotator(0, 180.0f, 0));
		Effect->SetRelativeLocation(FVector(40.0f, 1.0f, 50.0f));
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

//完成受击
void ASlime::FinishDamaged()
{
	GetSprite()->SetLooping(true);
	UpdateState();
	GetSprite()->Play();
}

