// Fill out your copyright notice in the Description page of Project Settings.


#include "PK_SlideSystem.h"
#include "PK_MainCharacter.h"
#include "PK_MovementBehaviour.h"
#include "PK_WallRunSystem.h"
#include "PK_AutoMoveSystem.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UPK_SlideSystem::UPK_SlideSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPK_SlideSystem::BeginPlay()
{
	Super::BeginPlay();
	InitOwner();
}


// Called every frame
void UPK_SlideSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPK_SlideSystem::InitOwner()
{
	owner = Cast<APK_MainCharacter>(GetOwner());
	InitOwnerComponents();
	InitDelegate();
	InitOwnerDelegate();
}

void UPK_SlideSystem::InitOwnerDelegate() 
{
	if (!owner) return;
	owner->OnUpdate()->AddLambda([this](float _tick)
	{
		AdjustHeight(_tick);
		CheckSlideSlope();
		ClampSlideSpeed();
	});
}

void UPK_SlideSystem::InitOwnerComponents()
{
	if (!owner) return;
	targetHeight = owner->GetBaseHeight();
	movementComponent = owner->GetCharacterMovement();
	movementBehaviour = owner->GetMovementBehaviour();
	wallrunSystem = owner->GetWallRunSystem();
	autoMoveSystem = owner->GetAutoMoveSystem();
	capsule = owner->GetCapsuleComponent();
}

void UPK_SlideSystem::InitDelegate() 
{
	onSlideOOBegin.AddLambda([this]()
	{
		targetHeight = slideHeight;
	});

	onSlideOOEnd.AddLambda([this]()
	{
		isSlidingOnObject = false;
		owner->ActivateAllMovements(true);
		if (owner) targetHeight = owner->GetBaseHeight();
	});
}

void UPK_SlideSystem::ModifyConstraints(const float _friction, const float _braking, const float _speed, const bool _enableOtherMovements)
{
	SetSlideFriction(_friction, _braking);
	EnableWallrun(_enableOtherMovements);
	if (!movementBehaviour) return;
	movementBehaviour->SetSpeedY(_speed);
	movementBehaviour->SetCanMoveX(_enableOtherMovements);

}

void UPK_SlideSystem::AdjustPlayerBehaviour()
{
	if(owner) owner->LaunchCharacter(-FVector::UpVector * 100000, false, true);
	if(movementComponent) movementComponent->Velocity = slideDirection * movementComponent->Velocity.Size();
}

void UPK_SlideSystem::AdjustHeight(const float _tick)
{
	if (!owner)return;
	UCapsuleComponent* _capsule = owner->GetCapsuleComponent();
	if (!_capsule) return;
	const float _height = UKismetMathLibrary::Lerp(_capsule->GetScaledCapsuleHalfHeight(), targetHeight, _tick* slidePosSpeed);
	_capsule->SetCapsuleHalfHeight(_height);
}

void UPK_SlideSystem::MoveLateralOnSlide(const float _direction)
{
	if (!owner || !movementComponent || _direction == 0 || !movementComponent->IsMovingOnGround() || !isSlidingDown) return;
	const FVector _impulse = owner->GetActorRightVector() * _direction;
	const float _dot = FVector::DotProduct(_impulse, movementComponent->Velocity);
	const bool _changeDir = _dot <= 0.1f;
	if (_changeDir)
	{
		FVector _saveDirection = FVector::VectorPlaneProject(movementComponent->Velocity, owner->GetActorRightVector());
		movementComponent->Velocity = _saveDirection;
	}
	owner->AddMovementInput(owner->GetActorRightVector(), _direction * lateralSpeed, true);
}

void UPK_SlideSystem::PlayerSlide()
{
	if (!canSlide || !owner || !movementComponent) return;
	const bool _enoughSpeed = owner->GetVelocity().Size() >= minSpeedToSlide;
	if (!_enoughSpeed) return;
	const bool _onGround = movementComponent->IsMovingOnGround();
	if (!_onGround) return;
	InitSlide();
	LaunchPlayerOnSlide();
}

void UPK_SlideSystem::InitSlide() 
{
	if (!owner)return;
	SetTargetHeight(slideHeight);
	rightVectorAtSlide = owner->GetActorRightVector();
	ModifyConstraints(slideFriction, slideBraking, lateralSpeed, false);
	GetDirectionSlide();
}

