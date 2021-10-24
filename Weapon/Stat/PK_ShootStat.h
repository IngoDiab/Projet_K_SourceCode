#pragma once

#include "CoreMinimal.h"
#include "PK_ShootStat.generated.h"

USTRUCT(BlueprintType)
struct PROJET_K_API FPK_ShootStat
{
	GENERATED_BODY()
	
private:
	UPROPERTY(Category="Weapon | Stat", EditAnywhere, meta = (DisplayName = "ammo prefab"))
		TSubclassOf<AActor> mAmmoPrefab = nullptr;
	UPROPERTY(Category="Weapon | Stat", EditAnywhere, meta = (DisplayName = "max ammo", UIMin = 0, UIMax = 100))
		int mMaxAmmo = 10;
	UPROPERTY(Category="Weapon | Stat", EditAnywhere, meta = (DisplayName = "current ammo", UIMin = 0, UIMax = 100))
		int mCurrentAmmo = 10;
	UPROPERTY(Category="Weapon | Stat", EditAnywhere, meta = (DisplayName = "time cooldown", UIMin = 0, UIMax = 10))
		float mCooldown = 0.5f;

	float mCurrentCooldown = 0.0f;
public:
	TSubclassOf<AActor> AmmoPrefab()const{return mAmmoPrefab;}
	int MaxAmmo()const {return mMaxAmmo;}
	int CurrentAmmo()const {return mCurrentAmmo;}
	float Cooldown()const {return mCooldown;}
	float CurrentCooldown()const {return mCurrentCooldown;}
	void SetCooldown() {mCurrentCooldown = mCooldown;}
	bool IsInCooldown()const {return mCurrentCooldown > 0.0f;}
	void Update(float _deltaTime)
	{
		if (!IsInCooldown()) return;
		mCurrentCooldown -= _deltaTime;
	}
};
