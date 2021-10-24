#include "Projet_KGameModeBase.h"
#include "Manager/TimeManager/PK_TimeManager.h"
#include "Utils/Debug.h"

AProjet_KGameModeBase::AProjet_KGameModeBase()
{
	
}

void AProjet_KGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}

void AProjet_KGameModeBase::PostInitProperties()
{
	Super::PostInitProperties();
}

UPK_TimeManager* AProjet_KGameModeBase::GetTimeManager() const
{
	return mTimeManager;
}

APK_MainCharacter* AProjet_KGameModeBase::GetMainCharacter() const
{
	return mMainCharacter;
}

void AProjet_KGameModeBase::SetMainCharacter(APK_MainCharacter* _mainCharacter)
{
	mTimeManager = NewObject<UPK_TimeManager>(this, mTimeManagerPrefab); 
	mMainCharacter = _mainCharacter;
	if (!GetTimeManager())
	{
		Debug::ScreenMessage("Time Manager is null", FColor::Red, 20.0f);
		return;
	}
	mTimeManager->InitTimeManager(_mainCharacter);
}
