#pragma once
#include "Act_2DTypes.generated.h"

//基本状态
UENUM(BlueprintType)
enum class EState :uint8
{
	Idle,
	Running,
	Jumping,
	Falling,
	Attacking,
	UnderAttack,
	Sleeping
};


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

	//清空
	void Clear()
	{
		AttackKey = false;
		SpecialKey = false;
		TriggerKey = false;
		JumpKey = false;
		UpKey = false;
		DownKey = false;
		LeftKey = false;
		RighKey = false;
	}

	//判断是否为空
	bool IsAttackEmpty()
	{
		return !(AttackKey | SpecialKey | TriggerKey);
	}

	//获取哈希
	int GetCommand()
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


//攻击类型
UENUM(BlueprintType)
enum class EAttackType : uint8
{
	HarmfulAttack,//伤害型攻击
	StateAttack//状态型攻击
};

//伤害类型
UENUM(BlueprintType)
enum class EAttackHarmfulType : uint8
{
	LightAttack,//轻攻击
	HeavyAttack//重攻击
};

//状态类型
UENUM(BlueprintType)
enum class EAttackStateType : uint8 
{
	Default
};

//攻击属性结构体
USTRUCT(BlueprintType)
struct FAttackProperty
{
	GENERATED_BODY()

	EAttackType AttackType;
	EAttackHarmfulType HarmfulType;
	int Damage;

	EAttackStateType StateType;

	FAttackProperty() {};

	FAttackProperty(EAttackHarmfulType AttackHarmfulType, int AttackDamage)
	{
		AttackType = EAttackType::HarmfulAttack;
		HarmfulType = AttackHarmfulType;
		Damage = AttackDamage;
	}

	FAttackProperty(EAttackStateType AttackStateType)
	{
		AttackType = EAttackType::StateAttack;
		StateType = AttackStateType;
	}

};
