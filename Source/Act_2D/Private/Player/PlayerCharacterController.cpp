#include "Player/PlayerCharacterController.h"

//重复包含
#include "Player/PlayerCharacter.h"
#include "Player/PlayerAttackComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "Utils/GlobalBlueprintFunctionLibrary.h"

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
	bool bLeftPressed = KeyPressMap.Find(EGameKeyType::Left)? KeyPressMap[EGameKeyType::Left]:false;
	bool bRightPressed = KeyPressMap.Find(EGameKeyType::Right) ? KeyPressMap[EGameKeyType::Right] : false;

	if (IsAllowMove() && (bLeftPressed || bRightPressed))
	{
		//添加移动
		PlayerCharacter->AddMovementInput(FVector(1, 0, 0), bRightPressed ? 1.0f : -1.0f);

		//根据角色方向调整动画
		PlayerCharacter->bFacingRight = bRightPressed;
	}
}

FKeyCombination APlayerCharacterController::GetNextKeyCombation()
{
	FKeyCombination NextKeyCombation;
	InputCombinationQueue.Dequeue(NextKeyCombation);
	return MoveTemp(NextKeyCombation);
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
	KeyPressMap.Add(EGameKeyType::Up, true);
}
void APlayerCharacterController::UpReleased()
{
	KeyPressMap.Add(EGameKeyType::Up, false);
}

//下
void APlayerCharacterController::DownPressed()
{
	KeyPressMap.Add(EGameKeyType::Down, true);
}
void APlayerCharacterController::DownReleased()
{
	KeyPressMap.Add(EGameKeyType::Down, false);
}

//左
void APlayerCharacterController::LeftPressed()
{
	KeyPressMap.Add(EGameKeyType::Left, true);
}
void APlayerCharacterController::LeftReleased()
{
	KeyPressMap.Add(EGameKeyType::Left, false);
}

//右
void APlayerCharacterController::RightPressed()
{
	KeyPressMap.Add(EGameKeyType::Right, true);
}
void APlayerCharacterController::RightReleased()
{
	KeyPressMap.Add(EGameKeyType::Right, false);
}

//攻击键
void APlayerCharacterController::AttackPressed()
{
	KeyPressMap.Add(EGameKeyType::Attack, true);
	AddKeyCombination();
}
void APlayerCharacterController::AttackReleased()
{
	KeyPressMap.Add(EGameKeyType::Attack, false);
}

//特殊键
void APlayerCharacterController::SpecialPressed()
{
	KeyPressMap.Add(EGameKeyType::Special, true);
	AddKeyCombination();
}
void APlayerCharacterController::SpecialReleased()
{
	KeyPressMap.Add(EGameKeyType::Special, false);
}

//扳机键
void APlayerCharacterController::TriggerPressed()
{
	KeyPressMap.Add(EGameKeyType::Trigger, true);
	AddKeyCombination();
}
void APlayerCharacterController::TriggerReleased()
{
	KeyPressMap.Add(EGameKeyType::Trigger, false);
}

//跳跃键
void APlayerCharacterController::JumpPressed()
{
	KeyPressMap.Add(EGameKeyType::Jump, true);

	//可移动帧取消攻击
	if (AttackComponent->IsAttacking() && AttackComponent->IsMovable())
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
	KeyPressMap.Add(EGameKeyType::Jump, false);

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
			UGlobalBlueprintFunctionLibrary::SetMainUI(MainUI);
			MainUI->AddToViewport();
		}
	}
}

//是否允许移动
bool APlayerCharacterController::IsAllowMove()
{
	return PlayerCharacter && 
	!(PlayerCharacter->GetState() == EState::Attack) &&
	!(PlayerCharacter->GetState() == EState::Hit);
}

void APlayerCharacterController::AddKeyCombination()
{
	FKeyCombination KeyCombation;

	// 将按下的按键以Set形式转换为按键组合
	for (auto It : KeyPressMap)
	{
		if (It.Value == true)
		{
			KeyCombation.Keys.Add(It.Key);
		}
	}

	// 记录输入
	InputCombinationQueue.Enqueue(MoveTemp(KeyCombation));
}
