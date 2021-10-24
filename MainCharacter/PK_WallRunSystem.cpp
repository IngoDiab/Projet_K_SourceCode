// Fill out your copyright notice in the Description page of Project Settings.


#include "PK_WallRunSystem.h"
#include "PK_MainCharacter.h"
#include "PK_MovementBehaviour.h"
#include "PK_GrappleSystem.h"
#include "PK_ClimbSystem.h"
#include "Projet_K/MainCharacter/Camera/PK_Camera.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "Components/CapsuleComponent.h"

// Sets default values for this component's properties
UPK_WallRunSystem::UPK_WallRunSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UPK_WallRunSystem::BeginPlay()
{
	Super::BeginPlay();
	InitOwnerMovement();
}


// Called every frame
void UPK_WallRunSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPK_WallRunSystem::ExitTriggerHitbox()
{
	if (!wasWallrunning || !capsulePlayer)return;
	const float _lengthCollection = capsulePlayer->GetScaledCapsuleRadius() * 3;
	const bool _tooNear = CollectionsLineTrace(wasWallrunningOnRightWall ? 1 : -1, 45, -45, _lengthCollection);
	if (_tooNear)return;
	canWallRunAgain = true;
}

void UPK_WallRunSystem::InitOwnerMovement()
{
	owner = Cast<APK_MainCharacter>(GetOwner());
	InitOwnerComponents();
	InitDelegate();
	InitOwnerDelegate();
}

void UPK_WallRunSystem::GetCamera()
{
	if (!owner) return;
	camera = owner->GetCamera();
}

void UPK_WallRunSystem::InitOwnerDelegate()
{
	if (!owner)return;
	owner->OnUpdate()->AddLambda([this](float _tick)
	{
		onWallUpdateRotation.Broadcast(_tick);
		onWallrunChecks.Broadcast();
		BeginWallrun();
		CheckInitialImpulseTimer(_tick);
		ExitTriggerHitbox();
		CheckResetWallrun();
	});
}

void UPK_WallRunSystem::InitOwnerComponents()
{
	if (!owner) return;
	ownerMovementBehaviour = owner->GetMovementBehaviour();
	ownerMovement = owner->GetCharacterMovement();
	ownerGrappleSystem = owner->GetGrappleSystem();
	ownerClimbSystem = owner->GetClimbSystem();
	capsulePlayer = owner->GetCapsuleComponent();

	FTimerHandle _getCamera;
	GetWorld()->GetTimerManager().SetTimer(_getCamera, this, &UPK_WallRunSystem::GetCamera, .1f, false);
}

void UPK_WallRunSystem::InitDelegate()
{
	onWallrunBegin.AddDynamic(this, &UPK_WallRunSystem::Wallrun);

	onWallrunChecks.AddLambda([this]()
	{
		WallDetectionSystem();
		CheckVelocityOnWall();
		CheckAngleOnWall();
		CheckStayOnWall();
		CheckOnFloor();
	});

	onWallUpdateRotation.AddLambda([this](float _tick)
	{
		ClampView(_tick);
		RotateToTargeted(_tick);
	});

	onWallrunEnd.AddLambda([this]()
	{
		QuitWallRun();
	});
}

void UPK_WallRunSystem::SetConstraints()
{
	if (ownerMovementBehaviour)ownerMovementBehaviour->SetCanMoveY(false);
	if (!ownerMovement) return;
	ownerMovement->MaxWalkSpeed = wallrunSpeed;
	ownerMovement->MaxAcceleration = wallrunAcceleration;
	ownerMovement->GravityScale = activateGravity ? gravity : 0;
	ownerMovement->BrakingDecelerationFalling = 30000;
	ownerMovement->Velocity = FVector(ownerMovement->Velocity.X, ownerMovement->Velocity.Y, 0);
}

void UPK_WallRunSystem::ResetConstraints()
{
	if (ownerMovementBehaviour)ownerMovementBehaviour->SetCanMoveY(true);
	if (!ownerMovement || !owner) return;
	ownerMovement->MaxWalkSpeed = owner->GetBaseMaxSpeed();
	ownerMovement->MaxAcceleration = owner->GetBaseMaxAcceleration();
	ownerMovement->GravityScale = owner->GetBaseGravity();
	ownerMovement->BrakingDecelerationFalling = owner->GetBaseBrakingFalling();
}

