// Copyright RaulSerranoDev


#include "Character/EnemyCharacter.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Aura/Aura.h"
#include "AbilitySystem/GameAbilitySystemComponent.h"
#include "AbilitySystem/GameAttributeSet.h"
#include "UI/Widget/GameUserWidget.h"
#include "AbilitySystem/GameAbilitySystemLibrary.h"
#include "GameGameplayTags.h"
#include "AI/GameAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

AEnemyCharacter::AEnemyCharacter()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	GetMesh()->MarkRenderStateDirty();
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->MarkRenderStateDirty();

	AbilitySystemComponent = CreateDefaultSubobject<UGameAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	AttributeSet = CreateDefaultSubobject<UGameAttributeSet>("AttributeSet");

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());

	BaseWalkSpeed = 250.f;
}

void AEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!HasAuthority()) return;

	GameAIController = Cast<AGameAIController>(NewController);
	GameAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	GameAIController->RunBehaviorTree(BehaviorTree);
	GameAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);
	GameAIController->GetBlackboardComponent()->SetValueAsBool(FName("RangedAttacker"), CharacterClass != ECharacterClass::Warrior);
}

void AEnemyCharacter::HighlightActor_Implementation()
{
	GetMesh()->SetRenderCustomDepth(true);
	Weapon->SetRenderCustomDepth(true);
}

void AEnemyCharacter::UnHighlightActor_Implementation()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

void AEnemyCharacter::SetMoveToLocation_Implementation(FVector& OutDestination)
{
	// Do not change OutDestination
}

int32 AEnemyCharacter::GetPlayerLevel_Implementation() const
{
	return Level;
}

void AEnemyCharacter::Die(const FVector& DeathImpulse)
{
	SetLifeSpan(LifeSpan);
	if (GameAIController) GameAIController->GetBlackboardComponent()->SetValueAsBool(FName("Dead"), true);

	SpawnLoot();
	Super::Die(DeathImpulse);
}

void AEnemyCharacter::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	CombatTarget = InCombatTarget;
}

AActor* AEnemyCharacter::GetCombatTarget_Implementation() const
{
	return CombatTarget;
}

void AEnemyCharacter::MulticastHandleDeath_Implementation(const FVector& DeathImpulse)
{
	Super::MulticastHandleDeath_Implementation(DeathImpulse);

	FDetachmentTransformRules DetachmentTransformRules = FDetachmentTransformRules::KeepWorldTransform;
	HealthBar->DetachFromComponent(DetachmentTransformRules);
}

void AEnemyCharacter::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;

	if (!HasAuthority() || !GameAIController || !GameAIController->GetBlackboardComponent()) return;

	GameAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), bHitReacting);
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitAbilityActorInfo();

	InitHitReact();
	InitHealthBarWidget();
}

void AEnemyCharacter::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UGameAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
	AbilitySystemComponent->RegisterGameplayTagEvent(TAG_Debuff_Stun, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ThisClass::StunTagChanged);

	if (HasAuthority())
	{
		InitializeDefaultAttributes();
		UGameAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent, CharacterClass);
	}
	OnASCRegistered.Broadcast(AbilitySystemComponent);
}

void AEnemyCharacter::InitializeDefaultAttributes() const
{
	UGameAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}

void AEnemyCharacter::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	Super::StunTagChanged(CallbackTag, NewCount);

	if (!HasAuthority() || !GameAIController || !GameAIController->GetBlackboardComponent()) return;

	GameAIController->GetBlackboardComponent()->SetValueAsBool(FName("Stunned"), bIsStunned);
}

void AEnemyCharacter::InitHitReact()
{
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;

	AbilitySystemComponent->RegisterGameplayTagEvent(TAG_Effects_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(
		this,
		&ThisClass::HitReactTagChanged
	);
}

void AEnemyCharacter::InitHealthBarWidget()
{
	if (UGameUserWidget* UserWidget = Cast<UGameUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		UserWidget->SetWidgetController(this);
	}

	if (const UGameAttributeSet* AS = Cast<UGameAttributeSet>(AttributeSet))
	{
		UAttributeDelegateTypes::BindToAttributeValueChangeDelegate(AbilitySystemComponent, AS->GetHealthAttribute(), &OnHealthChanged);
		UAttributeDelegateTypes::BindToAttributeValueChangeDelegate(AbilitySystemComponent, AS->GetMaxHealthAttribute(), &OnMaxHealthChanged);

		OnHealthChanged.Broadcast(AS->GetHealth());
		OnMaxHealthChanged.Broadcast(AS->GetMaxHealth());
	}
}
