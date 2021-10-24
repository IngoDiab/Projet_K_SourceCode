#include "PK_OrbsOfDestruction.h"

#include "Projet_K/AI/Bullet/PK_OrbsBullet.h"
#include "Projet_K/Utils/Debug.h"

#pragma region Accessors

TArray<AActor*> APK_OrbsOfDestruction::GetPoints() const
{
	return mPoints;
}

void APK_OrbsOfDestruction::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	mOrbsShoot.Update(DeltaSeconds);
}
#pragma endregion Accessors

#pragma region Override

bool APK_OrbsOfDestruction::IsValid() const
{
	return Super::IsValid() && mBulletPrefab != nullptr && mCurrentIndexPoints < mPoints.Num();
}


bool APK_OrbsOfDestruction::CanShoot()
{
	if (!IsValid() || !mPlayerHasPassedTrigger || mPoints[mCurrentIndexPoints] == nullptr)
		return false;
	if (mOrbsShoot.GetCurrentCooldown() > 0.0f) return false;
	return true;
}

void APK_OrbsOfDestruction::Shoot()
{
	Super::Shoot();
	if (!CanShoot()) return;
	if (mCurrentIndexPoints == mPoints.Num()-1)
		OnUpdate.Remove(this, &APK_OrbsOfDestruction::Shoot);
	
	SpawnBullet();
	OnShoot();
}

void APK_OrbsOfDestruction::SpawnBullet()
{
	APK_OrbsBullet* _bullet = GetWorld()->SpawnActorDeferred<APK_OrbsBullet>(mBulletPrefab, GetActorTransform());
	if (!_bullet) return;
	_bullet->SetEndLocation(mPoints[mCurrentIndexPoints]->GetActorLocation());
	_bullet->FinishSpawning(GetActorTransform());
	mCurrentIndexPoints++;
	if (mLoopPoints && mCurrentIndexPoints == mPoints.Num() -1) mCurrentIndexPoints = 0;
	mOrbsShoot.SetCurrentCooldown(mOrbsShoot.GetCooldown());
}

void APK_OrbsOfDestruction::OnDamage()
{
	Super::OnDamage();
	GetWorld()->DestroyActor(this);
}

void APK_OrbsOfDestruction::OnPlayerEnterInTrigger()
{
	Super::OnPlayerEnterInTrigger();
	mPlayerHasPassedTrigger = true;
}

void APK_OrbsOfDestruction::OnPlayerExitInTrigger()
{
	Super::OnPlayerExitInTrigger();
	// mPlayerHasPassedTrigger = false;
}

void APK_OrbsOfDestruction::Init()
{
	Super::Init();
}

void APK_OrbsOfDestruction::Awake()
{
	Super::Awake();
}

void APK_OrbsOfDestruction::Destroy()
{
	Super::Destroy();
}

void APK_OrbsOfDestruction::OnDrawGizmos()
{
	Super::OnDrawGizmos();
	const FVector _location = GetActorLocation();
	Debug::DrawSphere(GetWorld(), _location, 25, 32, FColor::Blue);
	for (int i = 0; i < mPoints.Num(); i++)
	{
		if (mPoints[i] == nullptr) continue;
		const FVector _pointsLocation = mPoints[i]->GetActorLocation();
		Debug::DrawLine(GetWorld(), _location, _pointsLocation, FColor::Blue);
		Debug::DrawSphere(GetWorld(), _pointsLocation, 25, 32, FColor::Blue);
	}
}

#pragma endregion Override