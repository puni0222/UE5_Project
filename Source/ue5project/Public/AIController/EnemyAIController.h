// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"


UCLASS()
class UE5PROJECT_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAIController();
	
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	
private:
	
	void RandomMove();

	UPROPERTY(EditAnywhere)
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere)
	class UBlackboardData* BlackboardData;
};
