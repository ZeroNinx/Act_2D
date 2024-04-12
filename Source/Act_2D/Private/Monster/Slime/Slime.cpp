#include "Monster/Slime/Slime.h"
#include "Player/PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Utils/GlobalBlueprintFunctionLibrary.h"
#include "PaperZDAniminstance.h"

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

}

void ASlime::OnJumpReadyAmimEnd()
{
	EnterAttackState();

	//设定起跳速度
	float DirectMark = bFacingRight ? 1.0f : -1.0f;
	float JumpSpeed = 400.0f * DirectMark;
	GetCharacterMovement()->Velocity = FVector(JumpSpeed, 0, 0);
	Jump();

}

//被击中
void ASlime::OnHit(AActor* Attacker, FSkillProperty HitAttackProperty)
{
	Super::OnHit(Attacker, HitAttackProperty);

	// 受击以后结束技能
	UMonsterSkillComponent* SkillComponent = GetSkillComponent();
	if (SkillComponent)
	{
		SkillComponent->ForceSkillEnd();
	}

	if (HitAttackProperty.SkillType == ESkillType::HarmfulAttack)
	{
		if (HitAttackProperty.HarmfulType == EAttackHarmfulType::LightAttack)
		{
			SetHorizonVelocity(-200.f);
		}
		else if (HitAttackProperty.HarmfulType == EAttackHarmfulType::HeavyAttack)
		{
			SetHorizonVelocity(-600.f);
		}
	}

}

void ASlime::EnterAttackState()
{
	UMonsterSkillComponent* SkillComponent = GetSkillComponent();
	if (SkillComponent)
	{
		SkillComponent->UseSkill(GetAttackSkill());
	}
}

void ASlime::OnJumpStateChanged()
{
	if (bMovingOnGround)
	{
		//落地
		
		//结束技能
		UMonsterSkillComponent* SkillComponent = GetSkillComponent();
		if (SkillComponent)
		{
			SkillComponent->ForceSkillEnd();
		}

		// 播放落地动画
		if (GetState() != EState::Hit)
		{
			GetAnimInstance()->JumpToNode(FName(TEXT("Land")));
		}
	}
	else
	{
		//起跳后
		EnterAttackState();
	}
}

void ASlime::Attack_Implementation()
{
	GetAnimInstance()->JumpToNode(FName(TEXT("JumpReady")));
}
