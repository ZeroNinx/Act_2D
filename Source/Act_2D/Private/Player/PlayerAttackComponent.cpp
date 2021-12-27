#include "PlayerAttackComponent.h"

//互相包含
#include "Monster.h"
#include "PlayerSkill.h"
#include "StateMachine.h"
#include "PlayerCharacterController.h"

//构造函数
UPlayerAttackComponent::UPlayerAttackComponent()
{
	//启用tick
	PrimaryComponentTick.bCanEverTick = true;

	//设置重叠
	SetCollisionResponseToAllChannels(ECR_Ignore);
	SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);

	//隐藏攻击模块
	SetVisibility(false);

}

//Tick函数
void UPlayerAttackComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	//当攻击外进入攻击
	if (AttackID == 0 && !NextKeyCombation.IsAttackEmpty())
	{
		//跳跃攻击
		bool bPlayerJumping = PlayerCharacter->IsInState(EState::Jumping|EState::Falling);
		if (bPlayerJumping)
		{
			Attack(4);
		}
		else //一般攻击
		{
			//获得命令
			int NextCommand = NextKeyCombation.GetCommand();
			NextKeyCombation.Clear();

			//进行对应攻击
			SetupCombo();
			if (ComboMap.Contains(NextCommand))
			{
				//立即停止移动，开始攻击
				PlayerCharacter->GetCharacterMovement()->StopMovementImmediately();
				Attack(ComboMap[NextCommand]);
			}
		}
	}

	//当处于攻击时
	if (AttackID != 0)
	{	
		//根据动画播放位置（攻击进行的阶段）触发不同逻辑
		int AnimationPosition = GetAnimationPosition();

		if (AnimationPosition < AttackStartFrame)
		{
			//攻击前摇帧
			if (bBeforeAttackShouldJudge)
			{
				bBeforeAttackShouldJudge = false;
				Skill->JudgeAtBeginAttack(PlayerCharacter);
			}
			Skill->JudgeDuringBeforeAttack(PlayerCharacter);
		}
		else if (AnimationPosition >= AttackStartFrame && AnimationPosition < AttackFinishFrame)
		{
			//攻击进行帧
			if (bInAttackShouldJudge)
			{
				bInAttackShouldJudge = false;
				Skill->JudgeAtStartAttack(PlayerCharacter);
			}
			Skill->JudgeDuringAttack(PlayerCharacter);
		}
		else if (AnimationPosition > AttackFinishFrame)
		{
			//若有指令，进行指令判定
			if (!NextKeyCombation.IsAttackEmpty())
			{
				int NextCommand = NextKeyCombation.GetCommand();

				if (ComboMap.Contains(NextCommand))
				{
					ResetAttack();
					Attack(ComboMap[NextCommand]);
					return;
				}
			}

			//攻击后摇帧
			if (bAfterAttackShouldJudge)
			{
				bAfterAttackShouldJudge = false;
				Skill->JudgeAtFinishAttack(PlayerCharacter);
			}
			Skill->JudgeDuringFinishAttack(PlayerCharacter);
		}
		
	}
}

//初始化
void UPlayerAttackComponent::Setup(APlayerCharacter* NewCharacter)
{
	PlayerCharacter = NewCharacter;
}

//返回是否可以移动
bool UPlayerAttackComponent::IsMovable()
{
	return AttackID == 0 || GetAnimationPosition() >= AttackFinishFrame;
}

//接收下一次攻击组合
void UPlayerAttackComponent::SetKeyCombination(FKeyCombination KeyCombation)
{
	if (IsMovable() && !KeyCombation.IsAttackEmpty())
	{
		NextKeyCombation = KeyCombation;
	}
}

//获得当前攻击动画播放位置
int UPlayerAttackComponent::GetAnimationPosition()
{
	return PlayerCharacter->GetSprite()->GetPlaybackPositionInFrames();
}


//攻击
void UPlayerAttackComponent::Attack(int ID)
{
	//改变状态为攻击
	PlayerCharacter->SetState(EState::Attacking);

	//使动画结束播放的代理得以触发
	PlayerCharacter->GetSprite()->SetLooping(false);

	//设定攻击ID和类型
	AttackID = ID;
	SwitchAttack();

	//初始化攻击资源
	SetupAttack();

	//初始化连续技资源
	SetupCombo();

}

//选择攻击
void UPlayerAttackComponent::SwitchAttack()
{
	switch (AttackID)
	{
	case 1:
		Skill = NewObject<US_AttackI>(PlayerCharacter);
		break;
	case 2:
		Skill = NewObject<US_AttackII>(PlayerCharacter);
		break;
	case 3:
		Skill = NewObject<US_AttackIII>(PlayerCharacter);
		break;
	case 4:
		Skill = NewObject<US_AttackJump>(PlayerCharacter);
		break;
	case 5:
		Skill = NewObject<US_AttackDash>(PlayerCharacter);
		break;
	default:
		break;
	}
}