void UPK_SlideSystem::LaunchPlayerOnSlide()
{
	if (!owner || !movementComponent)return;
	movementComponent->Velocity = slideDirection * movementComponent->Velocity.Size();
	DrawDebugSphere(GetWorld(), owner->GetActorLocation() + slideDirection * movementComponent->Velocity.Size(), 12, 12, FColor::Black, true);
	owner->LaunchCharacter(movementComponent->Velocity * speedSlide - FVector::UpVector * 10000, true, true);
	isSliding = true;
	onSlide.Broadcast();
}

void UPK_SlideSystem::PlayerEndSlide()
{
	if (!owner) return;
	SetTargetHeight(owner->GetBaseHeight());
	ModifyConstraints(owner->GetBaseFriction(), owner->GetBaseBraking(), 1, true);
	isSliding = false;
	movementComponent->MaxWalkSpeed = owner->GetBaseMaxSpeed();
}

void UPK_SlideSystem::CheckForCancelSlide()
{
	if (!isSliding || !owner) return;
	if (movementComponent->IsMovingOnGround() && owner->GetVelocity().Size() >= minSpeedToSlide) return;
	PlayerEndSlide();
}

void UPK_SlideSystem::GetDirectionSlide()
{
	if (!owner) return;
	FHitResult _hit;
	const bool _hitSurfacePlayer = UKismetSystemLibrary::LineTraceSingle(GetWorld(), owner->GetActorLocation(), owner->GetActorLocation() - FVector::UpVector * 1000, ETraceTypeQuery::TraceTypeQuery1, true, owner->GetIgnoredActors(), EDrawDebugTrace::ForOneFrame, _hit, true);
	if (!_hitSurfacePlayer) return;
	normalSlide = _hit.Normal;
	slideDirection = FVector::CrossProduct(rightVectorAtSlide, normalSlide);
	slideDirection.Normalize();
}

void UPK_SlideSystem::SetTargetHeight(const float _target)
{
	targetHeight = _target;
}

void UPK_SlideSystem::CheckSlideSlope()
{
	if (!isSliding) return;
	GetDirectionSlide();
	const float _dot = FVector::DotProduct(FVector::UpVector, slideDirection);
	ApplyPhysicsSlope(_dot, normalSlide);
}

void UPK_SlideSystem::ClampSlideSpeed()
{
	if (!isSliding || !movementComponent) return;
	movementComponent->Velocity = movementComponent->Velocity.Size() > clampMaxSpeed ? movementComponent->Velocity * (clampMaxSpeed / movementComponent->Velocity.Size()) : movementComponent->Velocity;
}

void UPK_SlideSystem::ApplyPhysicsSlope(const float _dot, const FVector _normalSlope)
{
	if (_dot <.01f && _dot > -.01f) return;
	const float _dotSlope = FVector::DotProduct(FVector::UpVector, _normalSlope);
	isSlidingDown = _dot < 0;
	if (isSlidingDown) movementComponent->Velocity *= 1+ (1/(_dotSlope*_dotSlope)) / 100.f;
	else if (_dot > 0) movementComponent->Velocity *= 1 - (1 / (_dotSlope * _dotSlope)) / 75.f;
	movementComponent->MaxWalkSpeed = movementComponent->Velocity.Size();
}

void UPK_SlideSystem::SetSlideFriction(const float _friction, const float _braking)
{
	if (!movementComponent) return;
	movementComponent->GroundFriction = _friction;
	movementComponent->BrakingDecelerationWalking = _braking;
}

void UPK_SlideSystem::EnableMove(const bool _enable)
{
	if (!movementBehaviour) return;
	movementBehaviour->SetCanMove(_enable);
}

void UPK_SlideSystem::EnableWallrun(const bool _enable)
{
	if (!wallrunSystem) return;
	wallrunSystem->SetCanWallRun(_enable);
}

bool UPK_SlideSystem::CheckObstacle() 
{
	if (!owner || isSliding || isSlidingOnObject) return false;
	if (!movementComponent || !movementComponent->IsMovingOnGround())return false;
	if (!capsule) return false;

	FHitResult _hitObject;
	const float _minObject = capsule->GetScaledCapsuleHalfHeight() * minHeightObject;
	const float _maxObject = capsule->GetScaledCapsuleHalfHeight() * maxHeightObject;
	const bool _objectLow = CollectionsLineTraceFromPlayer(_hitObject, _maxObject, _minObject, capsule->GetScaledCapsuleRadius() * maxDistanceToSlide, 5);
	
	FHitResult _hitUp;
	const float _min = capsule->GetScaledCapsuleHalfHeight() * maxHeightObject;
	const float _max = capsule->GetScaledCapsuleHalfHeight() * maxHeightObject + slideHeight * 2;
	const bool _objectUp = CollectionsLineTraceFromPlayer(_hitUp, _max, _min, maxLengthObject, 5);

	if (!_objectLow || _objectUp) return false;
	//const bool _enoughSpeed = movementComponent ? movementComponent->Velocity.Size() > 700 : false;
	/*if(_enoughSpeed)*/ SlideOnObject(_hitObject);
	return true;
}

