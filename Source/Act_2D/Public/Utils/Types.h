#pragma once
#include "Types.generated.h"

// 游戏按键类型
UENUM(BlueprintType)
enum class EGameKeyType: uint8
{
	None	= 0		UMETA(Hidden),
	Up		= 1		UMETA(DisplayName = "上"),
	Down	= 2		UMETA(DisplayName = "下"),
	Left	= 3		UMETA(DisplayName = "左"),
	Right	= 4		UMETA(DisplayName = "右"),
	Jump	= 5		UMETA(DisplayName = "跳跃"),
	Attack	= 6		UMETA(DisplayName = "攻击"),
	Special	= 7		UMETA(DisplayName = "特殊技"),
	Trigger = 8		UMETA(DisplayName = "扳机"),
};

// 组合键结构体
USTRUCT(BlueprintType)
struct FKeyCombination
{
	GENERATED_BODY()

	// Key列表
	UPROPERTY(EditAnywhere)
	TSet<EGameKeyType> Keys;

	// 是否为空
	bool IsEmpty()
	{
		return Keys.IsEmpty();
	}

	// 是否包含按键类型
	bool ContainsKey(EGameKeyType KeyType)
	{
		return Keys.Contains(KeyType);
	}

	// 是否包含按键组合
	bool ContainsCombination(FKeyCombination& Combination)
	{
		for (auto KeyType : Combination.Keys)
		{
			if (!this->Keys.Contains(KeyType))
			{
				return false;
			}
		}
		return true;
	}

	// 可以哈希作为Map的Key
	friend uint32 GetTypeHash(const FKeyCombination& KeyCombination)
	{
		return GetTypeHash(KeyCombination.Keys.Array());
	}

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
		return AttackKey + (SpecialKey << 1) + (TriggerKey << 2);
	}
};


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
