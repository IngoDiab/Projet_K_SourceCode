// Fill out your copyright notice in the Description page of Project Settings.


#include "PK_HUD.h"
#include "PK_MCWidget.h"

void APK_HUD::BeginPlay()
{
	Super::BeginPlay();
	InitWidget();
}

void APK_HUD::InitWidget()
{
	if (!MCWidgetBP)return;
	MCWidget = CreateWidget<UPK_MCWidget>(GetWorld(), MCWidgetBP);
	if (!MCWidget) return;
	MCWidget->AddToViewport();
}

void APK_HUD::UpdateTimeDilate(const float _percent, const float _opacity)
{
	if (!MCWidget)return;
	const float _percentClamp = _percent > 1 ? 1 : _percent < 0 ? 0 : _percent;
	const float _opacityClamp = _opacity > 1 ? 1 : _opacity < 0 ? 0 : _opacity;
	MCWidget->UpdateTimeDilateUI(_percentClamp, _opacityClamp);
}
