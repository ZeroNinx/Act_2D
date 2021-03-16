#include "PlayerAttackComponent.h"

//构造函数
UPlayerAttackComponent::UPlayerAttackComponent()
{
	//启用tick
	PrimaryComponentTick.bCanEverTick = true;

	//隐藏攻击模块
	SetVisibility(false);

	SetActive(true);
}

//设定动画组件
void UPlayerAttackComponent::SetFlipbook(UPaperFlipbookComponent* Flipbook)
{
	PlayerFlipbook = Flipbook;
}

//Tick函数
void UPlayerAttackComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	//当判定启用
	if (bShouldJudge&&GetCurrentActionFrame()==NextAttackFrame)
	{
		bShouldJudge = false;
		//UKismetSystemLibrary::PrintString(GetWorld(), FString("Judge"));
		//启用判定
		AttackJudge();
	}
}

//攻击
void UPlayerAttackComponent::Attack(int AttackID)
{

	//////////////////////////////////////////////////////////////////////////
	//数据读取

	//读取数据库
	FString DBPath = FPaths::Combine(FPaths::ProjectContentDir() , TEXT("Database") , TEXT("Skill.db"));
	if (!FPaths::FileExists(DBPath))
	{
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Missing Skill Database") + FString(DBPath));
		return;
	}

	//载入数据库
	sqlite3* db;
	sqlite3_open_v2(TCHAR_TO_UTF8(*DBPath), &db, SQLITE_OPEN_READONLY, nullptr);

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

	/////////////////////////////////////////////////////////////////////////
	//数据处理

	//数据库对应列
	int column_id = 0;
	int column_name = 1;
	int column_attack_frame = 2;
	int column_flipbook = 3;
	int column_sprite = 4;
	int column_combo = 5;

	//取出数据
	int attack_frame = sqlite3_column_int(row, column_attack_frame);
	const char* flipbook = (const char*)sqlite3_column_text(row, column_flipbook);
	const char* sprite = (const char*)sqlite3_column_text(row, column_sprite);

	//初始化攻击
	SetupAttack(flipbook,sprite,attack_frame);

	//关闭查询
	sqlite3_finalize(row);

	//关闭数据库
	sqlite3_close_v2(db);
}


//初始化攻击
void UPlayerAttackComponent::SetupAttack(FString FlipbookReference, FString SpriteReference,int AttackFrame)
{
	UE_LOG(LogTemp, Warning, TEXT("Attack Set"));

	//载入资源
	UPaperFlipbook* AttackFlipbook = LoadObject<UPaperFlipbook>(GetWorld(), *FlipbookReference);
	if (!AttackFlipbook)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Load Flipbook Failed"));
		return;
	}

	UPaperSprite* AttackSprite = LoadObject<UPaperSprite>(GetWorld(), *SpriteReference);
	if (!AttackSprite)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Load Sprite Failed"));
		return;
	}

	//设定攻击动画
	PlayerFlipbook->SetFlipbook(AttackFlipbook);

	//设置当前的判定范围和判定帧
	SetSprite(AttackSprite);
	NextAttackFrame = AttackFrame;

	//设置启用判定
	bShouldJudge = true;
}

//攻击判定
void UPlayerAttackComponent::AttackJudge()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack Judge"));

	//取得重叠的Actor，进行攻击判定
	TSet<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors);	
	for (AActor* Actor : OverlappingActors)
	{
		//判定逻辑
		FString ActorName;
		Actor->GetName(ActorName);
		UKismetSystemLibrary::PrintString(GetWorld(), ActorName);
	}
}

//重置攻击
void UPlayerAttackComponent::ResetAttack()
{
	bShouldJudge = false;
	NextAttackFrame = 0;
	//SetSprite(nullptr);
}


//获得当前攻击动画播放位置
int UPlayerAttackComponent::GetCurrentActionFrame()
{
	return PlayerFlipbook->GetPlaybackPositionInFrames();
}

