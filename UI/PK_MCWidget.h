// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "PK_MCWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJET_K_API UPK_MCWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* timeDilateImage = nullptr;
	
	UPROPERTY(EditAnywhere)
	UMaterialInstanceDynamic* timeDilateMaterial = nullptr;

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void InitTimeDilateUI();
	void UpdateTimeDilateUI(const float _percent, const float _opacity);

};
