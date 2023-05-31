// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemySpawnManager.h"
#include "EnemyCharacter.h"


// Sets default values for this component's properties
UEnemySpawnManager::UEnemySpawnManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UEnemySpawnManager::RespawnEnemies()
{
	for (int i = 0; i < SpawnedEnemies.Num(); i++)
	{
		if (SpawnedEnemies[i] != nullptr)
		{
			SpawnedEnemies[i]->Destroy();
		}
		SpawnedEnemies.RemoveAt(i);
	}
	for (int i = 0; i < SpawnPoints.Num(); i++)
	{
		SpawnEnemy(SpawnPoints[i]);
	}
}


void UEnemySpawnManager::SpawnEnemy(const FVector& Location)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	FVector RandomizedLocation = Location;
	RandomizedLocation.X += FMath::RandRange(MinRandInterval, HighRandInterval);
	RandomizedLocation.Y += FMath::RandRange(MinRandInterval, HighRandInterval);

	AEnemyCharacter* Enemy = GetWorld()->SpawnActor<AEnemyCharacter>(EnemyActorClass, RandomizedLocation, FRotator::ZeroRotator, SpawnParams);
	SpawnedEnemies.Add(Enemy);
}

// Called when the game starts
void UEnemySpawnManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UEnemySpawnManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

