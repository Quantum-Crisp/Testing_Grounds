// Copyright Oliver Scott 2019

#include "ActorPool.h"

// Sets default values for this component's properties
UActorPool::UActorPool()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

//Allow the BP_Tile to get access to an actor from the pool.
AActor* UActorPool::Checkout()
{
	if (Pool.Num() == 0)
		return nullptr;

	return Pool.Pop();
}

//BP_Tile sends back an actor when it has finished being of use and is stored back in the pool.
void UActorPool::Return(AActor* ActorToReturn)
{
	Add(ActorToReturn);
}

//Adds actors to the pool at the beginning of game.
void UActorPool::Add(AActor* ActorToAdd)
{
	Pool.Push(ActorToAdd);
}
