#include "CoreMinimal.h"
#include "Utils/Types.h"

// 怪物渲染层级偏移系统，动态控制怪物层级，确保怪物渲染优先级不会相同造成显示错误，从MaxOffset进行递减
class FMonsterLayerOffsetSystem
{
public:
	static FMonsterLayerOffsetSystem* Get();

	// 获取下一个层级
	static int32 GetNextOffset();

	// 归还层级
	static void RecycleOffset(int32 Offset);

private:
	FMonsterLayerOffsetSystem();
	static FMonsterLayerOffsetSystem* Instance;

	//基础层级
	int32 MaxOffset = (TSP_CHARACTER - 1) - TSP_MONSTER;

	// 回收的层级
	TArray<int32> RecycledOffset;

};
