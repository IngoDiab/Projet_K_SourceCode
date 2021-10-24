// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PK_ShootSystem.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJET_K_API UPK_ShootSystem : public UActorComponent
{
	GENERATED_BODY()


protected:
	UPROPERTY()
		class APK_MainCharacter* owner = nullptr;

	UPROPERTY()
		class UPK_MCAnimation* animations = nullptr;
	
	UPROPERTY()
		class APK_Camera* ownerCamera = nullptr;

	UPROPERTY(EditAnywhere, Category = "Weapon", meta = (DisplayName = "Player weapon BP"))
		TSubclassOf<class APK_Weapon> weaponPrefab = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Weapon", meta = (DisplayName = "Player current weapon"))
		APK_Weapon* currentWeapon = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Weapon", meta = (DisplayName = "Player current range"))
		float range = 1000;

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FShootEvent);
	UPROPERTY(BlueprintAssignable)
	FShootEvent onShoot;

public:	
	// Sets default values for this component's properties
	UPK_ShootSystem();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	FORCEINLINE FShootEvent& OnShoot() { return onShoot; };

public:
	void InitOwner();
	void GetCamera();
	void InitAnimations();
	void SpawnWeapon();
	void LookAtCenterWeapon(const FVector _posToLook, const FVector _posToLookBarrel);

public:
	void RefreshAim();
	void Shoot();
	UFUNCTION()
	void ShootBullet();
};
