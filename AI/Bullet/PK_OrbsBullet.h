// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projet_K/Utils/Action/Action.h"

#include "PK_OrbsBullet.generated.h"

UCLASS()
class PROJET_K_API APK_OrbsBullet : public AActor
{
	GENERATED_BODY()
#pragma region Event
public:
	Action<> OnUpdate = Action<>();
#pragma endregion Event
#pragma region f/p
private:
	UPROPERTY(Category="Orbs Bullet | Component", EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Sphere Collider", AllowPrivateAccess = "true"))
		class USphereComponent* mSphereCollider = nullptr;
	UPROPERTY(Category="Orbs Bullet", VisibleAnywhere, meta = (DisplayName = "end location"))
		FVector mEndLocation = FVector::ZeroVector;
	UPROPERTY(Category="Orbs Bullet", EditAnywhere, meta = (DisplayName = "bullet speed", UIMin = 10, UIMax = 10000))
		float mSpeed = 150;
	UPROPERTY()
	float mDeltaTime = 0.0f;
#pragma endregion f/p
public:	
	APK_OrbsBullet();
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void BeginDestroy() override;
public:
	void SetEndLocation(const FVector& _location);
private:
	void MoveTo();
	void Init();

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor,
								UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _bFromSweep, const FHitResult & _sweepResult);
};
