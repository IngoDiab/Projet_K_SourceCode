// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PK_ClimbSystem.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJET_K_API UPK_ClimbSystem : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY()
	class APK_MainCharacter* owner = nullptr;
	
	UPROPERTY()
	class UPK_AutoMoveSystem* autoMove = nullptr;
	
	UPROPERTY()
	class UCharacterMovementComponent* movementComponent = nullptr;
	
	UPROPERTY()
	class UCapsuleComponent* capsulePlayer = nullptr;

	UPROPERTY()
	class UPK_WallRunSystem* wallRun = nullptr;

protected:
	UPROPERTY(EditAnywhere, Category = "Climb", meta = (DisplayName = "Player can climb"))
	bool canClimbing = true;

	UPROPERTY(VisibleAnywhere, Category = "Climb", meta = (DisplayName = "Player is climbing"), BlueprintReadWrite)
	bool isClimbing = false;

	UPROPERTY(EditAnywhere, Category = "Climb | Speed", meta = (DisplayName = "Speed climb"))
	float climbSpeed = 1000;
	
	UPROPERTY(EditAnywhere, Category = "Climb | Constraints", meta = (DisplayName = "(Multiply by capsuleHeight) Max height player can climb"))
	float maxHeightClimb = 2.f/3.f;
	
	UPROPERTY(EditAnywhere, Category = "Climb | Constraints", meta = (DisplayName = "(Multiply by capsuleRadius) Max distance to climb"))
	float maxDistanceToClimb = 1.2f;

	UPROPERTY(EditAnywhere, Category = "Climb | AutoMove", meta = (DisplayName = "Min distance to set next point in climb"))
	float minDistance = 1;

	DECLARE_EVENT(UPK_ClimbSystem, CheckClimb)
	CheckClimb onClimbCheck;

	DECLARE_EVENT(UPK_ClimbSystem, BeginClimb)
	BeginClimb onClimbBegin;

	DECLARE_EVENT(UPK_ClimbSystem, EndClimb)
	EndClimb onClimbEnd;

public:	
	// Sets default values for this component's properties
	UPK_ClimbSystem();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	FORCEINLINE CheckClimb* OnClimbCheck() { return &onClimbCheck; };
	FORCEINLINE BeginClimb* OnClimbBegin() { return &onClimbBegin; };
	FORCEINLINE EndClimb* OnClimbEnd() { return &onClimbEnd; };
	FORCEINLINE bool GetIsClimbing() {return isClimbing; }
	FORCEINLINE void SetCanClimb(const bool _canClimb) { canClimbing = _canClimb; }

public:
	void InitOwner();

	void InitOwnerDelegate();

	void InitOwnerComponents();

	void InitDelegate();

	void CheckObstacleToClimb();
	bool CollectionsLineTrace(FHitResult& _hit, const float _maxHeightClimb, const float _minHeightClimb, const float _length, const int _modulo);
	bool LineTrace(FHitResult& _hit, const float _height, const float _forward);

	void Climb(const class UCapsuleComponent& _ownerCapsule, const FVector _posHit);

	//void InitPhysics(const float _gravityScale, const bool _stopVelocity = false) const;
	void InitAutoMove(const TArray<struct FPK_AutoMovePoint> _path);
	TArray<FPK_AutoMovePoint> CreatePath(const float _offsetX, const float _offsetZ, const FVector _posHit) const;
	bool GetHitLocation(FHitResult& _hit, const FVector _posHit, const float _offsetZ) const;
	void InitPath(TArray<FPK_AutoMovePoint>& _path, const FVector _hit, const float _offsetX, const float _offsetZ) const;
	void CreatePathPoints(TArray<FPK_AutoMovePoint>& _path, const FVector _point) const;
};
