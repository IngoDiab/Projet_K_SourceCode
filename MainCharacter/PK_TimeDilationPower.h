// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PK_TimeDilationPower.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJET_K_API UPK_TimeDilationPower : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY()
		class APK_MainCharacter* owner = nullptr;
	
	UPROPERTY()
		class UPK_TimeManager* timeManager = nullptr;

	UPROPERTY()
		class AProjet_KGameModeBase* mGameMode = nullptr;
	
	UPROPERTY()
		class APK_HUD* hud = nullptr;

	UPROPERTY(EditAnywhere, Category = "Time Dilation", meta = (DisplayName = "Player can dilate time"))
		bool canDilateTime = true;

	UPROPERTY(VisibleAnywhere, Category = "Time Dilation", meta = (DisplayName = "Time is dilated"))
		bool isTimeDilated = false;

	UPROPERTY(VisibleAnywhere, Category = "Time Dilation", meta = (DisplayName = "Time dilation is in cooldown"))
		bool isTimeDilationInCD = false;

	UPROPERTY(EditAnywhere, Category = "Time Dilation", meta = (DisplayName = "Timer player can dilate time"))
		float timeDilate = 5;

	UPROPERTY(EditAnywhere, Category = "Time Dilation", meta = (DisplayName = "Cooldown dilate time"))
		float cooldownTimeDilate = 5;

	UPROPERTY()
		float timer = 0;

	DECLARE_EVENT_OneParam(APK_MainCharacter, Update, float)
	Update onUpdate;

protected:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTimeDilate);
	UPROPERTY(BlueprintAssignable)
		FTimeDilate onTimeDilate;

public:
	FORCEINLINE Update* OnUpdate() { return &onUpdate; };
	FORCEINLINE FTimeDilate* OnTimeDilate() { return &onTimeDilate; };
	FORCEINLINE bool IsTimeDilated() const { return isTimeDilated; };

public:	
	// Sets default values for this component's properties
	UPK_TimeDilationPower();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


public:
	void InitOwner();
	void InitOwnerDelegate();
	void InitHUD();
	void InitTimeManager();

	void TimeDilate();

	UFUNCTION(BlueprintCallable)
	void StopTimeDilate();

	void TimerTimeDilation(const float _tick);
	void TimerCooldownTimeDilation(const float _tick);

	void RefreshHUDTimeDilate();
};
