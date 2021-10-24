// Fill out your copyright notice in the Description page of Project Settings.


#include "PK_PistolAmmo.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"

void APK_PistolAmmo::BeginPlay()
{
	Super::BeginPlay();
	FireBullet();
}

void APK_PistolAmmo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	mDeltaTime = DeltaTime;
	
	const FVector _trajectoire = GetActorForwardVector() * speedBullet * mDeltaTime * 10000;
	mCapsuleComponent->AddForce(_trajectoire);
	CheckBulletCollision();
}

void APK_PistolAmmo::FireBullet()
{
	mCapsuleComponent= Cast<USphereComponent>(RootComponent);
	if (!mCapsuleComponent) return;
	mCapsuleComponent->SetGenerateOverlapEvents(true);
	mCapsuleComponent->SetEnableGravity(false);
	mCapsuleComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	mCapsuleComponent->SetSimulatePhysics(true);
}

void APK_PistolAmmo::CheckBulletCollision()
{
	FHitResult _hit;
	bool _hasHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * debugTraceLength, ETraceTypeQuery::TraceTypeQuery4,true, TArray<AActor*>(), EDrawDebugTrace::ForOneFrame, _hit, true);
	if (!_hasHit || !_hit.GetActor() || !_hit.GetComponent()) return;
	mMesh->OnComponentHit.Broadcast(mMesh, _hit.GetActor(), _hit.GetComponent(), _hit.ImpactNormal, _hit);
	if (!_hit.GetActor() || !_hit.GetComponent()) return;
	_hit.GetComponent()->OnComponentHit.Broadcast(_hit.GetComponent(), this, mMesh, -_hit.ImpactNormal, _hit);
}

void APK_PistolAmmo::OnBeginHit(UPrimitiveComponent* _hitComponent, AActor* _otherActor, UPrimitiveComponent* _otherComp, FVector _normalImpulse, const FHitResult& _hit)
{
	Super::OnBeginHit(_hitComponent, _otherActor, _otherComp, _normalImpulse, _hit);
	DrawDebugSphere(GetWorld(), _hit.Location, 12, 12, FColor::Blue, true);
	UGameplayStatics::ApplyPointDamage(_otherActor, 500, _hit.Location, _hit, nullptr, nullptr, nullptr);
	Destroy();
}
