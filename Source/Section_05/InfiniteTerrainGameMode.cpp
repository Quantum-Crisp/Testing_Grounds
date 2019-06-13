// Copyright Oliver Scott 2019

#include "InfiniteTerrainGameMode.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "EngineUtils.h"
#include "ActorPool.h"

AInfiniteTerrainGameMode::AInfiniteTerrainGameMode()
{
	NavMeshBoundsVolumePool = CreateDefaultSubobject<UActorPool>(FName("Nav Mesh Bounds Volume Pool"));
}

void AInfiniteTerrainGameMode::PopulateBoundsVolumePool()
{
	// Creating a pool of actors within our game.
	auto NavMeshIterator = TActorIterator<ANavMeshBoundsVolume>(GetWorld());

	while (NavMeshIterator) //While we are still iterating over all the actors.
	{
		AddToPool(*NavMeshIterator);
		++NavMeshIterator;
	}
}

void AInfiniteTerrainGameMode::AddToPool(ANavMeshBoundsVolume* BoundsVolumeToAdd)
{
	NavMeshBoundsVolumePool->Add(BoundsVolumeToAdd);
}