void UPK_WallRunSystem::WallDetectionSystem()
{
	if (!canWallRun || !ownerMovement)return;
	const bool _justJump = ownerMovement->Velocity.X == 0 && ownerMovement->Velocity.Y == 0;
	if (_justJump) 
	{
		ResetWallrunOnCancel();
		return;
	}
	if (!isWallRunning) CheckForWallToRunOn();
	else onWallrunBegin.Broadcast();
}

bool UPK_WallRunSystem::StayOnWall(float _way)
{
	if (!capsulePlayer)return false;
	const float _lengthCollection = capsulePlayer->GetScaledCapsuleRadius() * 3;
	if (!isWallRunning || (_way == 1 ? !isWallRunningOnRightWall : !isWallRunningOnLeftWall)) return false;
	return CollectionsLineTrace(_way,100, -80 ,_lengthCollection);
}

bool UPK_WallRunSystem::DetectWall(float _way)
{
	if (!capsulePlayer)return false;
	const float _lengthNonWallrun = capsulePlayer->GetScaledCapsuleRadius() * 1.5;
	const bool _hitLine = CollectionsLineTrace(_way, 0, -45, _lengthNonWallrun);
	if (!_hitLine)return false;
	_way == 1 ? isWallRunningOnRightWall = true : isWallRunningOnLeftWall = true;
	if (!isWallRunning)isAboutToWallrun = true;
	return true;
}

bool UPK_WallRunSystem::CollectionsLineTrace(const float _way, const float _angleBack, const float _angleForward, const float _length)
{
	FHitResult _hitResult;
	bool _isInSight = false;
	FVector _normal = FVector(0);
	for (int i = _angleForward; i < _angleBack; i++)
	{
		bool _hit = LineTraceWallrun(_way, i, _length, _hitResult);
		_isInSight |= _hit;
		if (!_hit) continue;
		_normal = _hitResult.Normal;
		break;
	}
	currentNormalWallRun = _normal;
	return _isInSight;
}

bool UPK_WallRunSystem::LineTraceWallrun(const float _way, const float _offsetForward, const float _length)
{
	FHitResult _hit;
	const FVector _endPos = (owner->GetActorLocation() + _way * (owner->GetActorRightVector().RotateAngleAxis(_way * _offsetForward, owner->GetActorUpVector()) * _length));
	const bool _hitWall = UKismetSystemLibrary::LineTraceSingle(GetWorld(), owner->GetActorLocation(), _endPos, ETraceTypeQuery::TraceTypeQuery1, true, owner->GetIgnoredActors(), EDrawDebugTrace::ForOneFrame, _hit, true);
	return _hitWall && !_hit.Actor->ActorHasTag(FName(TagWallRun)) ;
}

bool UPK_WallRunSystem::LineTraceWallrun(const float _way, const float _offsetForward, const float _length, FHitResult& _hit)
{
	const FVector _endPos = (owner->GetActorLocation() + _way * (owner->GetActorRightVector().RotateAngleAxis(_way*_offsetForward, owner->GetActorUpVector()) * _length));
	const bool _hitWall = UKismetSystemLibrary::LineTraceSingle(GetWorld(), owner->GetActorLocation(), _endPos, ETraceTypeQuery::TraceTypeQuery1, true, owner->GetIgnoredActors(), EDrawDebugTrace::ForOneFrame, _hit, true);
	return _hitWall && !_hit.Actor->ActorHasTag(FName(TagWallRun));
}

void UPK_WallRunSystem::MoveOnWall(const float _direction)
{
	if (!owner || _direction < 0)return;
	owner->AddMovementInput(directionWallRun, _direction, false);
	if (!isInImpulse) return;
	impulseWallRun = UKismetMathLibrary::VInterpTo_Constant(FVector::UpVector, directionWallRun, GetWorld()->DeltaTimeSeconds, timerImpulse/timerMaxImpulse);
	ownerMovement->AddImpulse(impulseWallRun * impulse, true);
}

void UPK_WallRunSystem::BeginWallrun()
{
	if (!canWallRun || !isAboutToWallrun || isOnFloor) return;
	if (ownerGrappleSystem && ownerGrappleSystem->IsGrappling()) return;
	UE_LOG(LogTemp, Warning, TEXT("BEGIN"));
	isWallRunning = true;
	isAboutToWallrun = false;
	SetTargetedRotation(isWallRunningOnLeftWall ? angleWallRun : -angleWallRun);

	if(hasImpulse) isInImpulse = true;
	if (ownerClimbSystem) ownerClimbSystem->SetCanClimb(false);

	if (!owner) return;
	owner->ResetJumpCount();
}

