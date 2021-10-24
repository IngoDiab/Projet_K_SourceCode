// Fill out your copyright notice in the Description page of Project Settings.


#include "PK_GrappleSystem.h"
#include "PK_MainCharacter.h"
#include "PK_AutoMoveSystem.h"
#include "Projet_K/MainCharacter/Camera/PK_Camera.h"
#include "Projet_K/MainCharacter/Animations/PK_MCAnimation.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../GPE/PK_GrappleGPE.h"
#include "../../../Plugins/Runtime/CableComponent/Source/CableComponent/Classes/CableComponent.h"

// Sets default values for this component's properties
UPK_GrappleSystem::UPK_GrappleSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
}


// Called when the game starts
void UPK_GrappleSystem::BeginPlay()
{
	Super::BeginPlay();
	InitOwner();

	onUpdate.AddLambda([this](float _tick)
	{
		CheckAimedGPE();
		MoveGrapple(_tick);
	});

	onObjectTouch.AddLambda([this]()
	{
		HangGrapple();
		UseTarget();
	});

	onEndAutoMove.AddLambda([this]()
	{
		ResetGrapple();
	});

	onGrapple.AddDynamic(this, &UPK_GrappleSystem::OnGrapple);
}


// Called every frame
void UPK_GrappleSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPK_GrappleSystem::InitOwner()
{
	owner = Cast<APK_MainCharacter>(GetOwner());
	if (!owner) return;
	autoMovement = owner->GetAutoMoveSystem();

	FTimerHandle _getCamera;
	GetWorld()->GetTimerManager().SetTimer(_getCamera, this, &UPK_GrappleSystem::GetCamera, .1f, false);

	InitCable();
	InitOwnerDelegate();
	InitAnimations();
}

void UPK_GrappleSystem::InitAnimations()
{
	if (!owner) return;
	USkeletalMeshComponent* _mesh = owner->GetMesh();
	if (!_mesh) return;
	animations = Cast<UPK_MCAnimation>(_mesh->GetAnimInstance());
	if (!animations) return;
	animations->GetOnGrapple()->AddDynamic(this, &UPK_GrappleSystem::LaunchGrapple);
}

void UPK_GrappleSystem::InitOwnerDelegate() 
{
	if (!owner) return;
	owner->OnUpdate()->AddLambda([this](float _tick)
	{
		UpdateAnimation();
		onUpdate.Broadcast(_tick);
	});
}

void UPK_GrappleSystem::InitCable()
{
	if (!owner) return;
	cable = owner->GetCable();
	endCable = owner->GetEndCable();
	if (!cable) return;
	cable->SetAttachEndTo(owner, TEXT("EndCable"));
}

void UPK_GrappleSystem::GetCamera()
{
	if (!owner) return;
	camera = owner->GetCamera();
}

void UPK_GrappleSystem::UpdateAnimation()
{
	if (animations) animations->SetIsGrappling(grappleLaunched);
}

void UPK_GrappleSystem::OnGrapple()
{
	onObjectTouch.Broadcast();
}

void UPK_GrappleSystem::HideCable(const bool _hide)
{
	if (!cable)return;
	cable->SetHiddenInGame(_hide, true);
	cable->bEnableCollision = !_hide;
	if (_hide && endCable) endCable->SetWorldLocation(owner->GetActorLocation());
}

void UPK_GrappleSystem::ShootGrapple()
{
	if (!canGrapple || !aimedGrappleGPE || (aimedGrappleGPE == targetGrappleGPE)) return;
	ResetCableComponent();
	SetCurrentTarget(aimedGrappleGPE);
	grappleLaunched = true;
	ActivateGrapple();
}

void UPK_GrappleSystem::ActivateGrapple() 
{
	if (animations) animations->SetIsGrappling(true);
}

void UPK_GrappleSystem::LaunchGrapple()
{
	if (!targetGrappleGPE || !endCable)return;
	onGrappleLaunch.Broadcast();
	HideCable(false);
	targetGrappleGPE->OnEndMoveWithGrapple()->AddLambda([this]()
	{
		onEndAutoMove.Broadcast();
	});
	endCable->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
}

