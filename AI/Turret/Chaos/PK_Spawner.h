// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Projet_K/Interface/Valid/PK_Valid.h"
#include "Projet_K/Interface/VisualDebug/PK_VisualDebug.h"
#include "Projet_K/Utils/Action/Action.h"

#include "PK_Spawner.generated.h"

UCLASS()
class PROJET_K_API APK_Spawner : public AActor, public IPK_Valid, public IPK_VisualDebug
{
	private:
	GENERATED_BODY()
#pragma region Event
public:
	Action<> OnDrawGizmosEvent = Action<>();
	Action<> OnUpdate = Action<>();
#pragma endregion Event
#pragma region f/p
private:
	/* settings */
	// UPROPERTY(Category="Spawner | Settings", EditAnywhere, meta = (DisplayName = "turret prefab"))
	// TSubclassOf<APK_Turret> mTurretPrefab = nullptr;
	UPROPERTY(Category="Spawner | Settings", EditAnywhere, meta = (DisplayName = "turret spawn location"))
	FVector mTurretSpawnLocation = FVector::ZeroVector;
	UPROPERTY(Category="Spawner | Settings", EditAnywhere, meta = (DisplayName = "distance for spawn chaos", UIMin = 10.0f, UIMax = 2000.0f))
	float mDistanceForSpawn = 100.0f;

	UPROPERTY(Category="Spawner | Target", VisibleAnywhere, meta = (DisplayName = "Target"))
	AActor* mTarget = nullptr;
	
	/* debug */
	UPROPERTY(Category="Spawner | Debug", EditAnywhere, meta = (DisplayName = "Debug valid color"))
	FColor mDebugValidColor = FColor::Green;
	UPROPERTY(Category="Spawner | Debug", EditAnywhere, meta = (DisplayName = "Debug error color"))
	FColor mDebugErrorColor = FColor::Red;
	UPROPERTY(Category="Spawner | Debug", EditAnywhere, meta = (DisplayName = "Debug distance color"))
	FColor mDebugDistanceColor = FColor::Yellow;
	UPROPERTY(Category="Spawner | Debug", EditAnywhere, meta = (DisplayName = "Debug size", UIMin = 0, UIMax = 1000))
	float mDebugSize;
	UPROPERTY(Category="Spawner | Debug", EditAnywhere, meta = (DisplayName = "Debug height", UIMin = 0.1f, UIMax = 500))
	float mDebugHeight;

	/* accessors */
	FVector GetSpawnLocation() const;
	bool TargetInZone() const;
#pragma endregion f/p
#pragma region constructor
public:
	APK_Spawner();
#pragma endregion constructor
#pragma region unreal methods
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void BeginDestroy() override;
protected:
#pragma endregion unreal methods
#pragma region custom methods
public:
	void Init();
	void OnDrawGizmos();
	void CheckForSpawnChaos();
	void SpawnChaos();
#pragma endregion custom methods
#pragma region Override
public:
#pragma region IValid
	virtual bool IsValid() const override;
#pragma endregion IValid
#pragma region IVisualDebug
	virtual FColor GetDebugValidColor() const override;
	virtual FColor GetDebugErrorColor() const override;
	virtual float GetDebugSize() const override;
	virtual float GetDebugHeight() const override;
#pragma endregion IVisualDebug
#pragma endregion Override
};
