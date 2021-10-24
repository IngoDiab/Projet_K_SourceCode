#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Projet_K/Utils/Action/Action.h"

#include "PK_Turret.generated.h"
// ici
UCLASS()
class PROJET_K_API APK_Turret : public APawn
{
	GENERATED_BODY()

#pragma region constructor
public:
	APK_Turret();
#pragma endregion constructor
#pragma region events
public: 
	Action<float> OnUpdate;
	Action<> OnDrawGizmosEvent;
#pragma endregion events
#pragma region f/p
private:
	UPROPERTY(Category="Turret | Settings", EditAnywhere,
		meta = (DisplayName = "Distance For Target Player", UIMin = 0.0f, UIMax = 800.0f))
	float mDistanceForTarget = 100.0f;
	UPROPERTY(Category="Turret | Settings", EditAnywhere,
		meta = (DisplayName = "Rotation Speed", UIMin = 0.0f, UIMax = 500.0f))
	float mRotationSpeed = 50.0f;
	UPROPERTY(Category="Turret | Target", EditAnywhere, meta = (DisplayName="Target"))
	ACharacter* mTarget = nullptr;

public:
	bool IsTargetInZone() const;
#pragma endregion f/p
#pragma region unreal methods
protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;
#pragma endregion unreal methods
#pragma region custom methods
private:
	void LookAtTarget(float _deltaTime);
	void OnDrawGizmos();
#pragma endregion custom methods
};