void UPK_SlideSystem::SlideOnObject(const FHitResult& _hitObject)
{
	if (!owner) return;
	owner->ActivateAllMovements(false);
	const bool _limite = CheckCrossLimit(_hitObject.Location);
	if (_limite) return;
	ExecuteSlideOO(_hitObject.Location);
}

bool UPK_SlideSystem::CheckCrossLimit(const FVector _hitObjectLocation) 
{
	if (!owner)return true;
	FHitResult _limiteHit;
	const FVector _endLineTrace = _hitObjectLocation + owner->GetActorForwardVector() * maxLengthObject - FVector::UpVector * 5;
	const FVector _beginLineTrace = _endLineTrace + FVector::UpVector * 10;
	return UKismetSystemLibrary::LineTraceSingle(GetWorld(), _beginLineTrace, _endLineTrace, ETraceTypeQuery::TraceTypeQuery1, true, owner->GetIgnoredActors(), EDrawDebugTrace::None, _limiteHit, true, FLinearColor::Black);
}

void UPK_SlideSystem::ExecuteSlideOO(const FVector _hitObjectLocation)
{
	if (!owner)return;
	onSlideOnObject.Broadcast();
	isSlidingOnObject = true;
	velocityBeforeSlideOO = movementComponent->Velocity;
	InitAutoMove(CreatePath(_hitObjectLocation));
	owner->LaunchCharacter(FVector(0, 0, 1), false, false);
}

bool UPK_SlideSystem::CollectionsLineTrace(FHitResult& _hitResult, const float _maxDistance, const float _minDistance, const float _referenceHeight, const int _modulo,const FVector _default)const
{
	FHitResult _temp;
	bool _touch = false;
	for (int i = _minDistance; i < _maxDistance; i++)
	{
		if (i % _modulo != 0) continue;
		const bool _hit = LineTrace(_temp, i, _referenceHeight);
		_touch |= _hit;
		if (_hit) _hitResult.Location = _temp.Location;
		if (_touch && !_hit) break;
	}
	if (!_touch) _hitResult.Location = _default;
	return _touch;
}

bool UPK_SlideSystem::CollectionsLineTraceFromPlayer(FHitResult& _hitResult, const float _maxHeight, const float _minHeight, const float _length, const int _modulo) const
{
	bool _isInSight = false;
	for (int i = _maxHeight; i > _minHeight; i--)
	{
		if (i % _modulo != 0) continue;
		bool _hit = LineTraceFromPlayer(_hitResult, i, _length);
		_isInSight |= _hit;
		if (_hit) break;
	}
	return _isInSight;
}

bool UPK_SlideSystem::LineTrace(FHitResult& _hit, const float _distance, const float _referenceHeight)const
{
	if (!owner)return false;
	FVector _endLineTrace = owner->GetActorLocation() + owner->GetActorForwardVector() * _distance;
	_endLineTrace.Z = _referenceHeight;
	FVector _beginLineTrace = owner->GetActorLocation() + owner->GetActorForwardVector() * _distance;
	_beginLineTrace.Z = _referenceHeight + slideHeight;
	return UKismetSystemLibrary::LineTraceSingle(GetWorld(), _beginLineTrace, _endLineTrace, ETraceTypeQuery::TraceTypeQuery1, true, owner->GetIgnoredActors(), EDrawDebugTrace::Persistent, _hit, true);
}

bool UPK_SlideSystem::LineTraceFromPlayer(FHitResult& _hit, const float _height, const float _forward)const
{
	if (!owner)return false;
	const FVector _beginLineTrace = owner->GetActorLocation() + owner->GetActorUpVector() * _height - owner->GetActorForwardVector() * 2;
	const FVector _endLineTrace = _beginLineTrace + owner->GetActorForwardVector() * _forward;
	return UKismetSystemLibrary::LineTraceSingle(GetWorld(), _beginLineTrace, _endLineTrace, ETraceTypeQuery::TraceTypeQuery1, true, owner->GetIgnoredActors(), EDrawDebugTrace::None, _hit, true);
}

