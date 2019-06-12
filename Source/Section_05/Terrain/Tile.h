// Copyright Oliver Scott 2019

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

UCLASS()
class SECTION_05_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Generate Tiles")
	void PlaceActors(TSubclassOf<AActor> ToSpawn, int32 MinObjs = 1, int32 MaxObjs = 1, float Radius = 500, float MinScale = 1, float MaxScale = 1);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	
	// Check if sphere collides with other objects in scene, returns true if there is intersecting objects.
	bool CanSpawnAtLocation(FVector Location, float Radius);

	bool FindEmptyLocation(float Radius, FVector& OutLocation);

	void PlaceActor(TSubclassOf<AActor> ToSpawn, FVector SpawnPoint, float YawRotation, float Scale);

};
