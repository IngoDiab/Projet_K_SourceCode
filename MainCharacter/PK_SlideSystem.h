// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PK_SlideSystem.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJET_K_API UPK_SlideSystem : public UActorComponent
{
	GENERATED_BODY()

	#pragma region Owner
	UPROPERTY()
		class APK_MainCharacter* owner = nullptr;
	
	UPROPERTY()
		class UCharacterMovementComponent* movementComponent = nullptr;
	
	UPROPERTY()
		class UPK_MovementBehaviour* movementBehaviour = nullptr;

	UPROPERTY()
		class UPK_WallRunSystem* wallrunSystem = nullptr;

	UPROPERTY()
		class UPK_AutoMoveSystem* autoMoveSystem = nullptr;

	UPROPERTY()
		class UCapsuleComponent* capsule = nullptr;
	#pragma endregion

protected:
	UPROPERTY(EditAnywhere, Category = "Slide", meta = (DisplayName = "Player speed when go to slide"))
		float slidePosSpeed = 17;

	UPROPERTY(EditAnywhere, Category = "Slide | Floor", meta = (DisplayName = "Player can slide"))
		bool canSlide = true;

	UPROPERTY(VisibleAnywhere, Category = "Slide | Floor", meta = (DisplayName = "Player is sliding"), BlueprintReadWrite)
		bool isSliding = false;
	
	UPROPERTY(VisibleAnywhere, Category = "Slide | Floor", meta = (DisplayName = "Player is sliding down"), BlueprintReadWrite)
		bool isSlidingDown = false;

	UPROPERTY(EditAnywhere, Category = "Slide | Floor | Speed", meta = (DisplayName = "Minimum speed to allow slide"))
		float minSpeedToSlide = 0;

	UPROPERTY(EditAnywhere, Category = "Slide | Floor | Speed", meta = (DisplayName = "Speed lateral move on slide"))
		float lateralSpeed = 1;
	
	UPROPERTY(EditAnywhere, Category = "Slide | Floor | Speed", meta = (DisplayName = "Intial impulse on slide"))
		float speedSlide = 2;

	UPROPERTY(EditAnywhere, Category = "Slide | Floor | Speed", meta = (DisplayName = "Max speed on slide"))
		float clampMaxSpeed = 2000;

	UPROPERTY(EditAnywhere, Category = "Slide | OnObjects", meta = (DisplayName = "Player can slide on objects"))
		bool canSlideOnObjects = true;

	UPROPERTY(VisibleAnywhere, Category = "Slide | OnObjects", meta = (DisplayName = "Player is sliding on an object"), BlueprintReadWrite)
		bool isSlidingOnObject = false;
	
	UPROPERTY(EditAnywhere, Category = "Slide | OnObjects", meta = (DisplayName = "(Multiply by capsuleHeight) Max object height to slide on"))
		float maxHeightObject = 2.f/3.f;

	UPROPERTY(EditAnywhere, Category = "Slide | OnObjects", meta = (DisplayName = "(Multiply by capsuleHeight) Min object height to slide on"))
		float minHeightObject = -1.f / 3.f;

	UPROPERTY(EditAnywhere, Category = "Slide | OnObjects", meta = (DisplayName = "Max object length to slide on"))
		float maxLengthObject = 500;

	UPROPERTY(EditAnywhere, Category = "Slide | OnObjects", meta = (DisplayName = "(Multiply by capsuleRadius) Max distance to trigger slide"))
		float maxDistanceToSlide = 1.2f;

	UPROPERTY(EditAnywhere, Category = "Slide | OnObjects", meta = (DisplayName = "Speed slide on object"))
		float speedSlideOnObject = 1200;

	UPROPERTY(EditAnywhere, Category = "Slide | OnObjects | AutoMove", meta = (DisplayName = "Min distance to set next point in slide on object"))
		float minDistance = 1;

	UPROPERTY(EditAnywhere, Category = "Slide | Constraints", meta = (DisplayName = "Player height on slide"))
		float slideHeight = 40;

	UPROPERTY(EditAnywhere, Category = "Slide | Constraints", meta = (DisplayName = "Player friction on slide"))
		float slideFriction = .3f;

	UPROPERTY(EditAnywhere, Category = "Slide | Constraints", meta = (DisplayName = "Player braking on slide"))
		float slideBraking = 0;
	
	UPROPERTY(VisibleAnywhere)
		float targetHeight = 0;

	UPROPERTY()
		FVector rightVectorAtSlide = FVector::ZeroVector;

	UPROPERTY()
		FVector slideDirection = FVector::ZeroVector;
	
	UPROPERTY()
		FVector velocityBeforeSlideOO = FVector::ZeroVector;

	UPROPERTY()
		FVector normalSlide = FVector::ZeroVector;

	DECLARE_EVENT(UPK_SlideSystem, SlideOOBegin)
	SlideOOBegin onSlideOOBegin;
	
	DECLARE_EVENT(UPK_SlideSystem, SlideOOEnd)
	SlideOOEnd onSlideOOEnd;

protected:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSlide);
	UPROPERTY(BlueprintAssignable)
		FSlide onSlide;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSlideOnObject);
	UPROPERTY(BlueprintAssignable)
		FSlideOnObject onSlideOnObject;

