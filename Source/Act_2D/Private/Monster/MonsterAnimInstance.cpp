#include "Monster/MonsterAnimInstance.h"
#include "Monster/Monster.h"
#include "Utils/GlobalBlueprintFunctionLibrary.h"
#include "AnimSequences/Players/PaperZDAnimPlayer.h"

AMonster* UMonsterAnimInstance::GetOwningMonster()
{
	if (!OwingMonster)
	{
		OwingMonster = Cast<AMonster>(GetOwningActor());
	}
	return OwingMonster;
}

EState UMonsterAnimInstance::GetCurrentState()
{
	AMonster* Owner = GetOwningMonster();
	if(Owner)
	{
		return Owner->GetState();
	}
	return EState::None;
}

bool UMonsterAnimInstance::ShouldExitHitState_Implementation()
{
	AMonster* OwnerMonster = GetOwningMonster();
	if (OwnerMonster)
	{
		// Hit��Ĭ�ϲ��˳�
		if (OwnerMonster->GetState() == EState::Hit)
		{
			return false;
		}

		// ���ܻ�״̬����
		if (!OwnerMonster->IsAlive())
		{
			return false;
		}
		return true;
	}
	return false;
}

