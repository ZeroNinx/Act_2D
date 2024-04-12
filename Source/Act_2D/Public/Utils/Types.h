#pragma once
#include "Types.generated.h"

// 游戏按键类型
UENUM(BlueprintType)
enum class EGameKeyType: uint8
{
	// 注意，不要超过31位
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

	//构造函数
	FKeyCombination() {};
	FKeyCombination(const TArray<EGameKeyType>& NewKeys)
	{
		for (auto Key : NewKeys)
		{
			AddKey(Key);
		}
	};

	// Key列表
	UPROPERTY(EditDefaultsOnly)
	TSet<EGameKeyType> Keys;

	UMETA(Hidden)
	uint32 KeyHash = 0; // 为了作为Map的Key快速GetHash, 同时无视Key的顺序，存一个按键组合

	// 添加按键
	void AddKey(EGameKeyType NewKey)
	{
		uint8 KeyNumber = uint8(NewKey);
		if (KeyNumber >= 32)
		{
			UE_LOG(LogTemp, Warning, TEXT("Key is Larger than 32!!!"));
		}
		Keys.Add(NewKey);
		KeyHash |= (1 << KeyNumber);
	}

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
	friend bool operator==(const FKeyCombination& KeyA, const FKeyCombination& KeyB)
	{
		return GetTypeHash(KeyA) == GetTypeHash(KeyB);
	}

	friend uint32 GetTypeHash(const FKeyCombination& KeyCombination)
	{
		if (KeyCombination.KeyHash != 0)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Get Hash"));
			return KeyCombination.KeyHash;
		}

		// 计算Hash
		uint32 Hash = 0;
		for (auto& Key : KeyCombination.Keys)
		{
			uint8 KeyNumber = uint8(Key);
			if (KeyNumber >= 32)
			{
				UE_LOG(LogTemp, Warning, TEXT("Key is Larger than 32!!!"));
			}
			Hash |= (1 << KeyNumber);
		}
		//UE_LOG(LogTemp, Warning, TEXT("Compute Hash"), Hash);
		return Hash;
	}
};

//技能类型
UENUM(BlueprintType)
enum class ESkillType : uint8
{
	HarmfulAttack	UMETA(DisplayName = "伤害类"),
};

//伤害类型
UENUM(BlueprintType)
enum class EAttackHarmfulType : uint8
{
	LightAttack		UMETA(DisplayName = "轻攻击"),
	HeavyAttack		UMETA(DisplayName = "重攻击"),
};

//技能属性结构体
USTRUCT(BlueprintType)
struct FSkillProperty
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, meta = (DisplayName = "技能类型"))
	ESkillType SkillType;

	UPROPERTY(EditDefaultsOnly, meta = (DisplayName = "伤害类型"))
	EAttackHarmfulType HarmfulType;

	UPROPERTY(EditDefaultsOnly, meta = (DisplayName = "伤害数值"))
	int32 Damage = 0;

	UPROPERTY(EditDefaultsOnly, meta = (DisplayName = "持续性技能"))
	bool bPersistent = false;

	UPROPERTY(EditDefaultsOnly, meta = (DisplayName = "启用在Tick中击中每个Actor一次", Category = "Tick设置"))
	bool bEnableExecuteHitOnTick = false;

	UPROPERTY(EditDefaultsOnly, meta = (DisplayName = "Judge前的Tick击中每个Actor", Category = "Tick设置"))
	bool ExecuteHitOnTickBeforeJudge = false;

	UPROPERTY(EditDefaultsOnly, meta = (DisplayName = "Judge时的Tick击中每个Actor", Category = "Tick设置"))
	bool ExecuteHitOnJudgeTick = false;

	UPROPERTY(EditDefaultsOnly, meta = (DisplayName = "Judge后的Tick击中每个Actor", Category = "Tick设置"))
	bool ExecuteHitOnTickAfterJudge = false;

	FSkillProperty() {};
	FSkillProperty(EAttackHarmfulType AttackHarmfulType, int AttackDamage)
	{
		SkillType = ESkillType::HarmfulAttack;
		HarmfulType = AttackHarmfulType;
		Damage = AttackDamage;
	}
};
