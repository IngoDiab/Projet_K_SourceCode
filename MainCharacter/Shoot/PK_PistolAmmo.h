// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projet_K/Weapon/Bullet/PK_Bullet.h"
#include "PK_PistolAmmo.generated.h"

class USphereComponent;

UCLASS()
class PROJET_K_API APK_PistolAmmo : public APK_Bullet
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere)
	float speedBullet = 10000;
	
	UPROPERTY(EditAnywhere)
	float debugTraceLength = 50;

	UPROPERTY() USphereComponent* mCapsuleComponent = nullptr;

	UPROPERTY() float mDeltaTime = 0.0f;
	
public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void FireBullet();
	void CheckBulletCollision();
	virtual void OnBeginHit(UPrimitiveComponent* _hitComponent, AActor* _otherActor, UPrimitiveComponent* _otherComp, FVector _normalImpulse, const FHitResult& _hit) override;
};
