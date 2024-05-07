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
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);

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

void AEnemyCharacter::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	Weapon->SetRenderCustomDepth(true);
}

void AEnemyCharacter::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

int32 AEnemyCharacter::GetPlayerLevel() const
{
	return Level;
}

void AEnemyCharacter::Die()
{
	SetLifeSpan(LifeSpan);
	Super::Die();
}

void AEnemyCharacter::MulticastHandleDeath_Implementation()
{
	Super::MulticastHandleDeath_Implementation();

	FDetachmentTransformRules DetachmentTransformRules = FDetachmentTransformRules::KeepWorldTransform;
	HealthBar->DetachFromComponent(DetachmentTransformRules);
}

void AEnemyCharacter::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;

	if (!HasAuthority()) return;
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

	if (HasAuthority())
	{
		InitializeDefaultAttributes();
		UGameAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent, CharacterClass);
	}
}

void AEnemyCharacter::InitializeDefaultAttributes() const
{
	UGameAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
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
