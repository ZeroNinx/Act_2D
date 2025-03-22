#include "Monster/MonsterLayerOffsetSystem.h"
#include "Utils/GlobalBlueprintFunctionLibrary.h"

FMonsterLayerOffsetSystem* FMonsterLayerOffsetSystem::Instance = nullptr;

FMonsterLayerOffsetSystem* FMonsterLayerOffsetSystem::Get()
{
	if (!Instance)
	{
		Instance = new FMonsterLayerOffsetSystem();
	}
	return Instance;
}

int32 FMonsterLayerOffsetSystem::GetNextOffset()
{
	int32 NextOffset = 0;

	FMonsterLayerOffsetSystem* Self = Get();
	if (Self->RecycledOffset.IsEmpty())
	{
		NextOffset = Self->MaxOffset;
		Self->MaxOffset--;
	}
	else
	{
		NextOffset = Self->RecycledOffset.Pop();
	}

	//UGlobalBlueprintFunctionLibrary::LogWarning(FString::Printf(TEXT("FMonsterLayerOffsetSystem::GetNextOffset offset = %d"), NextOffset));
	return NextOffset;
}

void FMonsterLayerOffsetSystem::RecycleOffset(int32 Offset)
{
	FMonsterLayerOffsetSystem* Self = Get();
	if (Self->RecycledOffset.Contains(Offset) || Offset <= Self->MaxOffset)
	{
		UGlobalBlueprintFunctionLibrary::LogWarning(FString::Printf(TEXT("FMonsterLayerOffsetSystem::RecycleOffset Offset warning, offset = %d"), Offset));
		return;
	}

	//UGlobalBlueprintFunctionLibrary::LogWarning(FString::Printf(TEXT("FMonsterLayerOffsetSystem::RecycleOffset offset = %d"), Offset));
	Self->RecycledOffset.Push(Offset);
}

FMonsterLayerOffsetSystem::FMonsterLayerOffsetSystem()
{
	
}
