#include "PK_BaseOrbsMovable.h"

#include "Kismet/KismetMathLibrary.h"
#include "Projet_K/Utils/Debug.h"

#pragma region Override
void APK_BaseOrbsMovable::Awake()
{
	Super::Awake();
}

void APK_BaseOrbsMovable::Init()
{
	Super::Init();
	OnUpdate.Add(this, &APK_BaseOrbsMovable::Update);
}


void APK_BaseOrbsMovable::Destroy()
{
	Super::Destroy();
}

void APK_BaseOrbsMovable::OnDrawGizmos()
{
	Super::OnDrawGizmos();
	if (!IsValid())return;
	Debug::DrawLine(GetWorld(),GetStartSplinePosition(), GetEndSplinePosition(), FColor::Red);
	Debug::DrawSphere(GetWorld(), GetStartSplinePosition(), 50,32, FColor::Green);
	Debug::DrawSphere(GetWorld(), GetActorLocation(), 25,32, FColor::Cyan);
	Debug::DrawSphere(GetWorld(), GetEndSplinePosition(), 50,32, FColor::Red);
	const FVector _endLocation = mIsAtEndPosition ? GetStartSplinePosition() : GetEndSplinePosition();
	Debug::DrawLine(GetWorld(), GetActorLocation(), _endLocation, FColor::Red);
}

void APK_BaseOrbsMovable::OnPlayerEnterInTrigger()
{
}

void APK_BaseOrbsMovable::OnPlayerExitInTrigger()
{
}

bool APK_BaseOrbsMovable::IsValid() const
{
	return mStartSpline != nullptr && mEndSpline != nullptr;
}
#pragma endregion Override

#pragma region custom methods
FVector APK_BaseOrbsMovable::GetStartSplinePosition() const
{
	if (!IsValid()) return FVector::ZeroVector;
	return mStartSpline->GetActorLocation();
}

FVector APK_BaseOrbsMovable::GetEndSplinePosition() const
{
	if (!IsValid()) return FVector::ZeroVector;
	return mEndSpline->GetActorLocation();
}

bool APK_BaseOrbsMovable::IsAtPosition(const FVector& _position, float _distance) const
{
	return FVector::Distance(GetActorLocation(), _position) < _distance;
}


void APK_BaseOrbsMovable::Update()
{
	MoveToSpline();
}

void APK_BaseOrbsMovable::MoveToSpline()
{
	if (mStartSpline == nullptr || mEndSpline == nullptr) return;
	const FVector _location = mIsAtEndPosition ? GetStartSplinePosition() : GetEndSplinePosition();
	const FVector _newLocation = UKismetMathLibrary::VInterpTo_Constant(GetActorLocation(), _location, mDeltaTime, mSpeed);
	SetActorLocation(_newLocation);
	if (!mLoop)return;
	const bool _isAtLocation = IsAtPosition(_location, mMinDistance);
	if (_isAtLocation) mIsAtEndPosition = !mIsAtEndPosition;
}
#pragma endregion custom methods