void UPK_SlideSystem::InitAutoMove(const TArray<FPK_AutoMovePoint> _path)
{
	if (!autoMoveSystem) return;
	autoMoveSystem->InitPhysics(0, owner->GetBaseBraking(), true);
	autoMoveSystem->SetPath(_path);
	autoMoveSystem->ActivateAutoMove(true);
}

TArray<FPK_AutoMovePoint> UPK_SlideSystem::CreatePath(const FVector _impactPos) const
{
	TArray<FPK_AutoMovePoint> _path = TArray<FPK_AutoMovePoint>();

	FHitResult _firstPointSlide;
	const FVector _firstPoint = CreateFirstPoint(_path, _firstPointSlide,_impactPos);
	CreateSecondPoint(_path, _firstPoint, _firstPointSlide.Location);

	return _path;
}

bool UPK_SlideSystem::GetHitLocation(FHitResult& _hit, const FVector _posHit) const
{
	if (!owner) return false;
	DrawDebugSphere(GetWorld(), _posHit, 6, 12, FColor::Cyan, true);
	const FVector _beginLineTrace = _posHit + FVector::UpVector * slideHeight + owner->GetActorForwardVector() * 2;
	const FVector _endLineTrace = _posHit - FVector::UpVector * 100 + owner->GetActorForwardVector() * 2;
	return UKismetSystemLibrary::LineTraceSingle(GetWorld(), _beginLineTrace, _endLineTrace, ETraceTypeQuery::TraceTypeQuery1, true, owner->GetIgnoredActors(), EDrawDebugTrace::Persistent, _hit, true, FLinearColor::White);
}

FVector UPK_SlideSystem::CreateFirstPoint(TArray<FPK_AutoMovePoint>& _path, FHitResult& _firstPointSlide, const FVector _impactPos) const
{
	if (!owner)return FVector(0);
	const bool _hitPos = GetHitLocation(_firstPointSlide, _impactPos);
	const FVector _firstPoint = owner->GetActorLocation() + FVector::UpVector * owner->GetBaseHeight();
	DrawDebugSphere(GetWorld(), _firstPoint, 12, 12, FColor::Purple, true);
	InitFirstPoint(_path, _firstPoint);
	return _firstPoint;
}

void UPK_SlideSystem::InitFirstPoint(TArray<FPK_AutoMovePoint>& _path, const FVector _firstPoint) const
{
	FPK_AutoMovePoint _firstAutoPoint = FPK_AutoMovePoint();
	_firstAutoPoint.Init(_firstPoint, 1000, 1);
	_firstAutoPoint.onPointReached.AddLambda([this]()
	{
		onSlideOOBegin.Broadcast();
	});
	_path.Add(_firstAutoPoint);
}

void UPK_SlideSystem::CreateSecondPoint(TArray<FPK_AutoMovePoint>& _path, const FVector _firstPoint, const FVector _impactPos) const
{
	if (!owner||!capsule)return;
	FHitResult _secondPointSlide;
	const bool _objectLow = CollectionsLineTrace(_secondPointSlide, maxLengthObject, 0, _impactPos.Z - 10, 5, _firstPoint);
	FVector _secondPoint = _secondPointSlide.Location + FVector::UpVector * slideHeight + owner->GetActorForwardVector() * capsule->GetScaledCapsuleRadius() * 1.5f;
	_secondPoint = FVector(_secondPoint.X, _secondPoint.Y, _firstPoint.Z);
	DrawDebugSphere(GetWorld(), _secondPoint, 12, 12, FColor::Green, true);
	InitSecondPoint(_path, _secondPoint);
}

void UPK_SlideSystem::InitSecondPoint(TArray<FPK_AutoMovePoint>& _path, const FVector _secondPoint) const
{
	FPK_AutoMovePoint _secondAutoPoint = FPK_AutoMovePoint();
	_secondAutoPoint.Init(_secondPoint, speedSlideOnObject, minDistance);
	_secondAutoPoint.onPointReached.AddLambda([this]()
	{
		onSlideOOEnd.Broadcast();
		movementComponent->Velocity = velocityBeforeSlideOO;
	});
	_path.Add(_secondAutoPoint);
}