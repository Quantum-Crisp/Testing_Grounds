// Copyright Oliver Scott 2019

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

class UActorPool;

USTRUCT()
struct FSpawnPosition
{
	GENERATED_USTRUCT_BODY()

	FVector Location;
	float YawRotation;
	float Scale;
};

USTRUCT(BlueprintType)
struct FSpawnVariables
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Parameters")
	int32 MinObjs = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Parameters")
	int32 MaxObjs = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Parameters")
	float Radius = 500;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Parameters")
	float MinScale = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Parameters")
	float MaxScale = 1;
};

UCLASS()
class SECTION_05_API ATile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATile();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Spawning")
		void PlaceActors(TSubclassOf<AActor> ToSpawn, FSpawnVariables SpawnVariables);

	UFUNCTION(BlueprintCallable, Category = "Spawning")
		void PlaceAIPawns(TSubclassOf<APawn> ToSpawn, FSpawnVariables SpawnVariables);

	UFUNCTION(BlueprintCallable, Category = "Pool")
		void SetActorPool(UActorPool* InPool);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// The min and max values of our tile plane.
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		FVector BoxMin;
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		FVector BoxMax;
	UPROPERTY(EditDefaultsOnly, Category = "Navigation")
		FVector NavigationBoundsOffset;
private:
	// Check if sphere collides with other objects in scene, returns true if there is intersecting objects.
	bool CanSpawnAtLocation(FVector Location, float Radius);

	template<class T> //Allows for Pawns and Actors to be handled separately.
	void RandomlyPlaceActors(TSubclassOf<T> ToSpawn, FSpawnVariables SpawnVariables);

	void PlaceActor(TSubclassOf<AActor> ToSpawn, FSpawnPosition SpawnPos);

	void PlaceActor(TSubclassOf<APawn> ToSpawn, FSpawnPosition SpawnPos);

	bool FindEmptyLocation(float Radius, FVector& OutLocation);

	UActorPool* Pool;

	AActor* NavMeshBoundsVolume;

	void PositionNavMeshBoundsVolume();
};