void UPK_GrappleSystem::MoveGrapple(const float _tick)
{
	if (!endCable || !grappleLaunched)return;
	if (!targetGrappleGPE || !targetGrappleGPEHangPoint)
	{
		onEndAutoMove.Broadcast();
		return;
	}
	const FVector _endTarget = targetGrappleGPEHangPoint->GetComponentLocation();
	const FVector _newPos = UKismetMathLibrary::VInterpTo_Constant(endCable->GetComponentLocation(), _endTarget, _tick, speedGrapple);
	endCable->SetWorldLocation(_newPos);
	if (GrappleTouched(_endTarget)) onGrapple.Broadcast();
}

void UPK_GrappleSystem::HangGrapple()
{
	grappleLaunched = false;
	isGrappling = true;
	cable->SetAttachEndTo(targetGrappleGPE, TEXT("HangPoint"));
}

bool UPK_GrappleSystem::GrappleTouched(const FVector _endPos) const
{
	if (!endCable) return false;
	return FVector::Dist(endCable->GetComponentLocation(), _endPos) < minDistance;
}

void UPK_GrappleSystem::SetCurrentTarget(APK_GrappleGPE* _target)
{
	if (!_target) return;
	targetGrappleGPE = _target;
	targetGrappleGPEHangPoint = _target->GetHangPoint();
}

void UPK_GrappleSystem::UseTarget()
{
	if (!targetGrappleGPE) return;
	targetGrappleGPE->Use(owner);
}

void UPK_GrappleSystem::RemoveInRangeGPE(APK_GrappleGPE& _gpe)
{
	if (grappleGPEInRange.Contains(&_gpe)) grappleGPEInRange.Remove(&_gpe);
	if (aimedGrappleGPE == &_gpe) aimedGrappleGPE = nullptr;
}

void UPK_GrappleSystem::CheckAimedGPE()
{
	if (!owner || grappleGPEInRange.Num() <= 0) return;
	float _currentdotInView = minAngle;
	APK_GrappleGPE* _currentGPE = nullptr;
	for(APK_GrappleGPE* _gpe : grappleGPEInRange) 
	{
		_currentGPE = CheckGPE(_currentdotInView, *_gpe, _currentGPE);
	}
	if (aimedGrappleGPE) aimedGrappleGPE->SetAimed(false);
	aimedGrappleGPE = _currentGPE;
	if (aimedGrappleGPE) _currentGPE->SetAimed(true);
}

APK_GrappleGPE* UPK_GrappleSystem::CheckGPE(float& _currentdotInView, APK_GrappleGPE& _newGPE, APK_GrappleGPE* _currentGPE)
{
	if (!camera)return _currentGPE;
	const USceneComponent* _newGPEHangPoint = _newGPE.GetHangPoint();
	if (!_newGPEHangPoint)return _currentGPE;
	const bool _isAnObstacle = CheckObstacle(_newGPEHangPoint->GetComponentLocation());
	if (_isAnObstacle) return _currentGPE;
	float _dotInView = GetDotProductViewGPE(_newGPE);
	if (_dotInView <= _currentdotInView) return _currentGPE;
	_currentdotInView = _dotInView;
	return &_newGPE;
}

float UPK_GrappleSystem::GetDotProductViewGPE(const APK_GrappleGPE& _newGPE)
{
	if (!owner || !camera ||!&_newGPE) return minAngle;
	FVector _gpePos = _newGPE.GetActorLocation() - owner->GetActorLocation();
	_gpePos.Normalize();
	return FVector::DotProduct(camera->GetActorForwardVector(), _gpePos);
}

void UPK_GrappleSystem::ResetCableComponent()
{
	HideCable(true);
	cable->SetAttachEndTo(owner, TEXT("EndCable"));
	endCable->AttachToComponent(owner->GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false), FName("grapple"));
}

void UPK_GrappleSystem::ResetGrapple()
{
	ResetCableComponent();
	grappleLaunched = false;
	targetGrappleGPE = nullptr;
	targetGrappleGPEHangPoint = nullptr;
	aimedGrappleGPE = nullptr;
	isGrappling = false;
}

void UPK_GrappleSystem::ResetGrappleOnDeath()
{
	ResetGrapple();
	grappleGPEInRange.Empty();
}

bool UPK_GrappleSystem::CheckObstacle(const FVector _gpePos) const
{
	if (!owner) return true;
	FHitResult _hit;
	return UKismetSystemLibrary::LineTraceSingle(GetWorld(), owner->GetActorLocation(), _gpePos, ETraceTypeQuery::TraceTypeQuery1, true, owner->GetIgnoredActors() , EDrawDebugTrace::None, _hit, true);
}
