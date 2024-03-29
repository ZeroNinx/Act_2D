#include "PlayerCharacter.h"

//重复包含
#include "Kismet/GameplayStatics.h"
#include "PlayerAttackComponent.h"
#include "GlobalBlueprintFunctionLibrary.h"
#include "UIInterface.h"

//构造函数
APlayerCharacter::APlayerCharacter()
{

 	//启用Tick
	PrimaryActorTick.bCanEverTick = true;

	//设定模型
	GetCapsuleComponent()->SetCapsuleHalfHeight(70.0f);
	GetSprite()->SetRelativeLocation(FVector(0.0f, 0.0f, 8.0f));
	GetSprite()->SetRelativeScale3D(FVector(5.0f, 5.0f, 5.0f));

	//设定移动组件
	GetCharacterMovement()->GravityScale = 2.5f;
	GetCharacterMovement()->JumpZVelocity = 600.0f;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 100.0f;
	GetCharacterMovement()->bUseSeparateBrakingFriction = true;
	GetCharacterMovement()->BrakingFriction = 8.0f;
	GetCharacterMovement()->AirControl = 1;
	JumpMaxHoldTime = 0.2f;

	//默认角色朝右
	bFacingRight = true;

	//胶囊体组件
	GetCapsuleComponent()->SetCapsuleRadius(23.0f);
	
	//弹簧臂组件
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);

	//相机组件
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	//状态机
	StateMachine = CreateDefaultSubobject<UStateMachine>(TEXT("StateMachine"));
}

//开始游戏
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	UGlobalBlueprintFunctionLibrary::UpdatePlayerCharacter(GetWorld(), this);
	CameraComponent->Activate();
}


//Tick函数
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 获取攻击组件
	UPlayerAttackComponent* AttackComponent = GetAttackComponent();
	if (!AttackComponent)
	{
		return;
	}

	//当非战斗时自动调整动画
	if (!AttackComponent->IsAttacking() && GetState() != EState::Hit)
	{
		UpdateDirection();
		UpdateState();
	}
	else if (AttackComponent->AttackID == 4)
	{
		//攻击落地强制恢复
		UpdateState();
		if (GetState() == EState::Idle || GetState() == EState::Run)
		{
			AttackComponent->ResetAttack();
			GetSprite()->SetLooping(true);
			GetSprite()->Play();
		}
	}

}

//调整方向
void APlayerCharacter::UpdateDirection()
{
	if (bFacingRight)
	{
		GetSprite()->SetRelativeRotation(FRotator(0, 0, 0));
	}
	else
	{
		GetSprite()->SetRelativeRotation(FRotator(0, 180.0f, 0));
	}
}

void APlayerCharacter::RestoreFromAttack()
{
	UPlayerAttackComponent* AttackComponent = GetAttackComponent();
	if (!AttackComponent)
	{
		UGlobalBlueprintFunctionLibrary::LogWarning("APlayerCharacter::RestoreFromAttack AttackComponent Invalid");
		return;
	}

	AttackComponent->ResetAttack();
}

//调整状态
void APlayerCharacter::UpdateState()
{
	//获取速度
	FVector Velocy = GetCharacterMovement()->GetLastUpdateVelocity();

	//根据Z轴速度判断起跳/下落
	if (Velocy.Z> 0)
	{
		StateMachine->SetState(EState::Jump);
	}
	else if(Velocy.Z <0)
	{
		StateMachine->SetState(EState::Fall);
	}
	else
	{
		//否则根据X轴速度判断奔跑/静止
		if (UKismetMathLibrary::Abs(Velocy.X) != 0)
		{
			StateMachine->SetState(EState::Run);	
		}
		else
		{
			StateMachine->SetState(EState::Idle);
		}	
	}
}

//设置状态
void APlayerCharacter::SetState(EState NewState)
{
	StateMachine->SetState(NewState);
}

//获得状态
EState APlayerCharacter::GetState()
{
	return StateMachine->GetState();
}

bool APlayerCharacter::IsInState(EState InState)
{
	return StateMachine->IsInState(InState);
}

//获得攻击组件
UPlayerAttackComponent* APlayerCharacter::GetAttackComponent()
{
	return GetComponentByClass<UPlayerAttackComponent>();
}

//受击函数
void APlayerCharacter::Hit_Implementation(AActor* Attacker, FAttackProperty AttackProperty)
{
	if (GetState() != EState::Hit)
	{
		GetCharacterMovement()->StopMovementImmediately();
		UPlayerAttackComponent* AttackComponent = GetAttackComponent();
		if (!AttackComponent)
		{
			UGlobalBlueprintFunctionLibrary::LogWarning("APlayerCharacter::Hit_Implementation AttackComponent Invalid");
			return;
		}
		AttackComponent->ResetAttack();

		//添加瞬时速度
		bFacingRight = Attacker->GetActorLocation().X > GetActorLocation().X;
		UpdateDirection();
		float DirectMark = bFacingRight ? -1.0f : 1.0f;
		float VelocyX = 900.0f * DirectMark;
		GetCharacterMovement()->Velocity = FVector(VelocyX, 0, 0);

		//TODO：设定动画
		SetState(EState::Hit);
		PlayHitAnimation();

		HealthPoint -= 1;
		
		UUserWidget* MainUI = UGlobalBlueprintFunctionLibrary::GetMainUI(GetWorld());
		if (MainUI)
		{
			IUIInterface::Execute_UpdatePlayerHP(MainUI, HealthPoint);
		}
		
	}
}

void APlayerCharacter::SetGlobalDelay(float Delation, float DelayDuration)
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), Delation);

	auto RestoreFunction = [&]()->void
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
	};
	auto dlg = FTimerDelegate::CreateLambda(RestoreFunction);
	GetWorldTimerManager().SetTimer(HitDelayTimerHandle, dlg, DelayDuration * Delation, false);
}

