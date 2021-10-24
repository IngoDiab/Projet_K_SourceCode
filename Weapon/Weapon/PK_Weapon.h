#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projet_K/Interface/Valid/PK_Valid.h"
#include "Projet_K/Utils/Action/Action.h"
#include "Projet_K/Weapon/Stat/PK_ShootStat.h"
#include "PK_Weapon.generated.h"

UCLASS()
class PROJET_K_API APK_Weapon : public AActor, public IPK_Valid
{
private:
	GENERATED_BODY()
	
#pragma region f/p
	private:
	UPROPERTY(Category="Weapon | Settings | Stats", EditAnywhere, meta = (DisplayName = "weapon stats"))
	FPK_ShootStat mWeaponStats;
	UPROPERTY(Category="Weapon | Settings", EditAnywhere, meta = (DisplayName = "weapon shoot position"))
	USceneComponent* mBulletPosition = nullptr;
	UPROPERTY(Category = "Weapon | Settings", EditAnywhere, meta = (DisplayName = "weapon mesh"))
	UStaticMeshComponent* mMesh = nullptr;
public:
	virtual bool IsValid() const override;
	FVector GetShootPosition()const;
	USceneComponent* GetBulletPosition()const;
#pragma endregion f/p
#pragma region Event
public:
	Action<float> OnUpdate;
	Action<> OnDrawGizmosEvent;
	Action<> OnShoot;
#pragma endregion Event
#pragma region constructor
public:	
	APK_Weapon();
#pragma endregion constructor
#pragma region unreal methods
protected:
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	virtual void Tick(float DeltaSeconds) override;
protected:
#pragma endregion unreal methods
#pragma region custom methods
private:
	void OnDrawGizmos();
	void Init();
public:
	void Shoot();
#pragma endregion custom methods
};
