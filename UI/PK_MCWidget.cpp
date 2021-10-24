#include "PK_MCWidget.h"

#include "Components/Image.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void UPK_MCWidget::NativeConstruct()
{
	Super::NativeConstruct();
	InitTimeDilateUI();
	UpdateTimeDilateUI(1,1);
}

void UPK_MCWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UPK_MCWidget::InitTimeDilateUI()
{
	if (!timeDilateImage)return;
	FSlateBrush _brush = timeDilateImage->Brush;
	timeDilateMaterial = UWidgetBlueprintLibrary::GetDynamicMaterial(_brush);
}

void UPK_MCWidget::UpdateTimeDilateUI(const float _percent, const float _opacity)
{
	if (!timeDilateMaterial)return;
	timeDilateMaterial->SetScalarParameterValue(FName("Percent"), _percent);
	timeDilateMaterial->SetScalarParameterValue(FName("Opacity"), _opacity);
	if (!timeDilateImage)return;
	timeDilateImage->SetBrushFromMaterial(timeDilateMaterial);
	if (_percent >= 1) timeDilateImage->SetVisibility(ESlateVisibility::Hidden);
	else timeDilateImage->SetVisibility(ESlateVisibility::Visible);
}
