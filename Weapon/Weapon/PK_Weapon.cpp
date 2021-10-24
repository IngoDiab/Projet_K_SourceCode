#include "PK_Weapon.h"

#include "Components/SphereComponent.h"
#include "Projet_K/Utils/Debug.h"

#pragma region accessors
bool APK_Weapon::IsValid() const
{
	return mWeaponStats.AmmoPrefab() != nullptr && mMesh != nullptr;
}

FVector APK_Weapon::GetShootPosition() const
{
	return mBulletPosition->GetComponentLocation();
}

USceneComponent* APK_Weapon::GetBulletPosition() const
{
	return mBulletPosition;
}

#pragma endregion accessors
#pragma region constructor
APK_Weapon::APK_Weapon()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root Component");
	mBulletPosition = CreateDefaultSubobject<USceneComponent>("Bullet position");
	mBulletPosition->SetupAttachment(RootComponent);	
	mMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	mMesh->SetupAttachment(RootComponent);
}
#pragma endregion constructor
#pragma region unreal methods
void APK_Weapon::BeginPlay()
{
	Super::BeginPlay();
	Init();
}

void APK_Weapon::BeginDestroy()
{
	Super::BeginDestroy();
	OnShoot.Clear();
	OnDrawGizmosEvent.Clear();
}

void APK_Weapon::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	OnDrawGizmosEvent.Invoke();
	OnUpdate.Invoke(DeltaSeconds);
	mWeaponStats.Update(DeltaSeconds);
}
#pragma endregion unreal methods
#pragma region custom methods
void APK_Weapon::OnDrawGizmos()
{
	Debug::DrawLine(GetWorld(), GetActorLocation(), GetShootPosition(), FColor::Red);
	Debug::DrawSphere(GetWorld(), GetShootPosition(), 5.0f, 32, FColor::Red);
}

void APK_Weapon::Init()
{
	OnDrawGizmosEvent.Add(this, &APK_Weapon::OnDrawGizmos);
}

void APK_Weapon::Shoot()
{
	if (!IsValid())
	{
		Debug::ScreenMessage("[Weapon] => Ammo prefab is null !", FColor::Red, 5.0f);
		return;
	}
	if (mWeaponStats.IsInCooldown()) return;
	//////
	const FVector _bulletPos = mBulletPosition->GetComponentLocation();
	const FRotator _bulletRot = mBulletPosition->GetComponentRotation();
	//////
	const AActor* _bullet = GetWorld()->SpawnActor(mWeaponStats.AmmoPrefab(), &_bulletPos, &_bulletRot);
	if (!_bullet)return;
	mWeaponStats.SetCooldown();
	OnShoot.Invoke();
}
#pragma endregion custom methods
