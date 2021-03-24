#pragma once
#include "Act_2DTypes.generated.h"

//组合键结构体
USTRUCT(BlueprintType)
struct FKeyCombination
{
	GENERATED_BODY()

	bool AttackKey = false;
	bool SpecialKey = false;
	bool TriggerKey = false;
	bool JumpKey = false;
	bool UpKey = false;
	bool DownKey = false;
	bool LeftKey = false;
	bool RighKey = false;

	//构造函数
	FKeyCombination() {};
	FKeyCombination
	(
		bool isAttackPressed,
		bool isSpecialPressed,
		bool isTriggerPressed,
		bool isJumpPressed,
		bool isUpPressed,
		bool isDownPressed,
		bool isLeftPressed,
		bool isRightPressed
	)
	{
		AttackKey = isAttackPressed;
		SpecialKey = isSpecialPressed;
		TriggerKey = isTriggerPressed;
		JumpKey = isJumpPressed;
		UpKey = isUpPressed;
		DownKey = isDownPressed;
		LeftKey = isLeftPressed;
		RighKey = isRightPressed;
	}

	//判断是否为空
	bool IsAttackEmpty()
	{
		return !(AttackKey | SpecialKey | TriggerKey);
	}

	//获取哈希
	int GetHash()
	{
		return (TriggerKey << 8) + (SpecialKey << 4) + AttackKey;
	}
};

//Combo哈希
#define Combo1 0x001
#define Combo2 0x010
#define Combo3 0x100
#define Combo4 0x101
#define Combo5 0x110
#define Combo6 0x011
#define Combo7 0x111