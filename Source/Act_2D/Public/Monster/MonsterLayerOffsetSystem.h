#include "CoreMinimal.h"
#include "Utils/Types.h"

// ������Ⱦ�㼶ƫ��ϵͳ����̬���ƹ���㼶��ȷ��������Ⱦ���ȼ�������ͬ�����ʾ���󣬴�MaxOffset���еݼ�
class FMonsterLayerOffsetSystem
{
public:
	static FMonsterLayerOffsetSystem* Get();

	// ��ȡ��һ���㼶
	static int32 GetNextOffset();

	// �黹�㼶
	static void RecycleOffset(int32 Offset);

private:
	FMonsterLayerOffsetSystem();
	static FMonsterLayerOffsetSystem* Instance;

	//�����㼶
	int32 MaxOffset = (TSP_CHARACTER - 1) - TSP_MONSTER;

	// ���յĲ㼶
	TArray<int32> RecycledOffset;

};
