// Fill out your copyright notice in the Description page of Project Settings.


#include "PK_ClimbSystem.h"
#include "PK_MainCharacter.h"
#include "PK_AutoMoveSystem.h"
#include "PK_WallRunSystem.h"

#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UPK_ClimbSystem::UPK_ClimbSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPK_ClimbSystem::BeginPlay()
{
	Super::BeginPlay();
	InitOwner();
}


// Called every frame
void UPK_ClimbSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPK_ClimbSystem::InitOwner()
{
	owner = Cast<APK_MainCharacter>(GetOwner());
	InitOwnerComponents();
	InitDelegate();
	InitOwnerDelegate();
}

void UPK_ClimbSystem::InitOwnerDelegate() 
{
	if (!owner)return;
	owner->OnUpdate()->AddLambda([this](float _tick)
	{
		onClimbCheck.Broadcast();
	});
}

void UPK_ClimbSystem::InitOwnerComponents()
{
	if (!owner) return;
	autoMove = owner->GetAutoMoveSystem();
	movementComponent = owner->GetCharacterMovement();
	wallRun = owner->GetWallRunSystem();
	capsulePlayer = owner->GetCapsuleComponent();
}

void UPK_ClimbSystem::InitDelegate()
{
	onClimbCheck.AddLambda([this]()
	{
		CheckObstacleToClimb();
	});

	onClimbBegin.AddLambda([this]()
	{

	});

	onClimbEnd.AddLambda([this]()
	{
		isClimbing = false;
		if (owner) owner->ActivateAllMovements(true);
	});
}

void UPK_ClimbSystem::CheckObstacleToClimb()
{
	if (!owner || !capsulePlayer || isClimbing || !canClimbing) return;
	if (!movementComponent || movementComponent->IsMovingOnGround())return;
	FHitResult _hitResultLow;
	FHitResult _hitResultHigh;
	bool _hitLow = CollectionsLineTrace(_hitResultLow, capsulePlayer->GetScaledCapsuleHalfHeight()* maxHeightClimb, 0, capsulePlayer->GetScaledCapsuleRadius() *maxDistanceToClimb, 10);
	bool _hitHigh = CollectionsLineTrace(_hitResultHigh, (capsulePlayer->GetScaledCapsuleHalfHeight() * maxHeightClimb) + capsulePlayer->GetScaledCapsuleHalfHeight()*2, (capsulePlayer->GetScaledCapsuleHalfHeight() * maxHeightClimb), capsulePlayer->GetScaledCapsuleRadius() * (maxDistanceToClimb+2.5f), 10);
	if (_hitLow && !_hitHigh) Climb(*capsulePlayer, _hitResultLow.Location);
}

bool UPK_ClimbSystem::CollectionsLineTrace(FHitResult& _hitResult, const float _maxHeightClimb, const float _minHeightClimb, const float _length, const int _modulo)
{
	bool _isInSight = false;
	for (int i = _maxHeightClimb; i > _minHeightClimb; i--)
	{
		if (i % _modulo != 0) continue;
		bool _hit = LineTrace(_hitResult, i, _length);
		_isInSight |= _hit;
		if (_hit) break;
	}
	return _isInSight;
}

bool UPK_ClimbSystem::LineTrace(FHitResult& _hit, const float _height, const float _forward)
{
	if (!owner)return false;
	const FVector _beginLineTrace = owner->GetActorLocation() + owner->GetActorUpVector() * _height;
	const FVector _endLineTrace = _beginLineTrace + owner->GetActorForwardVector() * _forward;
	return UKismetSystemLibrary::LineTraceSingle(GetWorld(), _beginLineTrace, _endLineTrace, ETraceTypeQuery::TraceTypeQuery1, true, owner->GetIgnoredActors(), EDrawDebugTrace::ForOneFrame, _hit, true);
}

void UPK_ClimbSystem::Climb(const UCapsuleComponent& _ownerCapsule, const FVector _posHit)
{
	if (!owner) return;
	DrawDebugSphere(GetWorld(), _posHit, 6, 12, FColor::Orange, true);
	isClimbing = true;
	owner->ActivateAllMovements(false);
	const TArray<FPK_AutoMovePoint> _path = CreatePath(_ownerCapsule.GetScaledCapsuleRadius()*2, _ownerCapsule.GetScaledCapsuleHalfHeight(), _posHit);
	InitAutoMove(_path);
}

void UPK_ClimbSystem::InitAutoMove(const TArray<FPK_AutoMovePoint> _path)
{
	if (!autoMove) return;
	autoMove->InitPhysics(0, owner->GetBaseBraking(),true);
	autoMove->SetPath(_path);
	autoMove->ActivateAutoMove(true);
}

TArray<FPK_AutoMovePoint> UPK_ClimbSystem::CreatePath(const float _offsetX, const float _offsetZ, const FVector _posHit) const
{
	FHitResult _hit;
	const bool _hitPos = GetHitLocation(_hit, _posHit, _offsetZ*2);
	if (!_hitPos) return TArray<FPK_AutoMovePoint>();

	TArray<FPK_AutoMovePoint> _path = TArray<FPK_AutoMovePoint>();
	InitPath(_path, _hit.Location, 0, _offsetZ);

	return _path;
}

bool UPK_ClimbSystem::GetHitLocation(FHitResult& _hit, const FVector _posHit, const float _offsetZ) const
{
	if (!owner) return false;
	const FVector _beginHit = _posHit + FVector::UpVector * _offsetZ;
	const FVector _endHit = _posHit - FVector::UpVector * 10 + owner->GetActorForwardVector() * 2;
	return UKismetSystemLibrary::LineTraceSingle(GetWorld(), _beginHit, _endHit, ETraceTypeQuery::TraceTypeQuery1, true, owner->GetIgnoredActors(), EDrawDebugTrace::Persistent, _hit, true, FLinearColor::White);
}

void UPK_ClimbSystem::InitPath(TArray<FPK_AutoMovePoint>& _path, const FVector _hit, const float _offsetX, const float _offsetZ) const
{
	if (!owner)return;
	const FVector _firstLocation = FVector(owner->GetActorLocation().X, owner->GetActorLocation().Y, _hit.Z + _offsetZ);
	CreatePathPoints(_path, _firstLocation);

	const FVector _secondLocation = FVector(_hit.X + owner->GetActorForwardVector().X * _offsetX, _hit.Y, _hit.Z + _offsetZ);
	CreatePathPoints(_path, _secondLocation);
	_path.Last().onPointReached.AddLambda([this]()
	{
		onClimbEnd.Broadcast();
	});

	DrawDebugSphere(GetWorld(), _path[0].position, 12, 12, FColor::Red, true);
	DrawDebugSphere(GetWorld(), _path[1].position, 12, 12, FColor::Blue, true);
}

void UPK_ClimbSystem::CreatePathPoints(TArray<FPK_AutoMovePoint>& _path, const FVector _point) const
{
	FPK_AutoMovePoint _pathPoint = FPK_AutoMovePoint();_point;
	_pathPoint.Init(_point, climbSpeed, minDistance);
	_path.Add(_pathPoint);
}

