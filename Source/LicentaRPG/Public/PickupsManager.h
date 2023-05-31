// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enums/PickupsEnum.h"
#include "PickupsManager.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LICENTARPG_API UPickupsManager : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Pickups")
	TSubclassOf<AActor> HealthPickupClass;

	UPROPERTY(EditAnywhere, Category = "Pickups")
	TSubclassOf<AActor> ManaPickupClass;

	UPROPERTY(EditAnywhere, Category = "Pickups")
	TMap<FVector, TEnumAsByte<EPickups>> PickupsMap;

public:	
	// Sets default values for this component's properties
	UPickupsManager();

	void RespawnPickups();
	void SpawnPickup(const FVector& Location, EPickups PickupType);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
};
