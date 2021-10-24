// Fill out your copyright notice in the Description page of Project Settings.


#include "PK_ShootSystem.h"
#include "Projet_K/Weapon/Weapon/PK_Weapon.h"
#include "Projet_K/MainCharacter/PK_MainCharacter.h"
#include "Projet_K/MainCharacter/PK_WallRunSystem.h"
#include "Projet_K/MainCharacter/Animations/PK_MCAnimation.h"

#include "Projet_K/MainCharacter/Camera/PK_Camera.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UPK_ShootSystem::UPK_ShootSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPK_ShootSystem::BeginPlay()
{
	Super::BeginPlay();
	InitOwner();
	SpawnWeapon();
}


// Called every frame
void UPK_ShootSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	RefreshAim();
}

void UPK_ShootSystem::InitOwner()
{
	owner = Cast<APK_MainCharacter>(GetOwner());
	FTimerHandle _getCamera;
	GetWorld()->GetTimerManager().SetTimer(_getCamera, this, &UPK_ShootSystem::GetCamera, .1f, false);
	InitAnimations();
}

void UPK_ShootSystem::GetCamera()
{
	if (!owner) return;
	ownerCamera = owner->GetCamera();
}

void UPK_ShootSystem::InitAnimations()
{
	if (!owner)return;
	const USkeletalMeshComponent* _mesh = owner->GetMesh();
	if (!_mesh) return;
	animations = Cast<UPK_MCAnimation>(_mesh->GetAnimInstance());
	if (!animations) return;
	animations->GetOnShoot()->AddDynamic(this, &UPK_ShootSystem::ShootBullet);
}

void UPK_ShootSystem::SpawnWeapon()
{
	if (!owner || !weaponPrefab)return;
	currentWeapon = Cast<APK_Weapon>(GetWorld()->SpawnActor(weaponPrefab));
	if (!currentWeapon) return;
	currentWeapon->AttachToComponent(owner->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("gun"));
}

void UPK_ShootSystem::LookAtCenterWeapon(const FVector _posToLookWeapon, const FVector _posToLookBarrel)
{
	if (!currentWeapon) return;
	FRotator _newBarrel = UKismetMathLibrary::FindLookAtRotation(currentWeapon->GetShootPosition(), _posToLookBarrel);
	currentWeapon->GetBulletPosition()->SetWorldRotation(_newBarrel);

	if (!animations || !animations->GetIsShooting()) return;
	FRotator _newWeapon = UKismetMathLibrary::FindLookAtRotation(currentWeapon->GetActorLocation(), _posToLookWeapon);
	currentWeapon->SetActorRotation(_newWeapon);
}

void UPK_ShootSystem::RefreshAim()
{
	if (!owner || !ownerCamera)return;
	FHitResult _hitAim;
	const bool _aimNotFar = UKismetSystemLibrary::LineTraceSingle(GetWorld(), ownerCamera->GetActorLocation(), ownerCamera->GetActorLocation() + ownerCamera->GetActorForwardVector() * range, 
		ETraceTypeQuery::TraceTypeQuery1, true, owner->GetIgnoredActors(), EDrawDebugTrace::None, _hitAim, true);
	if (_aimNotFar) LookAtCenterWeapon(ownerCamera->GetActorLocation() + ownerCamera->GetActorForwardVector() * range, _hitAim.Location);
}

void UPK_ShootSystem::Shoot()
{
	animations->SetIsShooting(true);
}

void UPK_ShootSystem::ShootBullet()
{
	if (!currentWeapon) return;
	onShoot.Broadcast();
	currentWeapon->Shoot();
}

