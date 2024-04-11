// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Monster.h"
#include "Monster/MonsterController.h"
#include "Utils/GlobalBlueprintFunctionLibrary.h"
#include "PaperZDAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/PlayerCharacter.h"

//构造函数
AMonster::AMonster()
{
	//启用tick
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	//状态机
	StateMachine = CreateDefaultSubobject<UStateMachine>(TEXT("State Machine"));
	StateMachine->SetState(EState::Idle);

	//调整自定义碰撞体
	RealCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RealCapsuleComponent"));
	RealCapsule->SetupAttachment(RootComponent);
	RealCapsule->SetCollisionResponseToAllChannels(ECR_Ignore);
	RealCapsule->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	RealCapsule->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);

	//受击动画组件
	HitEffectComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("HitEffectComponent"));
	HitEffectComponent->SetupAttachment(GetSprite());
	HitEffectComponent->SetLooping(false);

	// 设置默认的AIController
	UClass* MonsterControllerClass = StaticLoadClass(AMonsterController::StaticClass(), nullptr, TEXT("/Game/Blueprints/Monster/BP_MonsterController.BP_MonsterController_C"));
	if (MonsterControllerClass)
	{
		AIControllerClass = MonsterControllerClass;
	}

}

bool AMonster::IsAlive()
{
	return GetHealthPoint() > 0;
}

EState AMonster::GetState()
{
	return StateMachine->GetState();
}

void AMonster::SetState(EState State)
{
	StateMachine->SetState(State);
}

//获取HP
int AMonster::GetHealthPoint()
{
	return HealthPoint;
}

UMonsterSkillComponent* AMonster::GetSkillComponent()
{
	return GetComponentByClass<UMonsterSkillComponent>();
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
	if (StateMachine->GetState() != EState::Hit)
	{
		UpdateFacingDirection();
		UpdateState();
	}
	UpdateJumpState();
}

//更新朝向
void AMonster::UpdateFacingDirection()
{
	APawn* PlayerPawn = UGlobalBlueprintFunctionLibrary::GetPlayerCharacter();
	if (!PlayerPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("AMonster::UpdateFacingDirection PlayerPawn Invalid"));
		return;
	}

	float PlayerLocationX = PlayerPawn->GetActorLocation().X;
	if (PlayerLocationX < GetActorLocation().X)
	{
		bFacingRight = false;
		GetSprite()->SetRelativeRotation(FRotator(0, 0, 0));
	}
	else
	{
		bFacingRight = true;
		GetSprite()->SetRelativeRotation(FRotator(0, 180.0f, 0));
	}
}

//更新状态
void AMonster::UpdateState()
{
	//获取速度
	FVector Velocy = GetCharacterMovement()->GetLastUpdateVelocity();

	//判断速度
	if (Velocy.Z > 0)
	{
		StateMachine->SetState(EState::Jump);
	}
	else if (Velocy.Z < 0)
	{
		StateMachine->SetState(EState::Fall);
	}
	else
	{
		StateMachine->SetState(EState::Idle);
	}
}

void AMonster::UpdateJumpState()
{
	bool NewIsMovingOnGround = GetCharacterMovement()->IsMovingOnGround();
	if (!NewIsMovingOnGround == IsMovingOnGround)
	{
		IsMovingOnGround = NewIsMovingOnGround;
		OnJumpStateChanged();
	}
}

void AMonster::OnJumpStateChanged()
{

}

void AMonster::PlayDeathEffect()
{
	SetActorTickEnabled(false);
	GetCapsuleComponent()->SetEnableGravity(false);
	GetSprite()->Stop();

	AMonsterController* MonsterController = Cast<AMonsterController>(GetController());
	if (MonsterController)
	{
		MonsterController->StopBehaviorTree();
	}

	DeathFlashCounter = 0;
	auto PlayDeathEffect = [&]()
	{
		bool bVisable = (DeathFlashCounter % 2 == 1);
		this->GetSprite()->SetVisibility(bVisable);
		DeathFlashCounter++;
		if (DeathFlashCounter >= 5)
		{
			GetWorldTimerManager().ClearTimer(DeathEffectTimerHandle);
			this->Destroy();
		}
	};
	GetWorldTimerManager().SetTimer(DeathEffectTimerHandle, PlayDeathEffect, 0.3, true);
}

void AMonster::Hit_Implementation(AActor* Attacker, FSkillProperty HitAttackProperty)
{
	if (IsAlive())
	{
		OnHit(Attacker, HitAttackProperty);
	}
}

void AMonster::OnHit(AActor* Attacker, FSkillProperty HitAttackProperty)
{
	//改变状态
	StateMachine->SetState(EState::Hit);

	//播放受击动画
	GetAnimInstance()->JumpToNode(FName(TEXT("Hit")));

	//重新播放效果
	HitEffectComponent->PlayFromStart();

	//对于伤害形技能，扣血
	if (HitAttackProperty.SkillType == ESkillType::HarmfulAttack)
	{
		HealthPoint -= HitAttackProperty.Damage;
	}

	//死亡判断
	if (!IsAlive())
	{
		StateMachine->SetState(EState::Dead);
		OnDead();
	}
}

void AMonster::OnDead()
{
	PlayDeathEffect();
}

void AMonster::OnHitAnimationPlayComplete()
{
	// 死亡不处理
	if (!IsAlive())
	{
		return;
	}

	SetState(EState::Idle);
}

void AMonster::PlayOverrideAnim(class UPaperZDAnimSequence* InAnimSquence)
{
	UPaperZDAnimInstance* AnimationInstance = GetAnimInstance();
	if (AnimationInstance)
	{
		AnimationInstance->PlayAnimationOverride(InAnimSquence);
	}
}

void AMonster::Attack_Implementation()
{

}
