#pragma once

#include "CoreMinimal.h"
#include "../../BaseOrbsMovable/PK_BaseOrbsMovable.h"
#include "PK_OrbsOfTime.generated.h"

class AProjet_KGameModeBase;

UCLASS()
class PROJET_K_API APK_OrbsOfTime : public APK_BaseOrbsMovable
{
	GENERATED_BODY()
#pragma region f/p
private:
	UPROPERTY(Category="Orbs Of Time | Settings", EditAnywhere, meta =(DisplayName= "Time for reset"))
	float mTimeForReset = 5.0f;
	UPROPERTY(Category="Orbs Of Time | Debug", VisibleAnywhere, meta = (DisplayName = "Is Actif"))
	bool mIsActif = false;
	UPROPERTY(Category="Orbs Of Time | Debug", VisibleAnywhere, meta = (DisplayName = "Actor in zone"))
	TArray<AActor*> mActorsInZone = TArray<AActor*>();
	UPROPERTY()
	AProjet_KGameModeBase* mGameMode = nullptr;
	UPROPERTY()
	FTimerHandle mTimerHandle = FTimerHandle();
#pragma endregion f/p
#pragma region custom methods
private:
	 void ApplyTimeInZone();
	void InitActorsInZone();
	UFUNCTION()
	void ResetOrbs();
#pragma endregion custom methods
#pragma region Override
public:
	virtual void OnDamage() override;
protected:
	virtual void Init() override;
	virtual void Awake() override;
	virtual void Destroy() override;
	virtual void OnDrawGizmos() override;
public:
	virtual bool IsValid() const override;
	virtual void OnActorExitInTrigger(AActor* _actor) override;
#pragma endregion Override
};
