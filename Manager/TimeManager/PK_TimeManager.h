#pragma once

#include "CoreMinimal.h"

#include "Projet_K/Interface/Valid/PK_Valid.h"

#include "PK_TimeManager.generated.h"

class APK_MainCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class PROJET_K_API UPK_TimeManager : public UObject, public IPK_Valid
{
private:
	GENERATED_BODY()
#pragma region f/p
private:
	UPROPERTY(Category="TimeManager | Settings", EditAnywhere, meta = (DisplayName = "Real Time", UIMin = 0.1f, UIMax = 1.0f))
	float mRealTime = 1;
	UPROPERTY(Category="TimeManager | Settings", EditAnywhere, meta = (DisplayName = "Slow Mode Time", UIMin = 0.1f, UIMax = 1.0f))
	float mSlowMoTime = 0.2f;
	UPROPERTY(Category="TimeManager | Settings", EditAnywhere, meta = (DisplayName = "Slow Mode Time for player", UIMIn = 0.1f, UIMax = 1.0f))
	float mSlowMoTimeForPlayer = 10.f;
	UPROPERTY(Category="TimeManager | Settings", EditAnywhere, meta = (DisplayName = "Slow Mode Time for player orbs"))
	float mSlowMoTimeForPlayerOrbs = 0.5f;
	UPROPERTY(Category="TimeManager | Settings", EditAnywhere, meta = (DisplayName = "motion blur"))
	float mMotionBlurAmount = 1;

	UPROPERTY(Category="TimeManager | Player", VisibleAnywhere, meta = (DisplayName = "Main Character"))
	APK_MainCharacter* mMainCharacter = nullptr;
	
#pragma endregion f/p
#pragma region constructor
public:	
	UPK_TimeManager();
#pragma endregion constructor
#pragma region custom methods
public:
	void SetSlowForAll();
	void SetSlowMoWithDifferentForPlayer();
	void SetSlowForActors(TArray<AActor*> _actors);
	void SetRealTimeForActors(TArray<AActor*> _actors);
	void SetRealTimeForActor(AActor* _actor);
	void SetSlowForActor(AActor* _actor);
	void SetRealTime();
	void InitTimeManager(APK_MainCharacter* _mainCharacter);
#pragma endregion custom methods
#pragma region Override
public:
	virtual bool IsValid() const override;

#pragma endregion Override
};
