#include "PK_Spawner.h"

#include "Projet_K/Utils/Debug.h"

#pragma region accessors
FVector APK_Spawner::GetSpawnLocation() const
{
	return GetActorLocation() + mTurretSpawnLocation;
}

bool APK_Spawner::TargetInZone() const
{
	return FVector::Distance(GetActorLocation(), mTarget->GetActorLocation()) < mDistanceForSpawn;
}
#pragma endregion accessors 

#pragma region constructor

APK_Spawner::APK_Spawner()
{
	PrimaryActorTick.bCanEverTick = true;
}
#pragma endregion constructor

#pragma region unreal methods
void APK_Spawner::BeginPlay()
{
	Super::BeginPlay();
	Init();
}

void APK_Spawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	OnUpdate.Invoke();
	OnDrawGizmosEvent.Invoke();
}

void APK_Spawner::BeginDestroy()
{
	Super::BeginDestroy();
	OnUpdate.Clear();
	OnDrawGizmosEvent.Clear();
}

#pragma endregion unreal methods

#pragma region custom methods
void APK_Spawner::Init()
{
	mTarget = GetWorld()->GetFirstPlayerController()->GetOwner();
	OnDrawGizmosEvent.Add(this, &APK_Spawner::OnDrawGizmos);
	OnUpdate.Add(this, &APK_Spawner::CheckForSpawnChaos);
}

void APK_Spawner::OnDrawGizmos()
{
	const FColor _color = IsValid() ? mDebugValidColor : mDebugErrorColor;
	const FVector _spawnLocation = GetSpawnLocation();
	Debug::DrawSphere(GetWorld(), _spawnLocation, GetDebugSize(), 32, _color);
	Debug::DrawLine(GetWorld(), GetActorLocation(), _spawnLocation, _color);
	Debug::DrawSphere(GetWorld(), GetActorLocation(), mDistanceForSpawn, 32, mDebugDistanceColor);
}

void APK_Spawner::CheckForSpawnChaos()
{
	if (!IsValid() || !TargetInZone())return;
	SpawnChaos();
}

void APK_Spawner::SpawnChaos()
{
	const FVector _spawnLocation = GetSpawnLocation();
	// const APK_Turret * _turret = GetWorld()->SpawnActor<APK_Turret>(mTurretPrefab, _spawnLocation, FRotator::ZeroRotator);
	// if (!_turret) return;
	Debug::ScreenMessage("Chaos spawned !", FColor::Cyan, -1);
}
#pragma endregion custom methods

#pragma region Override
#pragma region IValid
bool APK_Spawner::IsValid() const
{
	return false; //mTurretPrefab != nullptr && mTarget != nullptr;
}
#pragma endregion IValid
#pragma region IVisualDebug
FColor APK_Spawner::GetDebugValidColor() const
{
	return mDebugValidColor;
}

FColor APK_Spawner::GetDebugErrorColor() const
{
	return mDebugErrorColor;
}

float APK_Spawner::GetDebugSize() const
{
	return mDebugSize;
}

float APK_Spawner::GetDebugHeight() const
{
	return mDebugHeight;
}
#pragma endregion IVisualDebug
#pragma endregion Override