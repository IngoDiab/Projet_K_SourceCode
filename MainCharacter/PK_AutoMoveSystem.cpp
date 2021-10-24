// Fill out your copyright notice in the Description page of Project Settings.


#include "PK_AutoMoveSystem.h"
#include "PK_MainCharacter.h"
#include "PK_MovementBehaviour.h"
#include "Kismet/KismetMathLibrary.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UPK_AutoMoveSystem::UPK_AutoMoveSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPK_AutoMoveSystem::BeginPlay()
{
	Super::BeginPlay();
	InitOwner();
}


// Called every frame
void UPK_AutoMoveSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPK_AutoMoveSystem::InitOwner()
{
	owner = Cast<APK_MainCharacter>(GetOwner());
	if (!owner) return;
	movementBehaviour = owner->GetMovementBehaviour();
	movementCharacter = owner->GetCharacterMovement();
	owner->OnUpdate()->AddLambda([this](float _tick)
	{
		AutoMove(_tick);
	});
}

void UPK_AutoMoveSystem::InitPhysics(const float _gravityScale, const float _braking, const bool _stopVelocity) const
{
	if (!owner || !movementCharacter) return;
	movementCharacter->GravityScale = _gravityScale;
	movementCharacter->BrakingDecelerationFalling = _braking;
	if (_stopVelocity) movementCharacter->Velocity = FVector(0);
}

void UPK_AutoMoveSystem::AutoMove(const float _tick)
{
	if (!isAutoMoving || !owner) return;

	if (currentIndex >= path.Num()) 
	{
		ResetSystem();
		return;
	}

	owner->ActivateAllMovements(false);
	DrawDebugSphere(GetWorld(), path[currentIndex].position, 20, 20, FColor::Black);
	const FVector _newCharPos = UKismetMathLibrary::VInterpTo_Constant(owner->GetActorLocation(), path[currentIndex].position, _tick, path[currentIndex].speedAutoMove);
	owner->SetActorLocation(_newCharPos);

	CheckNextPoint();
}

bool UPK_AutoMoveSystem::IsAtPos() const
{
	if (currentIndex >= path.Num()) return true;
	return FVector::DistSquared(owner->GetActorLocation(), path[currentIndex].position) < path[currentIndex].minDist;
}

void UPK_AutoMoveSystem::CheckNextPoint()
{
	if (!IsAtPos() || currentIndex >= path.Num()) return;
	path[currentIndex].onPointReached.Broadcast();
	currentIndex++;
}

void UPK_AutoMoveSystem::ResetSystem()
{
	isAutoMoving = false;
	path = TArray<FPK_AutoMovePoint>();
	currentIndex = 0;
	if (!owner) return;
	owner->ActivateAllMovements(true);
	InitPhysics(owner->GetBaseGravity(), owner->GetBaseBrakingFalling());
}

