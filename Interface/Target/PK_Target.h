#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PK_Target.generated.h"

UINTERFACE(MinimalAPI)
class UPK_Target : public UInterface
{
	GENERATED_BODY()
};

class PROJET_K_API IPK_Target
{
	GENERATED_BODY()
public:
	virtual void OnDamage() PURE_VIRTUAL(,;);
};
