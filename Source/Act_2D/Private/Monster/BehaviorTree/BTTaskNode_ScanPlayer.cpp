// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_ScanPlayer.h"

//执行函数
UBTTaskNode_ScanPlayer::UBTTaskNode_ScanPlayer()
{
	
}

//执行任务
EBTNodeResult::Type UBTTaskNode_ScanPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//获取控制器
	AMonsterController* Controller = Cast<AMonsterController>(OwnerComp.GetAIOwner());
	if (!Controller)
	{
		return EBTNodeResult::Failed;
	}

	//计算玩家和怪物的距离
	FVector PlayerLocation = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation();
	FVector MonsterLocation = Controller->GetPawn()->GetActorLocation();
	float DurationToPlayer = UKismetMathLibrary::Vector_Distance(PlayerLocation, MonsterLocation);

	//根据距离改变标记
	EDistance DistanceMark;
	if (DurationToPlayer > MaxProper)
	{
		DistanceMark = EDistance::Far;
	}
	else if(DurationToPlayer > MaxClose)
	{
		DistanceMark = EDistance::Proper;
	}
	else
	{
		DistanceMark = EDistance::Close;
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(Controller->DistanceToPlayerID, (uint8)DistanceMark);



	return EBTNodeResult::Succeeded;
}
