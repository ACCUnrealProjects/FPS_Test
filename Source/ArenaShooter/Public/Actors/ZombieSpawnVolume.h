// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZombieSpawnVolume.generated.h"

UCLASS()
class ARENASHOOTER_API AZombieSpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AZombieSpawnVolume();

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* WhereToSpawn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
	float SpawnTimerLow = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
	float SpawnTimerHigh = 1.5f;
	FTimerHandle SpawnTimeHandler;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
	int32 SpawnNumber = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
	uint8 DropChance = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
	bool isSpawning = false;

	int32 ToSpawn;

	FActorSpawnParameters ZombieSpawnParams;

	TArray<TWeakObjectPtr<AActor>> SpawnedEnemies;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<class AZombie> ZombiesToSpawn;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TArray<TSubclassOf<class APickUps>> Drops;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	class UBoxComponent* GetWhereToSpawn() const;

	UFUNCTION(BlueprintPure, Category = "Spawning")
	FVector GetRandomPointInVolume();

	void SpawnZombies();

	bool HaveIFinishedSpawningAndAllDead();

};