public:	
	// Sets default values for this component's properties
	UPK_SlideSystem();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	FORCEINLINE FSlide* OnSlide() { return &onSlide; };
	FORCEINLINE FSlideOnObject* OnSlideOnObject() { return &onSlideOnObject; };
	FORCEINLINE bool IsSliding() { return isSliding; };
	FORCEINLINE void SetCanSlide(const bool _canSlide) { canSlide = _canSlide; };
	FORCEINLINE bool IsSlidingOnObject() { return isSlidingOnObject; };
	FORCEINLINE void SetCanSlideOO(const bool _canSlideOO) { canSlideOnObjects = _canSlideOO; };

public:
	void InitOwner();
	void InitOwnerDelegate();
	void InitOwnerComponents();
	void InitDelegate();
	void ModifyConstraints(const float _friction, const float _braking, const float _speed, const bool _enableOtherMovements);
	void AdjustPlayerBehaviour();
	void AdjustHeight(const float _tick);
	void MoveLateralOnSlide(const float _direction);
	void PlayerSlide();
	void InitSlide();
	void LaunchPlayerOnSlide();
	void PlayerEndSlide();
	void CheckForCancelSlide();
	void GetDirectionSlide();
	void SetTargetHeight(const float _target);
	void CheckSlideSlope();
	void ClampSlideSpeed();
	void ApplyPhysicsSlope(const float _dot, const FVector _normalSlope);

	void SetSlideFriction(const float _friction, const float _braking);
	void EnableMove(const bool _enable);
	void EnableWallrun(const bool _enable);
	bool CheckObstacle();
	void SlideOnObject(const FHitResult& _hitObject);
	bool CheckCrossLimit(const FVector _hitObjectLocation);
	void ExecuteSlideOO(const FVector _hitObjectLocation);
	bool CollectionsLineTrace(FHitResult& _hitResult, const float _maxHeight, const float _minHeight, const float _length, const int _modulo, const FVector _default) const;
	bool CollectionsLineTraceFromPlayer(FHitResult& _hitResult, const float _maxHeightClimb, const float _minHeightClimb, const float _length, const int _modulo) const;
	bool LineTrace(FHitResult& _hit, const float _height, const float _forward)const;
	bool LineTraceFromPlayer(FHitResult& _hit, const float _height, const float _forward)const;
	void InitAutoMove(const TArray<struct FPK_AutoMovePoint> _path);
	TArray<FPK_AutoMovePoint> CreatePath(const FVector _impactPos)const;
	bool GetHitLocation(FHitResult& _hit, const FVector _posHit) const;
	FVector CreateFirstPoint(TArray<FPK_AutoMovePoint>& _path, FHitResult& _firstPointSlide, const FVector _impactPos) const;
	void InitFirstPoint(TArray<FPK_AutoMovePoint>& _path, const FVector _firstPoint) const;
	void CreateSecondPoint(TArray<FPK_AutoMovePoint>& _path, const FVector _firstPoint, const FVector _impactPos) const;
	void InitSecondPoint(TArray<FPK_AutoMovePoint>& _path, const FVector _secondPoint) const;
};
