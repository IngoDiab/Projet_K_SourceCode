// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PK_AutoMoveSystem.generated.h"

USTRUCT(BlueprintType)
struct PROJET_K_API FPK_AutoMovePoint
{
	GENERATED_BODY()

	UPROPERTY()
	FVector position = FVector::ZeroVector;

	UPROPERTY()
	float minDist = 1000;

	UPROPERTY()
	float speedAutoMove = 500;

	DECLARE_EVENT(UPK_AutoMoveSystem, PointReached)
	PointReached onPointReached;

	void Init(const FVector& _pos, const float _speed, const float _minDist) 
	{
		position = _pos;
		speedAutoMove = _speed;
		minDist = _minDist;
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJET_K_API UPK_AutoMoveSystem : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY()
		class APK_MainCharacter* owner = nullptr;

	UPROPERTY()
		class UPK_MovementBehaviour* movementBehaviour = nullptr;

	UPROPERTY()
		class UCharacterMovementComponent* movementCharacter = nullptr;

	UPROPERTY()
		TArray<FPK_AutoMovePoint> path;

	UPROPERTY()
		int currentIndex = 0;


	UPROPERTY(VisibleAnywhere, Category = "AutoMove", meta = (DisplayName = "Player is automoving"))
		bool isAutoMoving = false;

public:	
	// Sets default values for this component's properties
	UPK_AutoMoveSystem();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	FORCEINLINE void ActivateAutoMove(bool _activate) { isAutoMoving = _activate; };

	FORCEINLINE void SetPath(const TArray<FPK_AutoMovePoint> _path) { path = _path; };

public:
	void InitOwner();

	void InitPhysics(const float _gravityScale, const float _braking, const bool _stopVelocity = false) const;

	void AutoMove(const float _tick);
	bool IsAtPos() const;
	void CheckNextPoint();

	UFUNCTION(BlueprintCallable)
	void ResetSystem();
};
