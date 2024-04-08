#include "Monster/MonsterAnimInstance.h"
#include "Monster/Monster.h"
#include "Utils/GlobalBlueprintFunctionLibrary.h"
#include "AnimSequences/Players/PaperZDAnimPlayer.h"

AMonster* UMonsterAnimInstance::GetOwnerMonster()
{
	if (!OwingMonster)
	{
		OwingMonster = Cast<AMonster>(GetOwningActor());
	}
	return OwingMonster;
}

EState UMonsterAnimInstance::GetCurrentState()
{
	AMonster* Owner = GetOwnerMonster();
	if(Owner)
	{
		return Owner->GetState();
	}
	return EState::None;
}

bool UMonsterAnimInstance::ShouldExitHitState_Implementation()
{
	AMonster* OwnerMonster = GetOwnerMonster();
	if (OwnerMonster)
	{
		// Hit中默认不退出
		if (OwnerMonster->GetState() == EState::Hit)
		{
			return false;
		}

		// 以受击状态死亡
		if (!OwnerMonster->IsAlive())
		{
			return false;
		}
		return true;
	}
	return false;
}

