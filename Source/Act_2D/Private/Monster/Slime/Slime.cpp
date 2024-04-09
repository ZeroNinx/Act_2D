#include "Monster/Slime/Slime.h"
#include "Player/PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Utils/GlobalBlueprintFunctionLibrary.h"

//构造函数
ASlime::ASlime():Super()
{
	bFacingRight = false;

	//设置变换
	GetCapsuleComponent()->SetCapsuleHalfHeight(34.0f);
	RealCapsule->SetRelativeRotation(FRotator(90.0f,0,0));
	RealCapsule->SetCapsuleHalfHeight(44.0f);
	RealCapsule->SetCapsuleRadius(33.0f);
	GetSprite()->SetRelativeLocation(FVector(0, 0, 44.0f));
	
	//设置摩擦力
	GetCharacterMovement()->AirControl = 0;
	GetCharacterMovement()->GravityScale = 0.8f;
	GetCharacterMovement()->BrakingDecelerationWalking = 200.0f;
	GetCharacterMovement()->GroundFriction = 2.0f;

	//设定攻击组件
	AttackCompnent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("AttackComponent"));
	AttackCompnent->SetupAttachment(GetSprite());
	AttackCompnent->SetCollisionResponseToAllChannels(ECR_Ignore);
	AttackCompnent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	AttackCompnent->SetVisibility(false);
}

//跳跃攻击
void ASlime::JumpAttack()
{
	if (StateMachine->GetState() == EState::Idle&&!bFalled)
	{
		//延迟跳跃
		auto DelayJumpAttack = [&]()->void
		{
			//设定起跳速度
			float DirectMark = bFacingRight ? 1.0f : -1.0f;
			float JumpSpeed = 300.0f * DirectMark;
			GetCharacterMovement()->Velocity = FVector(JumpSpeed, 0, 0);
			Jump();

			//检测已存在重叠
			APlayerCharacter* Character = UGlobalBlueprintFunctionLibrary::GetPlayerCharacter();
			bool bOverlappingPlayer = AttackCompnent->IsOverlappingActor(Character);
			if (bOverlappingPlayer)
			{
				AttackJudge(Character);
			}
		};
		auto dlg = FTimerDelegate::CreateLambda(DelayJumpAttack);
		GetWorldTimerManager().SetTimer(JumpAttackHandle, dlg, (const float)JumpReadyTime, false);
	
	}
}

//被击中
void ASlime::OnHit(AActor* Attacker, FSkillProperty HitAttackProperty)
{
	Super::OnHit(Attacker, HitAttackProperty);

	float DirectMark = bFacingRight ? -1.0f : 1.0f;
	float LightVelocyX = 200.0f * DirectMark;
	float HeavyVelocyX = 600.0f * DirectMark;

	if (HitAttackProperty.HarmfulType == EAttackHarmfulType::HeavyAttack)
	{
		GetCharacterMovement()->Velocity = FVector(HeavyVelocyX, 0, 0);
	}
	else
	{
		GetCharacterMovement()->Velocity = FVector(LightVelocyX, 0, 0);
	}

}

//攻击组件重叠时
void ASlime::OnAttackComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

//攻击判定
void ASlime::AttackJudge(AActor* HitActor)
{
	//为玩家设定攻击
	Skill = NewObject<UMS_SlimeAttack>();
	IActorInterface::Execute_Hit(HitActor, this, Skill->AttackProperty);
}
