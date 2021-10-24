#include "PK_OrbsOfTime.h"

#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Projet_K/Projet_KGameModeBase.h"
#include "Projet_K/Manager/TimeManager/PK_TimeManager.h"
#include "Projet_K/Utils/Debug.h"


#pragma region custom methods
void APK_OrbsOfTime::ApplyTimeInZone()
{
	if (!IsValid() || !mIsActif)return;
	mGameMode->GetTimeManager()->SetRealTimeForActors(mActorsInZone);
	InitActorsInZone();
	mGameMode->GetTimeManager()->SetSlowForActors(mActorsInZone);
}

void APK_OrbsOfTime::InitActorsInZone()
{
	mActorsInZone.Empty();
	if (!mSphereTrigger) return;
	const FVector _start = GetActorLocation();
	const FVector _extent = mSphereTrigger->GetCollisionShape().GetExtent();
	TArray<FHitResult> _results = TArray<FHitResult>();
	bool _hasActors = UKismetSystemLibrary::BoxTraceMulti(GetWorld(), _start, _start, _extent, FRotator::ZeroRotator, TraceTypeQuery4, true, TArray<AActor*>(), EDrawDebugTrace::ForOneFrame, _results, true, FLinearColor::Red, FLinearColor::Blue, 5.0f);
	if (!_hasActors) return;
	for (FHitResult _res : _results)
	{
		AActor* _actor = _res.GetActor();
		if (!_actor) continue;
		mActorsInZone.Add(_actor);
	}
}


#pragma endregion custom methods
#pragma region Override
void APK_OrbsOfTime::ResetOrbs()
{
	mIsActif = true;
	GetWorld()->GetTimerManager().ClearTimer(mTimerHandle);
}

void APK_OrbsOfTime::OnDamage()
{
	Super::OnDamage();
	if (!IsValid())return;
	mIsActif = false;
	mGameMode->GetTimeManager()->SetRealTimeForActors(mActorsInZone);
	mActorsInZone.Empty();
	FTimerDelegate _delegate;
	_delegate.BindUFunction(this, "ResetOrbs");
	GetWorld()->GetTimerManager().SetTimer(mTimerHandle, _delegate, mTimeForReset, false);
}

void APK_OrbsOfTime::Init()
{
	Super::Init();
	mIsActif = true;
	OnUpdate.Add(this, &APK_OrbsOfTime::ApplyTimeInZone);
	mGameMode = Cast<AProjet_KGameModeBase>(GetWorld()->GetAuthGameMode());
	InitActorsInZone();
}

void APK_OrbsOfTime::Awake()
{
	Super::Awake();
}

void APK_OrbsOfTime::Destroy()
{
	Super::Destroy();
	mActorsInZone.Empty();
}

void APK_OrbsOfTime::OnDrawGizmos()
{
	Super::OnDrawGizmos();
	if (!mSphereTrigger) return;
	Debug::DrawCube(GetWorld(), GetActorLocation(), mSphereTrigger->GetCollisionShape().GetExtent(), FColor::Blue);
}

bool APK_OrbsOfTime::IsValid() const
{
	return Super::IsValid() && mGameMode != nullptr;
}

void APK_OrbsOfTime::OnActorExitInTrigger(AActor* _actor)
{
	Super::OnActorExitInTrigger(_actor);
	if (!mActorsInZone.Contains(_actor)) return;
	mActorsInZone.Remove(_actor);
	mGameMode->GetTimeManager()->SetRealTimeForActor(_actor);
}
#pragma endregion Override
