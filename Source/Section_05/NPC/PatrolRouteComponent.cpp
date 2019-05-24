// Copyright Oliver Scott 2019

#include "PatrolRouteComponent.h"

TArray<AActor*> UPatrolRouteComponent::GetPatrolPoints() const
{
	return PatrolPoints;
}