// Copyright Oliver Scott 2019

#include "Tile.h"
#include "WorldCollision.h"
#include "DrawDebugHelpers.h"
#include "ActorPool.h"
#include "AI/NavigationSystemBase.h"

// Sets default values
ATile::ATile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxMin = FVector(0.0f, -2000.f, 0.0f);
	BoxMax = FVector(4000.0f, 2000.f, 0.f);
	NavigationBoundsOffset = FVector(2000.f, 0.f, 0.f);
}
// Sets the actor pool, and then positions the nav meshes.
void ATile::SetActorPool(UActorPool* InPool)
{
	Pool = InPool;
	PositionNavMeshBoundsVolume();
}
// Applies the position of nav mesh to the world and applies its offest to center it within the tile.
void ATile::PositionNavMeshBoundsVolume()
{
	NavMeshBoundsVolume = Pool->Checkout(); //Take a nav mesh out of the actor pool.
	if (NavMeshBoundsVolume == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] Not enough actors in pool"), *GetName())
			return;
	}
	NavMeshBoundsVolume->SetActorLocation(GetActorLocation() + NavigationBoundsOffset); //Set its position.
	FNavigationSystem::Build(*GetWorld()); // "Re-build" the nav mash.
}
// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();

}
// Called before destruction of object.
void ATile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if(Pool != nullptr && NavMeshBoundsVolume != nullptr)
		Pool->Return(NavMeshBoundsVolume);
}
// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
// Function to place actors and AI into tiles (Called from the Blueprint).
void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, FSpawnVariables SpawnVariables)
{
	RandomlyPlaceActors(ToSpawn, SpawnVariables);
}
void ATile::PlaceAIPawns(TSubclassOf<APawn> ToSpawn, FSpawnVariables SpawnVariables)
{
	RandomlyPlaceActors(ToSpawn, SpawnVariables);
}
//Function to generate an array of Random Positions within the tile.
template<class T>
void ATile::RandomlyPlaceActors(TSubclassOf<T> ToSpawn, FSpawnVariables SpawnVariables)
{
	// Number of props to spawn randomly chosen between min and max.
	int32 NumToSpawn = FMath::RandRange(SpawnVariables.MinObjs, SpawnVariables.MaxObjs);

	for (int32 i = 0; i < NumToSpawn; i++)
	{
		FSpawnPosition SpawnPos;
		SpawnPos.Scale = FMath::RandRange(SpawnVariables.MinScale, SpawnVariables.MaxScale);
		bool bFound = FindEmptyLocation(SpawnVariables.Radius *SpawnPos.Scale, SpawnPos.Location);
		if (bFound)
		{
			SpawnPos.YawRotation = FMath::RandRange(-180.f, 180.f);
			PlaceActor(ToSpawn, SpawnPos);
		}
	}
}
// Function that places the actor within the scene, and handles its variables / properties.
void ATile::PlaceActor(TSubclassOf<AActor> ToSpawn, FSpawnPosition SpawnPos)
{
	AActor* SpawnedObj = GetWorld()->SpawnActor(ToSpawn); //Spawn the desired actor
	if (SpawnedObj)
	{
		SpawnedObj->SetActorRelativeLocation(SpawnPos.Location); //Set the location of said actor in the randomly generated position
		SpawnedObj->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false)); //Attach the actor to the tile.
		SpawnedObj->SetActorRotation(FRotator(0.f, SpawnPos.YawRotation, 0.f)); //Apply the random amount of rotation
		SpawnedObj->SetActorScale3D(FVector(SpawnPos.Scale)); // Apply the random scale to the object.
	}
}
// Function that places the AI within the scene, and handles its variables / properties.
void ATile::PlaceActor(TSubclassOf<APawn> ToSpawn, FSpawnPosition SpawnPos)
{
	FRotator Rotation = FRotator(0.f, SpawnPos.YawRotation, 0.f);
	APawn* SpawnedObj = GetWorld()->SpawnActor<APawn>(ToSpawn, SpawnPos.Location, Rotation); //Spawn the desired actor
	if (SpawnedObj)
	{
		SpawnedObj->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false)); //Attach the actor to the tile.
		SpawnedObj->SpawnDefaultController(); //Attach AI Controller to it.
		SpawnedObj->Tags.Add(FName("Enemy")); //Add appropriate tags to AI.
	}
}
// Function that attempts to find an empty location within the tile to spawn an actor.
bool ATile::FindEmptyLocation(float Radius, FVector& OutLocation)
{
	// Create a bounding box based off of these points.
	FBox BoundBox(BoxMin, BoxMax);

	const int32 MAX_ATTEMPTS = 100;
	for (int32 i = 0; i < MAX_ATTEMPTS; i++)
	{
		FVector CandidatePoint = FMath::RandPointInBox(BoundBox); //Choose a random point from within the box.
		if (CanSpawnAtLocation(CandidatePoint, Radius))
		{
			OutLocation = CandidatePoint;
			return true;
		}
	}
	// We found no valid location within 100 iterations.
	return false;
}
//Function that checks projects a boundary sphere into the world to check if other objects have spawned within it.
bool ATile::CanSpawnAtLocation(FVector Location, float Radius)
{
	FHitResult HitResult; //Empty but needed as out parameter for sweep function.

	FVector GlobalLocation = ActorToWorld().TransformPosition(Location); //Convert local location to global location.

	bool HasHit = GetWorld()->SweepSingleByChannel(HitResult, GlobalLocation, GlobalLocation,
		FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel2, FCollisionShape::MakeSphere(Radius));

	return !HasHit;
}
