// Copyright RaulSerranoDev


#include "Character/CharacterBase.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

#include "AbilitySystem/GameAbilitySystemComponent.h"
#include "Aura/Aura.h"
#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include "GameGameplayTags.h"

ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetCapsuleComponent()->SetWorldLocation(FVector(0, 0, GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_EffectActor, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), "WeaponHandSocket");
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	BurnDebuffComponent = CreateDefaultSubobject<UDebuffNiagaraComponent>("BurnDebuffComponent");
	BurnDebuffComponent->SetupAttachment(GetRootComponent());
	BurnDebuffComponent->DebuffTag = TAG_Debuff_Burn;

	StunDebuffComponent = CreateDefaultSubobject<UDebuffNiagaraComponent>("StunDebuffComponent");
	StunDebuffComponent->SetupAttachment(GetRootComponent());
	StunDebuffComponent->DebuffTag = TAG_Debuff_Stun;
}

void ACharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACharacterBase, bIsStunned);
	DOREPLIFETIME(ACharacterBase, bIsBurned);
}

UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

FVector ACharacterBase::GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag)
{
	FVector SocketLocation;
	if ((IsValid(Weapon) && GetSocketLocationOnMesh(MontageTag, WeaponTagNameSocketMap, Weapon, SocketLocation))
		|| GetSocketLocationOnMesh(MontageTag, MeshTagNameSocketMap, GetMesh(), SocketLocation))
	{
		return SocketLocation;
	}
	return FVector();
}

UAnimMontage* ACharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

bool ACharacterBase::IsDead_Implementation() const
{
	return bDead;
}

AActor* ACharacterBase::GetAvatar_Implementation()
{
	return this;
}

TArray<FTaggedMontage> ACharacterBase::GetAttackMontages_Implementation()
{
	return AttackMontages;
}

UNiagaraSystem* ACharacterBase::GetBloodEffect_Implementation(bool bIsCritical)
{
	return bIsCritical ? CriticalBloodEffect : BloodEffect;
}

FTaggedMontage ACharacterBase::GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag)
{
	for (FTaggedMontage TaggedMontage : AttackMontages)
	{
		if (TaggedMontage.MontageTag == MontageTag)
		{
			return TaggedMontage;
		}
	}
	return FTaggedMontage();
}

int32 ACharacterBase::GetMinionCount_Implementation()
{
	return MinionCount;
}

void ACharacterBase::AddMinionCount_Implementation(int32 Amount)
{
	MinionCount += Amount;
}

ECharacterClass ACharacterBase::GetCharacterClass_Implementation() const
{
	return CharacterClass;
}

USkeletalMeshComponent* ACharacterBase::GetWeapon_Implementation() const
{
	return Weapon;
}

void ACharacterBase::Die(const FVector& DeathImpulse)
{
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));

	MulticastHandleDeath(DeathImpulse);
}

FOnASCRegistered& ACharacterBase::GetOnASCRegisteredDelegate()
{
	return OnASCRegistered;
}

FOnDeath& ACharacterBase::GetOnDeathDelegate()
{
	return OnDeath;
}

void ACharacterBase::MulticastHandleDeath_Implementation(const FVector& DeathImpulse)
{
	UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation(), GetActorRotation());

	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	Weapon->AddImpulse(DeathImpulse * Weapon->GetMass());

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	GetMesh()->AddImpulse(DeathImpulse * GetMesh()->GetMass());

	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	Dissolve();

	bDead = true;
	OnDeath.Broadcast(this);
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

}

void ACharacterBase::InitAbilityActorInfo()
{
}

void ACharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
}

void ACharacterBase::InitializeDefaultAttributes() const
{
}

void ACharacterBase::AddCharacterAbilities()
{
	if (!HasAuthority()) return;

	UGameAbilitySystemComponent* ASC = CastChecked<UGameAbilitySystemComponent>(AbilitySystemComponent);
	ASC->AddCharacterAbilities(StartupAbilities);
	ASC->AddCharacterPassiveAbilities(StartupPassiveAbilities);
}

void ACharacterBase::Dissolve()
{
	if (IsValid(DissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
		GetMesh()->SetMaterial(0, DynamicMatInst);
		StartDissolveTimeline(DynamicMatInst);
	}
	if (IsValid(WeaponDissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInstance, this);
		Weapon->SetMaterial(0, DynamicMatInst);
		StartWeaponDissolveTimeline(DynamicMatInst);
	}
}

void ACharacterBase::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bIsStunned = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bIsStunned ? 0.f : BaseWalkSpeed;
}

void ACharacterBase::OnRep_Stunned()
{
}

void ACharacterBase::OnRep_Burned()
{
}

bool ACharacterBase::GetSocketLocationOnMesh(const FGameplayTag& MontageTag, const TMap<FGameplayTag, FName>& SocketMeshMap, const USkeletalMeshComponent* SocketMesh, FVector& Location) const
{
	for (auto TagNamePair : SocketMeshMap)
	{
		if (TagNamePair.Key.MatchesTagExact(MontageTag))
		{
			Location = SocketMesh->GetSocketLocation(TagNamePair.Value);
			return true;
		}
	}
	return false;
}
