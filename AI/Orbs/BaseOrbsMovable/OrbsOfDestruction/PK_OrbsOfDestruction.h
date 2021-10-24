#pragma once

#include "CoreMinimal.h"
#include "../PK_BaseOrbsMovable.h"
#include "PK_OrbsOfDestruction.generated.h"

class APK_OrbsBullet;

USTRUCT()
struct FOrbsShoot
{
	GENERATED_BODY()
private:
	UPROPERTY(Category="Orbs Shoot | Settings", EditAnywhere, meta = (DisplayName = "Cooldown"))
		float mCooldown = 2.0f;
	UPROPERTY(Category="Orbs Shoot | Settings", VisibleAnywhere, meta = (DisplayName = "Current Cooldown"))
		float mCurrentCooldown = 0.0f;
public:
	FORCEINLINE float GetCooldown()const { return mCooldown;}
	FORCEINLINE float GetCurrentCooldown()const {return mCurrentCooldown;}
	FORCEINLINE void SetCurrentCooldown(const float _cooldown) { mCurrentCooldown = _cooldown; }
	FORCEINLINE void Update(const float& _deltaTime)
	{
		if (GetCurrentCooldown() <= 0) return;
		mCurrentCooldown -= _deltaTime;
	}
};

UCLASS()
class PROJET_K_API APK_OrbsOfDestruction : public APK_BaseOrbsMovable
{
	GENERATED_BODY()
#pragma region f/p
private:
	UPROPERTY(Category="Orbs Of Destruction", EditAnywhere, meta = (DisplayName = "Orbs Shoot"))
		FOrbsShoot mOrbsShoot = FOrbsShoot();
	UPROPERTY(Category="Orbs Of Destruction", EditAnywhere, meta = (DisplayName = "points"))
	TArray<AActor*> mPoints = TArray<AActor*>();
	UPROPERTY(Category="Orbs Of Destruction", VisibleAnywhere, meta = (DisplayName = "current index of points"))
	int mCurrentIndexPoints = 0;
	UPROPERTY(Category="Orbs Of Destruction", EditAnywhere, meta = (DisplayName = "player passed trigger"))
	bool mPlayerHasPassedTrigger = false;
	UPROPERTY(Category="Orbs Of Destruction", EditAnywhere, meta = (DisplayName = "loop points"))
	bool mLoopPoints = false;
	UPROPERTY(Category="Orbs Of Destruction | Bullet", EditAnywhere, meta = (DisplayName = "bullet prefab"))
	TSubclassOf<APK_OrbsBullet> mBulletPrefab = nullptr;
	
	
#pragma endregion f/p
#pragma region Accessors
public:
	/// <summary>
	/// return all points 
	/// </summary>
	TArray<AActor*> GetPoints() const;
#pragma endregion Accessors
#pragma region custom methods
private:
public:
#pragma endregion custom methods
#pragma region Event methods
public:
	UFUNCTION(BlueprintImplementableEvent) void OnShoot();	
#pragma endregion Event methods
#pragma region unreal methods
protected:
	virtual void Tick(float DeltaSeconds) override;
#pragma endregion unreal methods
#pragma region Override
protected:
	virtual void Shoot() override;
public:
	virtual void OnDamage() override;
	virtual void OnPlayerEnterInTrigger() override;
	virtual void OnPlayerExitInTrigger() override;
protected:
	virtual void Init() override;
	virtual void Awake() override;
	virtual void Destroy() override;
	virtual void OnDrawGizmos() override;
public:
	virtual bool IsValid() const override;
	bool CanShoot();
	void SpawnBullet();
protected:
#pragma endregion Override
};