//载入数据库
sqlite3* UPlayerAttackComponent::LoadDB()
{
	//读取数据库
	FString DBPath = FPaths::Combine(FPaths::ProjectContentDir(), TEXT("Database"), TEXT("Attack.db"));
	if (!FPaths::FileExists(DBPath))
	{
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Missing Skill Database") + FString(DBPath));
		return nullptr;
	}

	//载入数据库
	sqlite3* db;
	sqlite3_open_v2(TCHAR_TO_UTF8(*DBPath), &db, SQLITE_OPEN_READONLY, nullptr);
	if (!db)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), FString("Load Failed"));
		return nullptr;
	}
	return db;
}

//初始化攻击
void UPlayerAttackComponent::SetupAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("Load Attack"));

	//打开数据库
	sqlite3* db = LoadDB();
	if (!db)
	{
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	//初始化技能资源

		//数据读取
#pragma region Load Database

	//查询数据
	sqlite3_stmt* row;
	FString CheckSQL = FString("select * from skill where id = ") + FString::FromInt(AttackID);
	int result = sqlite3_prepare_v2(db, TCHAR_TO_UTF8(*CheckSQL), -1, &row, nullptr);
	if (result != SQLITE_OK)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Check Failed"));
		sqlite3_close_v2(db);
		return;
	}
	if (sqlite3_step(row) != SQLITE_ROW)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("No Data Found"));
		return;
	}

#pragma endregion 

		//取出数据
#pragma region Get Data

	//数据库对应列
	int column_id = 0;
	int column_name = 1;
	int column_flipbook = 2;
	int column_sprite = 3;
	int column_attack_start_frame = 4;
	int column_attack_finish_frame = 5;

	//取出数据
	int attack_start_frame = sqlite3_column_int(row, column_attack_start_frame);
	int attack_finish_frame = sqlite3_column_int(row, column_attack_finish_frame);
	FString flipbook_reference = (const char*)sqlite3_column_text(row, column_flipbook);
	FString sprite_reference = (const char*)sqlite3_column_text(row, column_sprite);

#pragma endregion 

		//载入资源
#pragma region Load Resource

	UPaperFlipbook* AttackFlipbook = LoadObject<UPaperFlipbook>(GetWorld(), *flipbook_reference);
	if (!AttackFlipbook)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Load Flipbook Failed"));
		sqlite3_close_v2(db);
		return;
	}

	UPaperSprite* AttackSprite = LoadObject<UPaperSprite>(GetWorld(), *sprite_reference);
	if (!AttackSprite)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Load Sprite Failed"));
		sqlite3_close_v2(db);
		return;
	}

	//设定攻击动画
	PlayerCharacter->GetSprite()->SetFlipbook(AttackFlipbook);

	//设置当前的判定范围和帧
	SetSprite(AttackSprite);
	AttackStartFrame = attack_start_frame;
	AttackFinishFrame = attack_finish_frame;

#pragma endregion

	//关闭查询
	sqlite3_finalize(row);

	//关闭数据库
	sqlite3_close_v2(db);

}

//初始化连续技
void UPlayerAttackComponent::SetupCombo()
{
	UE_LOG(LogTemp,Warning,  TEXT("Load Combo"));
	//清空连招表
	ComboMap.Empty();
	
	//打开数据库
	sqlite3* db = LoadDB();
	if (!db)
	{
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	//初始化连续技资源

	//数据读取
#pragma region Load Database

	//查询数据
	sqlite3_stmt* row;
	FString CheckSQL = FString("select * from combo where id = ") + FString::FromInt(AttackID);
	int result = sqlite3_prepare_v2(db, TCHAR_TO_UTF8(*CheckSQL), -1, &row, nullptr);
	if (result != SQLITE_OK)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Check Failed"));
		sqlite3_close_v2(db);
		return;
	}

#pragma endregion 

	//取出数据
#pragma region Get Data

	while (sqlite3_step(row) == SQLITE_ROW)
	{
		//数据库对应列
		int column_id = 0;
		int column_command = 1;
		int column_target = 2;

		//取出数据
		int command = sqlite3_column_int(row, column_command);
		int target = sqlite3_column_int(row, column_target);

		ComboMap.Add(command, target);
	}

#pragma endregion 

	//关闭查询
	sqlite3_finalize(row);

	//关闭数据库
	sqlite3_close_v2(db);

}

//重置攻击
void UPlayerAttackComponent::ResetAttack()
{
	AttackID = 0;
	AttackStartFrame = 0;
	AttackFinishFrame = 0;
	bAfterAttackShouldJudge = true;
	bInAttackShouldJudge = true;
	bAfterAttackShouldJudge = true;
	NextKeyCombation.Clear();
	Skill = NewObject<USkill>();
}

