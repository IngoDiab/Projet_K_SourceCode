#include "PK_TimeManager.h"

#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Projet_K/MainCharacter/PK_MainCharacter.h"
#include "Projet_K/MainCharacter/Camera/PK_Camera.h"
#include "Projet_K/Utils/Debug.h"


UPK_TimeManager::UPK_TimeManager()
{
}

void UPK_TimeManager::SetSlowForAll()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), mSlowMoTime);
}

///<summary>
/// use this function for affect player
///
///</summary>
void UPK_TimeManager::SetSlowMoWithDifferentForPlayer()
{
	if (!IsValid())
	{
		Debug::ScreenMessage("TIME MANAGER Player == nullptr", FColor::Red, 10);
		return;
	}
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), mSlowMoTime);
	mMainCharacter->CustomTimeDilation = mSlowMoTimeForPlayer;
	mMainCharacter->GetCamera()->CustomTimeDilation = mSlowMoTimeForPlayer;
	mMainCharacter->GetCamera()->cameraComp->PostProcessSettings.MotionBlurAmount = mMotionBlurAmount;
	UE_LOG(LogTemp, Warning, TEXT("Slow mo with different for player"));
}

void UPK_TimeManager::SetSlowForActors(TArray<AActor*> _actors)
{
	if (!IsValid())return;
	for (AActor* _actor : _actors)
	{
		if (!_actor) continue;
		if (_actor == mMainCharacter  && !mMainCharacter->IsInCooldownSlowMo() )
		{
			mMainCharacter->CustomTimeDilation =  mSlowMoTimeForPlayerOrbs;
			mMainCharacter->GetCamera()->CustomTimeDilation = mSlowMoTimeForPlayerOrbs;
			mMainCharacter->GetCamera()->cameraComp->PostProcessSettings.MotionBlurAmount = mMotionBlurAmount;
		} else _actor->CustomTimeDilation = mSlowMoTime;
	}
}

void UPK_TimeManager::SetRealTimeForActors(TArray<AActor*> _actors)
{
	if (!IsValid())return;
	for (AActor* _actor : _actors)
	{
		if (!_actor) continue;
		if (_actor == mMainCharacter)
		{
			mMainCharacter->CustomTimeDilation = 1;
			mMainCharacter->GetCamera()->CustomTimeDilation = 1;
			mMainCharacter->GetCamera()->cameraComp->PostProcessSettings.MotionBlurAmount = 0.5f;
			mMainCharacter->SetCooldownForSlowMo();
		}
		else _actor->CustomTimeDilation = mRealTime;
	}
}

void UPK_TimeManager::SetRealTimeForActor(AActor* _actor)
{
	if (!IsValid())return;
	if (_actor == mMainCharacter)
	{
		Debug::ScreenMessage("Player real time for actor", FColor::Cyan, 10.0f);
		mMainCharacter->CustomTimeDilation = 1;
		mMainCharacter->GetCamera()->CustomTimeDilation = 1;
		mMainCharacter->GetCamera()->cameraComp->PostProcessSettings.MotionBlurAmount = 0.5f;
		mMainCharacter->SetCooldownForSlowMo();
	}
	else _actor->CustomTimeDilation = mRealTime;
}

void UPK_TimeManager::SetSlowForActor(AActor* _actor)
{
	if (!IsValid())return;
	if (_actor == mMainCharacter && !mMainCharacter->IsInCooldownSlowMo())
	{
		mMainCharacter->CustomTimeDilation =  mSlowMoTimeForPlayerOrbs;
		mMainCharacter->GetCamera()->CustomTimeDilation = mSlowMoTimeForPlayerOrbs;
		mMainCharacter->GetCamera()->cameraComp->PostProcessSettings.MotionBlurAmount = mMotionBlurAmount;
	}
	else _actor->CustomTimeDilation = mSlowMoTime;
}

///<summary>
/// use this function for affect world but not the player
///
///</summary>
void UPK_TimeManager::SetRealTime()
{
	if (!IsValid()) return;
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), mRealTime);
	mMainCharacter->CustomTimeDilation = 1;
	mMainCharacter->GetCamera()->CustomTimeDilation = 1;
	mMainCharacter->GetCamera()->cameraComp->PostProcessSettings.MotionBlurAmount = 0.5f;
	
}

void UPK_TimeManager::InitTimeManager(APK_MainCharacter* _mainCharacter)
{
	Debug::ScreenMessage("InitTimeManager", FColor::Red, 10);
	mMainCharacter = _mainCharacter;
	if (!mMainCharacter)
	{
		mMainCharacter = Cast<APK_MainCharacter>(GetWorld()->GetFirstPlayerController()->GetOwner());
		if (!mMainCharacter)
			Debug::ScreenMessage("Cast player == null", FColor::Red, 10);
	}
}

bool UPK_TimeManager::IsValid() const
{
	return mMainCharacter != nullptr;
}
