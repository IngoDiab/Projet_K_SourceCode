// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Projet_KGameModeBase.generated.h"

class UPK_TimeManager;
class APK_MainCharacter;

UCLASS()
class PROJET_K_API AProjet_KGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

AProjet_KGameModeBase();
	
#pragma region f/p
private:
	UPROPERTY(Category="GameMode  | Manager | Time", EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Time Manager Prefab", AllowPrivateAccess = true))
		TSubclassOf<UPK_TimeManager> mTimeManagerPrefab = nullptr;
	UPROPERTY(Category="GameMode | Manager | Time", VisibleAnywhere, meta = (DisplayName = "Time Manager"))
		UPK_TimeManager* mTimeManager = nullptr;
		
	UPROPERTY(Category="GameMode | Player", VisibleAnywhere, meta = (DisplayName = "Player"))
		APK_MainCharacter* mMainCharacter = nullptr;
#pragma endregion f/p
#pragma region unreal methods
protected:
	virtual void BeginPlay() override;
public:
	virtual void PostInitProperties() override;
	UPK_TimeManager* GetTimeManager()const;
	APK_MainCharacter* GetMainCharacter()const;

	void SetMainCharacter(APK_MainCharacter* _mainCharacter);

	
	
#pragma endregion unreal methods
};
