// Copyright Oliver Scott 2019

#include "Tile.h"

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
void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int32 MinObjs, int32 MaxObjs)
{
	// The min and max values of our tile plane.
	FVector BoxMin = FVector(0.0f, -2000.f, 0.0f);
	FVector BoxMax = FVector(4000.0f, 2000.f, 0.f);
	// Create a bounding box based off of these points.
	FBox BoundBox(BoxMin, BoxMax);

	// Number of props to spawn randomly chosen between min and max.
	int32 NumToSpawn = FMath::RandRange(MinObjs, MaxObjs);

	for (int32 i = 0; i < NumToSpawn; i++)
	{	
		FVector RandLocation = FMath::RandPointInBox(BoundBox);	//Create a random point from within the box.
		AActor* SpawnedObj = GetWorld()->SpawnActor(ToSpawn); //Spawn the desired actor
		SpawnedObj->SetActorRelativeLocation(RandLocation); //Set the location of said actor in the randomly generated position
		SpawnedObj->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false)); //Attach the actor to the tile.
	}

}