void UPK_WallRunSystem::Wallrun()
{
	if (!isWallRunning) return;
	SetConstraints();
	//const FVector _newDir = isWallRunningOnRightWall ? -FVector::CrossProduct(currentNormalWallRun, FVector::UpVector) : FVector::CrossProduct(currentNormalWallRun, FVector::UpVector);
	directionWallRun = isWallRunningOnRightWall ? -FVector::CrossProduct(currentNormalWallRun, FVector::UpVector) : FVector::CrossProduct(currentNormalWallRun, FVector::UpVector);//UKismetMathLibrary::VInterpTo_Constant(directionWallRun, _newDir, 5, GetWorld()->DeltaTimeSeconds);
}

void UPK_WallRunSystem::QuitWallRun()
{
	if (!isAboutToWallrun && !isWallRunning) return;

	canWallRun = false;
	wasWallrunning = true;
	isWallRunningOnLeftWall ? wasWallrunningOnLeftWall = true : wasWallrunningOnRightWall = true;

	ResetOnQuitWallRun();
}

void UPK_WallRunSystem::ResetOnQuitWallRun() 
{
	if (ownerClimbSystem) ownerClimbSystem->SetCanClimb(true);
	ResetInitialImpulseTimer();
	ResetWallrun();
	ResetRotation();
	ResetConstraints();
}

void UPK_WallRunSystem::ResetWallrun()
{
	currentNormalWallRun = FVector::ZeroVector;
	isWallRunning = false;
	isAboutToWallrun = false;
	isWallRunningOnLeftWall = false;
	isWallRunningOnRightWall = false;
}

void UPK_WallRunSystem::ResetWallrunOnCancel()
{
	if (isWallRunning) return;
	isWallRunningOnLeftWall = false;
	isWallRunningOnRightWall = false;
	isAboutToWallrun = false;
}

void UPK_WallRunSystem::ResetWallrunAfterWallrun()
{
	if (!wasWallrunning) return;
	canWallRun = true;
	wasWallrunning = false;
	wasWallrunningOnLeftWall = false;
	wasWallrunningOnRightWall = false;
	isAboutToWallrun = false;
	isWallRunningOnLeftWall = false;
	isWallRunningOnRightWall = false;
}

void UPK_WallRunSystem::ResetInitialImpulseTimer()
{
	if (ownerMovement) ownerMovement->Velocity = FVector(ownerMovement->Velocity.X, ownerMovement->Velocity.Y, 0);
	isInImpulse = false;
	impulseWallRun = FVector::UpVector;
	timerImpulse = 0;
}

void UPK_WallRunSystem::WallJump()
{
	if (!owner || !isWallRunning || !camera) return;
	const float _dotProductForward = FVector::DotProduct(owner->GetActorForwardVector(), directionWallRun);
	const bool _tooFront = cos(FMath::DegreesToRadians(minAngleJump)) < _dotProductForward && _dotProductForward < 1;
	if (_tooFront) WallJumpTooFront();
	else  owner->LaunchCharacter(camera->GetActorForwardVector() * jumpForward + camera->GetActorUpVector() * jumpUp, true, true);
	onWalljump.Broadcast();
	onWallrunEnd.Broadcast();
}

void UPK_WallRunSystem::WallJumpTooFront()
{
	if (!owner || !camera || !isWallRunning) return;
	const FVector _jumpRight = owner->GetActorRightVector() * (isWallRunningOnLeftWall ? jumpOnY : -jumpOnY);
	const FVector _jumpUp = CalculateUp();
	const FVector _jump = owner->GetActorForwardVector() * jumpOnX + _jumpRight + _jumpUp;
	owner->LaunchCharacter(_jump, false, false);
}

FVector UPK_WallRunSystem::CalculateUp()
{
	if (!owner) return FVector::ZeroVector;
	if (useJumpOnZ) return owner->GetActorUpVector() * jumpOnZ;
	else return CalculateUpWithZ();
}

