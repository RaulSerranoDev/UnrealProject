#pragma once

#include "GameplayEffectTypes.h"
#include "AbilityTypes.generated.h"

USTRUCT(BlueprintType)
struct FGameGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:

	bool IsBlockedHit() const { return bIsBlockedHit; }
	bool IsCriticalHit() const { return bIsCriticalHit; }

	void SetIsCriticalHit(bool bInIsCriticalHit)
	{
		bIsCriticalHit = bInIsCriticalHit;
	}

	void SetIsBlockedHit(bool bInIsBlockedHit)
	{
		bIsBlockedHit = bInIsBlockedHit;
	}

	/** Returns the actual struct used for serialization, subclasses must override this! */
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return StaticStruct();
	}

	/** Creates a copy of this context, used to duplicate for later modifications */
	virtual FGameGameplayEffectContext* Duplicate() const override
	{
		FGameGameplayEffectContext* NewContext = new FGameGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

	/** Custom serialization, subclasses must override this */
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);

protected:

	UPROPERTY()
	bool bIsBlockedHit = false;

	UPROPERTY()
	bool bIsCriticalHit = false;

};

template<>
struct TStructOpsTypeTraits<FGameGameplayEffectContext> : TStructOpsTypeTraitsBase2<FGameGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true,
	};
};
