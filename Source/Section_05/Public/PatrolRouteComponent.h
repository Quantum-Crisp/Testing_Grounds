// Copyright Oliver Scott 2019

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PatrolRouteComponent.generated.h"

/**
* A "route card" to help AI choose their next waypoint.
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SECTION_05_API UPatrolRouteComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	TArray<AActor*> GetPatrolPoints() const;

private:
	UPROPERTY(EditInstanceOnly, Category = "Patrol Route")
	TArray<AActor*> PatrolPoints;
};
