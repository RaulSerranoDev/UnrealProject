// Copyright RaulSerranoDev

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GameAIController.generated.h"

class UBlackboardComponent;
class UBehaviorTreeComponent;

/**
 *
 */
UCLASS()
class AURA_API AGameAIController : public AAIController
{
	GENERATED_BODY()

public:
	AGameAIController();

protected:
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;

};
