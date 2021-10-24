// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PK_GrappleSystem.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJET_K_API UPK_GrappleSystem : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY()
	class APK_MainCharacter* owner = nullptr;

	UPROPERTY()
	class UPK_AutoMoveSystem* autoMovement = nullptr;

	UPROPERTY()
	class APK_Camera* camera = nullptr;

	UPROPERTY()
	class UPK_MCAnimation* animations = nullptr;

protected:

	UPROPERTY()
	class UCableComponent* cable = nullptr;
	
	UPROPERTY()
	USceneComponent* endCable = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Player", meta = (DisplayName = "Player can grapple"), BlueprintReadWrite)
	bool canGrapple = true;

	UPROPERTY(VisibleAnywhere, Category = "Grapple", meta = (DisplayName = "Grapple has been launched"))
	bool grappleLaunched = false;

	UPROPERTY(VisibleAnywhere, Category = "Player", meta = (DisplayName = "Player is grappling"), BlueprintReadWrite)
	bool isGrappling = false;
	
	UPROPERTY(EditAnywhere, Category = "GPE", meta = (DisplayName = "Angle allowed to look to take the grapple"))
	float minAngle = .9f;

	UPROPERTY(EditAnywhere, Category = "GPE", meta = (DisplayName = "Max speed on grapple"))
	float maxSpeedGPE = 1200;
	
	UPROPERTY(EditAnywhere, Category = "Grapple", meta = (DisplayName = "Grapple speed to hangpoint"))
	float speedGrapple = 10000;

	UPROPERTY(EditAnywhere, Category = "Grapple | AutoMove", meta = (DisplayName = "Min distance to allow the grapple to be hanged"))
	float minDistance = 15;

	UPROPERTY(VisibleAnywhere, Category = "GPE", meta = (DisplayName = "All GPE in grapple range"))
	TArray<class APK_GrappleGPE*> grappleGPEInRange;

	UPROPERTY(VisibleAnywhere, Category = "GPE", meta = (DisplayName = "GPE aimed"))
	APK_GrappleGPE* aimedGrappleGPE;

	UPROPERTY(VisibleAnywhere, Category = "GPE", meta = (DisplayName = "GPE being used"))
	APK_GrappleGPE* targetGrappleGPE;

	UPROPERTY()
	USceneComponent* targetGrappleGPEHangPoint;

	DECLARE_EVENT_OneParam(UPK_MovementBehaviour, Update, float)
	Update onUpdate;

	DECLARE_EVENT(UPK_GrappleSystem, TouchObject)
	TouchObject onObjectTouch;

	DECLARE_EVENT(UPK_GrappleSystem, EndAutoMove)
	EndAutoMove onEndAutoMove;

protected:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGrappleLaunch);
	UPROPERTY(BlueprintAssignable)
		FOnGrappleLaunch onGrappleLaunch;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGrapple);
	UPROPERTY(BlueprintAssignable)
		FOnGrapple onGrapple;

public:
	FORCEINLINE bool GetIsGrappling()const { return isGrappling; };


public:	
	// Sets default values for this component's properties
	UPK_GrappleSystem();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	FORCEINLINE FOnGrappleLaunch* OnGrappleLaunch() { return &onGrappleLaunch; };
	FORCEINLINE TouchObject* OnObjectTouch() { return &onObjectTouch; };
	FORCEINLINE EndAutoMove* OnEndAutoMove() { return &onEndAutoMove; };

	FORCEINLINE bool IsGrappling() const { return isGrappling; };
	FORCEINLINE void SetCanGrapple(const bool _canGrapple) { canGrapple = _canGrapple; }
	FORCEINLINE APK_GrappleGPE* GetCurrentGPE() const { return targetGrappleGPE; };
	FORCEINLINE float GetMaxSpeedGPE() const { return maxSpeedGPE; };

	UFUNCTION()
		void OnGrapple();
public:
	//Init Component
	void InitOwner();
	void InitAnimations();
	void InitOwnerDelegate();
	void InitCable();
	void GetCamera();
	void UpdateAnimation();
	void SetCurrentTarget(APK_GrappleGPE* _target);
	void HideCable(const bool _hide);

	void ShootGrapple();
	void ActivateGrapple();
	UFUNCTION()
	void LaunchGrapple();
	void MoveGrapple(const float _tick);
	void HangGrapple();

	bool GrappleTouched(const FVector _endPos) const;

	void UseTarget();

	void ResetGrapple();
	UFUNCTION(BlueprintCallable)
	void ResetGrappleOnDeath();

public:
	FORCEINLINE void AddInRangeGPE(APK_GrappleGPE& _gpe) { if (!grappleGPEInRange.Contains(&_gpe)) grappleGPEInRange.Add(&_gpe); };
	void RemoveInRangeGPE(APK_GrappleGPE& _gpe);
	void CheckAimedGPE();
	APK_GrappleGPE* CheckGPE(float& _currentdotInView, APK_GrappleGPE& _newGPE, APK_GrappleGPE* _currentGPE);
	float GetDotProductViewGPE(const APK_GrappleGPE& _newGPE);
	void ResetCableComponent();
	bool CheckObstacle(const FVector _gpePos) const;
};
