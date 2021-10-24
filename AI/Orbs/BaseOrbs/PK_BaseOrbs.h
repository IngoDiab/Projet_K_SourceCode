#pragma once

#include "CoreMinimal.h"

#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Projet_K/Interface/Target/PK_Target.h"
#include "Projet_K/Utils/Action/Action.h"

#include "PK_BaseOrbs.generated.h"

class APK_GrappleGPE;

UCLASS()
class PROJET_K_API APK_BaseOrbs : public AActor, public IPK_Target
{
private:
	GENERATED_BODY()

#pragma region f/P
protected:
	
	UPROPERTY(Category="Base orbs | Components", EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Trigger", AllowPrivateAccess = "true"))
		UBoxComponent* mSphereTrigger = nullptr;
	UPROPERTY(Category="Base orbs | Components", EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Capsule collider", AllowPrivateAccess = "true"))
		UStaticMeshComponent* mMeshCollider = nullptr;
	UPROPERTY(Category="Base orbs | Settings", EditAnywhere, meta = (DisplayName = "Grapple Prefab"))
		TSubclassOf<APK_GrappleGPE> mGrapplePrefab = nullptr;
	UPROPERTY(Category="Base orbs | Settings", EditAnywhere, meta = (DisplayName = "Use Grapple on death"))
		bool mGrappleOnDeath = false;
	UPROPERTY()
		float mDeltaTime = 0.0f;
#pragma endregion f/p
#pragma region Event
public:
	Action<> OnTargetInZone = Action<>();
	Action<> OnUpdate = Action<>();
	Action<> OnDrawGizmosEvent = Action<>();
	Action<> OnTakeDamage = Action<>();
#pragma endregion Event
#pragma region constructor
public:	
	APK_BaseOrbs();
	
#pragma endregion constructor
#pragma region unreal methods
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void BeginDestroy() override;

public:
	virtual bool ShouldTickIfViewportsOnly() const override;
protected:
#pragma endregion unreal methods
#pragma region Event methods
public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnDeath();
#pragma endregion Event methods
#pragma region custom methods
protected:
	virtual void Init();
	virtual void Awake();
	virtual void Destroy();
	virtual void OnDrawGizmos();
	virtual void Shoot();
public:
	virtual void OnDamage() override;
	virtual void OnPlayerEnterInTrigger() PURE_VIRTUAL(,);
	virtual void OnPlayerExitInTrigger() PURE_VIRTUAL(,);
	virtual void OnActorEnterInTrigger(AActor* _actor);
	virtual void OnActorExitInTrigger(AActor* _actor);

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor,
								UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _bFromSweep, const FHitResult & _sweepResult);
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex);
#pragma endregion custom methods
};
