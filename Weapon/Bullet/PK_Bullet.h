#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projet_K/Utils/Action/Action.h"

#include "PK_Bullet.generated.h"

UCLASS()
class PROJET_K_API APK_Bullet : public AActor
{
	GENERATED_BODY()
	
#pragma region Event
	Action<> OnUpdate = Action<>();
	Action<> OnDrawGizmosEvent = Action<>();
#pragma endregion Event
#pragma region f/p
protected:
	/*UPROPERTY(Category="Bullet | Settings", EditAnywhere, meta = (DisplayName = "bullet collider"))
		class USphereComponent* mBulletCollider = nullptr;*/
	UPROPERTY(Category = "Bullet | Settings", EditAnywhere, meta = (DisplayName = "bullet mesh"))
		UStaticMeshComponent* mMesh = nullptr;
#pragma endregion f/p
#pragma region constructor
public:	
	APK_Bullet();
#pragma endregion constructor
#pragma region unreal methods
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void BeginDestroy() override;
protected:
#pragma endregion unreal methods
#pragma region custom methods
	/* init */	
	void Init();
	void InitEvent();
	void InitCollider();
	/* debug */
	void OnDrawGizmos();
	
	UFUNCTION()
	virtual void OnBeginHit(UPrimitiveComponent* _hitComponent, AActor* _otherActor,
		UPrimitiveComponent* _otherComp, FVector _normalImpulse, const FHitResult& _hit);
	
#pragma endregion custom methods
};
