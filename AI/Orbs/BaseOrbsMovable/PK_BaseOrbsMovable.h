#pragma once

#include "CoreMinimal.h"
#include "../BaseOrbs/PK_BaseOrbs.h"
#include "Projet_K/Interface/Valid/PK_Valid.h"

#include "PK_BaseOrbsMovable.generated.h"

UCLASS()
class PROJET_K_API APK_BaseOrbsMovable : public APK_BaseOrbs, public IPK_Valid
{
private:
	GENERATED_BODY()
#pragma region f/p
private:
	UPROPERTY(Category="Base orbs Movable", EditAnywhere, meta = (DisplayName = "Start Spline"))
		AActor* mStartSpline = nullptr;
	UPROPERTY(Category="Base orbs Movable", EditAnywhere, meta = (DisplayName = "End Spline"))
		AActor* mEndSpline = nullptr;
	UPROPERTY(Category="Base Orbs Movable", EditAnywhere, meta = (DisplayName = "Loop"))
		bool mLoop = false;
	UPROPERTY(Category="Base Orbs Movable", EditAnywhere, meta = (DisplayName = "Speed", UIMin = 1, UIMax = 1000))
		float mSpeed = 100;
	UPROPERTY(Category="Base orbs Movable", EditAnywhere, meta = (DisplayName = "is at end position"))
		bool mIsAtEndPosition = false;
	UPROPERTY(Category="Base Orbs Movable", EditAnywhere, meta = (DisplayName = "distance min", UIMin = 1, UIMax = 1000))
		float mMinDistance = 500;
#pragma endregion f/p
#pragma region accessors
public:
	FVector GetStartSplinePosition()const;
	FVector GetEndSplinePosition()const;
	bool IsAtPosition(const FVector& _position, float _distance)const;
#pragma endregion accessors
#pragma region custom methods
private:
	void Update();
	void MoveToSpline();
#pragma endregion custom methods
#pragma region Overrid
protected:
	virtual void Init() override;
	virtual void Awake() override;
	virtual void Destroy() override;
	virtual void OnDrawGizmos() override;

public:
	virtual void OnPlayerEnterInTrigger() override;
	virtual void OnPlayerExitInTrigger() override;
	virtual bool IsValid() const override;
#pragma endregion Override
};
