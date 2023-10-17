// Fill out your copyright notice in the Description page of Project Settings.


#include "AIController/EnemyAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

AEnemyAIController::AEnemyAIController()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT(TEXT("/Script/AIModule.BehaviorTree'/Game/Blueprints/AI/BT_Enemy.BT_Enemy'"));
	if (BT.Succeeded())
	{
		BehaviorTree = BT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBD(TEXT("/Script/AIModule.BlackboardData'/Game/Blueprints/AI/BB_Enemy.BB_Enemy'"));
	if (BBD.Succeeded())
	{
		BlackboardData = BBD.Object;
	}
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	UBlackboardComponent* BlackboardComp = Blackboard.Get();
	if (UseBlackboard(BlackboardData, BlackboardComp))
	{
		BlackboardComp->SetValueAsVector(FName(TEXT("HomePos")), InPawn->GetActorLocation());
		if (RunBehaviorTree(BehaviorTree))
		{
			
		}
	}
}

void AEnemyAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

void AEnemyAIController::RandomMove()
{
	auto CurrentPawn = GetPawn();
	if (CurrentPawn == nullptr) return;


	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (NavSystem == nullptr) return;

	FNavLocation RandomLocation;
	if (NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 500.f, RandomLocation))
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, RandomLocation.Location);
	}
}
