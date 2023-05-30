// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "EnemySpawnManager.generated.h"

class AEnemyCharacter;

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LICENTARPG_API UEnemySpawnManager : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Enemy Actor")
	TSubclassOf<AEnemyCharacter> EnemyActorClass;

	UPROPERTY(EditAnywhere, Category = "Spawn Points")
	TArray<FVector> SpawnPoints;

	UPROPERTY()
	TArray<AEnemyCharacter*> SpawnedEnemies;
	
public:	
	// Sets default values for this component's properties
	UEnemySpawnManager();

	void RespawnEnemies();
	void SpawnEnemy(const FVector& Location);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


private:
	float const MinRandInterval = -100.f;
	float const HighRandInterval = 100.f;
};
