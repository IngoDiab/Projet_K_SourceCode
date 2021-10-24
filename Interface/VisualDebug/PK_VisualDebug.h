#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PK_VisualDebug.generated.h"

UINTERFACE(MinimalAPI)
class UPK_VisualDebug : public UInterface
{
	GENERATED_BODY()
};

class PROJET_K_API IPK_VisualDebug
{
	GENERATED_BODY()
public:
	virtual FColor GetDebugValidColor()const PURE_VIRTUAL(, return FColor::White;);
	virtual FColor GetDebugErrorColor()const PURE_VIRTUAL(, return FColor::White;);
	virtual float GetDebugSize()const PURE_VIRTUAL(, return 0.0f;);
	virtual float GetDebugHeight()const PURE_VIRTUAL(, return 0.0f;);
	
};