FVector UPK_WallRunSystem::CalculateUpWithZ()
{
	if (!owner || !camera) return FVector::ZeroVector;
	const FVector _offset = camera->GetActorLocation() + camera->GetActorForwardVector();
	const float _height = _offset.Z - camera->GetActorLocation().Z;
	if (isMinClampZJump && _height < minClampZJump) return FVector::ZeroVector;
	else if (isMaxClampZJump && _height > maxClampZJump) return owner->GetActorUpVector() * jumpUp;
	return owner->GetActorUpVector() * _height * jumpOnZCamera;
}

void UPK_WallRunSystem::CheckAngleOnWall()
{
	if (!fallAngleToWide || !isWallRunning || !owner) return;
	const float _dotProductForward = FVector::DotProduct(isWallRunningOnLeftWall ? owner->GetActorRightVector() : -owner->GetActorRightVector(), currentNormalWallRun);
	const bool _notTooWide = cos(FMath::DegreesToRadians(angleToWide)) < _dotProductForward && _dotProductForward < 1;
	if (currentNormalWallRun == FVector::ZeroVector || _notTooWide) return;
	onWallrunEnd.Broadcast();
}

void UPK_WallRunSystem::CheckForWallToRunOn()
{
	const bool _wallOnRight = DetectWall(1);
	const bool _wallOnLeft = DetectWall(-1);
	if (!_wallOnRight && !_wallOnLeft && isAboutToWallrun) ResetWallrunOnCancel();
}

void UPK_WallRunSystem::CheckResetWallrun()
{
	if (!canWallRunAgain) return;
	ResetWallrunOnCancel();
	ResetWallrunAfterWallrun();
	canWallRunAgain = false;
}

void UPK_WallRunSystem::CheckVelocityOnWall()
{
	if (!fallOnStop || !isWallRunning) return;
	if (ownerMovement->Velocity.Size() <= minSpeedWallRun) onWallrunEnd.Broadcast();
}

void UPK_WallRunSystem::CheckStayOnWall()
{
	if (!StayOnWall(1) && !StayOnWall(-1) && isWallRunning) onWallrunEnd.Broadcast();
}

void UPK_WallRunSystem::CheckOnFloor()
{
	if (!owner)return;
	FHitResult _hit;
	const FVector _finalPos = owner->GetActorLocation() - FVector::UpVector * minDistanceFloor;
	isOnFloor = UKismetSystemLibrary::LineTraceSingle(GetWorld(), owner->GetActorLocation(), _finalPos, ETraceTypeQuery::TraceTypeQuery1, true, owner->GetIgnoredActors(), EDrawDebugTrace::ForOneFrame, _hit, true);
}

void UPK_WallRunSystem::CheckInitialImpulseTimer(const float _tick)
{
	if (!isInImpulse)return;
	if (timerImpulse < timerMaxImpulse) timerImpulse += _tick;
	else ResetInitialImpulseTimer();
}

void UPK_WallRunSystem::ClampView(const float _tick)
{
	if (!isWallRunning || !owner) return;
	if (!AllowRotation()) return;
	const FRotator _current = owner->GetActorRotation();
	const FRotator _clamped = UKismetMathLibrary::FindLookAtRotation(-owner->GetActorForwardVector(), directionWallRun);
	const FRotator _new = UKismetMathLibrary::RInterpTo_Constant(owner->GetActorRotation(), FRotator(_current.Pitch, _clamped.Yaw, _current.Roll), _tick, speedClampAngle);
	owner->SetActorRotation(_new);
}

bool UPK_WallRunSystem::AllowRotation()
{
	if (!owner || !camera)return true;
	const float _dot = FVector::DotProduct(camera->GetActorRightVector(), directionWallRun);
	const bool _needClamp = (isWallRunningOnLeftWall && _dot > 0.12f) || (isWallRunningOnRightWall && _dot < -0.12f);
	return _needClamp;
}

void UPK_WallRunSystem::RotateToTargeted(const float _tick)
{
	if (!owner) return;
	const FRotator _targetRotator = FRotator(owner->GetActorRotation().Pitch, owner->GetActorRotation().Yaw, targetedRoll);
	const FRotator _newCharRot = UKismetMathLibrary::RInterpTo_Constant(owner->GetActorRotation(), _targetRotator, _tick, speedRotation);
	owner->SetActorRotation(_newCharRot);
}

void UPK_WallRunSystem::ResetRotation()
{
	targetedRoll = 0;
}

void UPK_WallRunSystem::SetTargetedRotation(const float _offset)
{
	targetedRoll = _offset;
}