// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PK_HUD.generated.h"

/**
 * 
 */
UCLASS()
class PROJET_K_API APK_HUD : public AHUD
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Widgets", meta = (DisplayName = "Widget Main Character"))
	TSubclassOf<class UPK_MCWidget> MCWidgetBP = nullptr;
	
	UPROPERTY(VisibleAnywhere)
	UPK_MCWidget* MCWidget = nullptr;

public:
	void BeginPlay() override;

public:
	void InitWidget();

	void UpdateTimeDilate(const float _percent, const float _opacity);
};
