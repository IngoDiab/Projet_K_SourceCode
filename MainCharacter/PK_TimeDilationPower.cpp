// Fill out your copyright notice in the Description page of Project Settings.


#include "PK_TimeDilationPower.h"
#include "PK_MainCharacter.h"

#include "Projet_K/Projet_KGameModeBase.h"
#include "Projet_K/UI/PK_HUD.h"
#include "Projet_K/Manager/TimeManager/PK_TimeManager.h"

#include "Kismet/GameplayStatics.h"

UPK_TimeDilationPower::UPK_TimeDilationPower()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPK_TimeDilationPower::BeginPlay()
{
	Super::BeginPlay();
	InitOwner();
	InitHUD();
	InitTimeManager();
}

void UPK_TimeDilationPower::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPK_TimeDilationPower::InitOwner()
{
	owner = Cast<APK_MainCharacter>(GetOwner());
	InitOwnerDelegate();
}

void UPK_TimeDilationPower::InitOwnerDelegate()
{
	if (!owner) return;
	owner->OnUpdate()->AddLambda([this](float _tick)
	{
		TimerTimeDilation(_tick);
		TimerCooldownTimeDilation(_tick);
		RefreshHUDTimeDilate();
	});
}

void UPK_TimeDilationPower::InitHUD()
{
	APlayerController* _controller = GetWorld()->GetFirstPlayerController();
	if (!_controller)return;
	hud = Cast<APK_HUD>(_controller->GetHUD());
}

void UPK_TimeDilationPower::InitTimeManager()
{
	mGameMode = GetWorld()->GetAuthGameMode<AProjet_KGameModeBase>();
}

void UPK_TimeDilationPower::TimeDilate()
{
	if (!canDilateTime) return;
	onTimeDilate.Broadcast();
	timer = (timer / cooldownTimeDilate) * timeDilate;
	isTimeDilated = true;
	isTimeDilationInCD = false;
	mGameMode->GetTimeManager()->SetSlowMoWithDifferentForPlayer();
}

void UPK_TimeDilationPower::StopTimeDilate()
{
	if (!isTimeDilated) return;
	timer = (timer/timeDilate)* cooldownTimeDilate;
	isTimeDilated = false;
	isTimeDilationInCD = true;
	mGameMode->GetTimeManager()->SetRealTime();
}
void UPK_TimeDilationPower::TimerTimeDilation(const float _tick)
{
	if (!isTimeDilated)return;
	if (timer >= timeDilate) StopTimeDilate();
	else timer += _tick;
}
void UPK_TimeDilationPower::TimerCooldownTimeDilation(const float _tick)
{
	if (!isTimeDilationInCD) return;
	if (timer <= 0) isTimeDilationInCD = false;
	else timer -= _tick;
}

void UPK_TimeDilationPower::RefreshHUDTimeDilate()
{
	if (!hud)return;
	if (isTimeDilated) hud->UpdateTimeDilate(1-timer / timeDilate, 1);
	else if (isTimeDilationInCD) hud->UpdateTimeDilate(1-timer / cooldownTimeDilate, .5f);
}
