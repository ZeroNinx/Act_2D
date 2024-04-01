#include "PlayerCharacterController.h"

//重复包含
#include "PlayerCharacter.h"
#include "PlayerAttackComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "GlobalBlueprintFunctionLibrary.h"

//构造函数
APlayerCharacterController::APlayerCharacterController()
{
	
}

//初始化控制器
void APlayerCharacterController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	UE_LOG(LogTemp, Warning, TEXT("APlayerCharacterController::OnPossess"));

	//获取玩家指针和攻击组件
	PlayerCharacter = Cast<APlayerCharacter>(InPawn);
	AttackComponent = PlayerCharacter->GetAttackComponent();
}


void APlayerCharacterController::Tick(float DeltaSeconds)
{
	//当可移动时
	if (IsAllowMove() && (bLeftPressed||bRightPressed))
	{
		//添加移动
		PlayerCharacter->AddMovementInput(FVector(1, 0, 0), bRightPressed ? 1.0f : -1.0f);

		//根据角色方向调整动画
		PlayerCharacter->bFacingRight = bRightPressed;
	}
}

//初始化输入
void APlayerCharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//上
	InputComponent->BindAction("Up", IE_Pressed, this, &APlayerCharacterController::UpPressed);
	InputComponent->BindAction("Up", IE_Released, this, &APlayerCharacterController::UpReleased);
	//下
	InputComponent->BindAction("Down", IE_Pressed, this, &APlayerCharacterController::DownPressed);
	InputComponent->BindAction("Down", IE_Released, this, &APlayerCharacterController::DownReleased);
	//左
	InputComponent->BindAction("Left", IE_Pressed, this, &APlayerCharacterController::LeftPressed);
	InputComponent->BindAction("Left", IE_Released, this, &APlayerCharacterController::LeftReleased);
	//右
	InputComponent->BindAction("Right", IE_Pressed, this, &APlayerCharacterController::RightPressed);
	InputComponent->BindAction("Right", IE_Released, this, &APlayerCharacterController::RightReleased);

	//攻击键
	InputComponent->BindAction("Attack", IE_Pressed, this, &APlayerCharacterController::AttackPressed);
	InputComponent->BindAction("Attack", IE_Released, this, &APlayerCharacterController::AttackReleased);

	//特殊键
	InputComponent->BindAction("Special", IE_Pressed, this, &APlayerCharacterController::SpecialPressed);
	InputComponent->BindAction("Special", IE_Released, this, &APlayerCharacterController::SpecialReleased);

	//扳机键
	InputComponent->BindAction("Trigger", IE_Pressed, this, &APlayerCharacterController::TriggerPressed);
	InputComponent->BindAction("Trigger", IE_Released, this, &APlayerCharacterController::TriggerReleased);

	//跳跃键
	InputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacterController::JumpPressed);
	InputComponent->BindAction("Jump", IE_Released, this, &APlayerCharacterController::JumpReleased);

}

//上
void APlayerCharacterController::UpPressed()
{
	bUpPressed = true;

	//避免冲突
	bDownPressed = false;
}
void APlayerCharacterController::UpReleased()
{
	bUpPressed = false;
}

//下
void APlayerCharacterController::DownPressed()
{
	bDownPressed = true;

	//避免冲突
	bUpPressed = false;
}
void APlayerCharacterController::DownReleased()
{
	bDownPressed = false;
}

//左
void APlayerCharacterController::LeftPressed()
{
	bLeftPressed = true;

	//避免冲突
	bRightPressed = false;
}
void APlayerCharacterController::LeftReleased()
{
	bLeftPressed = false;
}

//右
void APlayerCharacterController::RightPressed()
{
	bRightPressed = true;

	//避免冲突
	bLeftPressed = false;
}
void APlayerCharacterController::RightReleased()
{
	bRightPressed = false;
}

//攻击键
void APlayerCharacterController::AttackPressed()
{
	bAttackPressed = true;
	AddAttackInput();
}
void APlayerCharacterController::AttackReleased()
{
	bAttackPressed = false;
	AddAttackInput();
}

//特殊键
void APlayerCharacterController::SpecialPressed()
{
	bSpecialPressed = true;
	AddAttackInput();
}
void APlayerCharacterController::SpecialReleased()
{
	bSpecialPressed = false;
	AddAttackInput();
}

//扳机键
void APlayerCharacterController::TriggerPressed()
{
	bTriggerPressed = true;
	AddAttackInput();
}
void APlayerCharacterController::TriggerReleased()
{
	bTriggerPressed = false;
	AddAttackInput();
}

//跳跃键
void APlayerCharacterController::JumpPressed()
{
	bJumpPressed = true;
	RecordKeyCombination();

	//可移动帧取消攻击
	if (PlayerCharacter->IsInState(EState::Attack) && AttackComponent->IsMovable())
	{
		PlayerCharacter->RestoreFromAttack();
	}

	//当可移动时
	if (IsAllowMove())
	{
		PlayerCharacter->Jump();
	}

}
void APlayerCharacterController::JumpReleased()
{
	bJumpPressed = false;
	RecordKeyCombination();

	PlayerCharacter->StopJumping();
}

void APlayerCharacterController::BeginPlay()
{
	Super::BeginPlay();

	InitializeMainUI();

}

void APlayerCharacterController::InitializeMainUI()
{
	UClass* MainUIClass = LoadClass<UUserWidget>(nullptr, TEXT("WidgetBlueprint'/Game/Blueprints/UMG/MainUI.MainUI_C'"));
	if (MainUIClass)
	{
		UUserWidget* MainUI = CreateWidget(GetWorld(), MainUIClass);
		if (MainUI)
		{
			UGlobalBlueprintFunctionLibrary::SetMainUI(GetWorld(), MainUI);
			MainUI->AddToViewport();
		}
	}
}

//是否允许移动
bool APlayerCharacterController::IsAllowMove()
{
	return PlayerCharacter && 
	!PlayerCharacter->IsInState(EState::Attack) &&
	!PlayerCharacter->IsInState(EState::Hit);
}


//记录输入组合
void APlayerCharacterController::RecordKeyCombination()
{
	NextkKeyCombation = FKeyCombination(bAttackPressed, bSpecialPressed, bTriggerPressed, bJumpPressed, bUpPressed, bDownPressed, bLeftPressed, bRightPressed);
}

//添加攻击输入
void APlayerCharacterController::AddAttackInput()
{
	NextkKeyCombation = FKeyCombination(bAttackPressed, bSpecialPressed, bTriggerPressed, bJumpPressed, bUpPressed, bDownPressed, bLeftPressed, bRightPressed);
	//设置延迟接受输入
	auto DelayAttackInput = [&]() -> void
	{
		if (PlayerCharacter && PlayerCharacter->GetState() != EState::Hit)
			AttackComponent->SetKeyCombination(NextkKeyCombation);
	};
	auto dlg = FTimerDelegate::CreateLambda(DelayAttackInput);
	GetWorldTimerManager().SetTimer(AttackDelayHandle, dlg, (const float)AttackInputDuration, false);
}

//立刻停止移动
void APlayerCharacterController::PrepareAttack()
{
	PlayerCharacter->GetCharacterMovement()->StopMovementImmediately();

}

