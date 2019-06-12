// Copyright Oliver Scott 2019

#include "Tile.h"
#include "WorldCollision.h"
#include "DrawDebugHelpers.h"


// Sets default values
ATile::ATile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();


}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Function to place assets into tiles.
void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int32 MinObjs, int32 MaxObjs, float Radius, float MinScale, float MaxScale)
{
	// Number of props to spawn randomly chosen between min and max.
	int32 NumToSpawn = FMath::RandRange(MinObjs, MaxObjs);

	for (int32 i = 0; i < NumToSpawn; i++)
	{
		FVector SpawnPoint;
		float RandScale = FMath::RandRange(MinScale, MaxScale);
		bool bFound = FindEmptyLocation(Radius * RandScale, SpawnPoint);
		if (bFound)
		{
			float RandRotation = FMath::RandRange(-180.f, 180.f);
			PlaceActor(ToSpawn, SpawnPoint, RandRotation, RandScale);
		}
			
	}
}

bool ATile::FindEmptyLocation(float Radius, FVector& OutLocation)
{
	// The min and max values of our tile plane.
	FVector BoxMin = FVector(0.0f, -2000.f, 0.0f);
	FVector BoxMax = FVector(4000.0f, 2000.f, 0.f);
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

void ATile::PlaceActor(TSubclassOf<AActor> ToSpawn, FVector SpawnPoint, float YawRotation, float Scale)
{
	AActor* SpawnedObj = GetWorld()->SpawnActor(ToSpawn); //Spawn the desired actor
	SpawnedObj->SetActorRelativeLocation(SpawnPoint); //Set the location of said actor in the randomly generated position
	SpawnedObj->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false)); //Attach the actor to the tile.
	SpawnedObj->SetActorRotation(FRotator(0.f, YawRotation, 0.f)); //Apply the random amount of rotation
	SpawnedObj->SetActorScale3D(FVector(Scale)); // Apply the random scale to the object.
}

bool ATile::CanSpawnAtLocation(FVector Location, float Radius)
{
	FHitResult HitResult; //Empty but needed as out parameter for sweep function.

	FVector GlobalLocation = ActorToWorld().TransformPosition(Location); //Convert local location to global location.

	bool HasHit = GetWorld()->SweepSingleByChannel(HitResult, GlobalLocation, GlobalLocation,
		FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel2, FCollisionShape::MakeSphere(Radius));

	return !HasHit;
}
