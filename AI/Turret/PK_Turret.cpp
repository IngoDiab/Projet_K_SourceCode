#include "PK_Turret.h"

#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"

// ici
APK_Turret::APK_Turret()
{
	PrimaryActorTick.bCanEverTick = true;
}

bool APK_Turret::IsTargetInZone() const
{
	if (!mTarget) return false;
	return FVector::Distance(GetActorLocation(), mTarget->GetActorLocation()) < mDistanceForTarget;
}

void APK_Turret::BeginPlay()
{
	Super::BeginPlay();
	OnUpdate.Add(this, &APK_Turret::LookAtTarget);
	OnDrawGizmosEvent.Add(this, &APK_Turret::OnDrawGizmos);
}

void APK_Turret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	OnDrawGizmosEvent.Invoke();
	OnUpdate.Invoke(DeltaTime);
}

void APK_Turret::LookAtTarget(float _deltaTime)
{
	if (!IsTargetInZone())return;
	
	const FRotator _rotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), mTarget->GetActorLocation());
	const FRotator _newRotation = UKismetMathLibrary::RInterpTo_Constant(
		GetActorRotation(), _rotation, _deltaTime, mRotationSpeed);
	SetActorRotation(_newRotation);
}

void APK_Turret::OnDrawGizmos()
{
	const bool _isTargetReached = IsTargetInZone();
	const FColor _debugColor = _isTargetReached ? FColor::Green : FColor::Red;
	DrawDebugSphere(GetWorld(), GetActorLocation(), mDistanceForTarget, 32, _debugColor);
	if (!_isTargetReached)return;
	DrawDebugLine(GetWorld(), GetActorLocation(), mTarget->GetActorLocation(), FColor::Red);
}
