// Fill out your copyright notice in the Description page of Project Settings.


#include "PK_MovementBehaviour.h"
#include "PK_MainCharacter.h"
#include "PK_WallRunSystem.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UPK_MovementBehaviour::UPK_MovementBehaviour()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UPK_MovementBehaviour::BeginPlay()
{
	Super::BeginPlay();
	InitOwner();
}


// Called every frame
void UPK_MovementBehaviour::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPK_MovementBehaviour::InitOwner()
{
	owner = Cast<APK_MainCharacter>(GetOwner());
	InitOwnerComponents();
	InitDelegate();
	InitOwnerDelegate();
}

void UPK_MovementBehaviour::InitOwnerDelegate()
{
	if (!owner) return;
	owner->OnUpdate()->AddLambda([this](float _tick)
	{
		onUpdate.Broadcast();
	});
}

void UPK_MovementBehaviour::InitDelegate()
{
	onUpdate.AddLambda([this]()
	{
		if (ownerMovement) isRunning = ownerMovement->Velocity.Size() > 0;
	});
}

void UPK_MovementBehaviour::InitOwnerComponents()
{
	if (!owner) return;
	ownerMovement = owner->GetCharacterMovement();
	ownerWallrunSystem = owner->GetWallRunSystem();
}

void UPK_MovementBehaviour::MoveForward(const float _direction)
{
	if (!owner || !canMove || !canMoveOnX) return;
	owner->AddMovementInput(owner->GetActorForwardVector(), _direction*speedMoveX, false);
}

void UPK_MovementBehaviour::MoveLateral(const float _direction)
{
	if (!owner || !canMove || !canMoveOnY) return;
	owner->AddMovementInput(owner->GetActorRightVector(), _direction * speedMoveY, false);
}

void UPK_MovementBehaviour::CharacterJump()
{
	if (!owner || !canJump || !ownerMovement->IsMovingOnGround()) return;
	onJump.Broadcast();
	owner->LaunchCharacter(FVector(owner->GetVelocity().X*jumpOnX,owner->GetVelocity().Y* jumpOnY,jumpHeight),true,true);
}

void UPK_MovementBehaviour::LookVertical(const float _direction)
{
	if (!owner || !canTurn) return;
	const bool _isKeyboard = owner->GetIsKeyboard();

	FRotator _characterRotator = owner->GetMesh()->GetRelativeRotation();
	owner->GetMesh()->SetRelativeRotation(_characterRotator.Add(0,0,-_direction * (_isKeyboard ? sensibilityYMouse : sensibilityYGamepad)));
	ClampVerticalView(*owner->GetMesh());
}

void UPK_MovementBehaviour::LookHorizontal(const float _direction)
{
	if (!owner || !canTurn) return;
	const bool _isKeyboard = owner->GetIsKeyboard();
	FRotator _characterRotator = owner->GetActorRotation();
	FRotator _newRotator = _characterRotator.Add(0, _direction * (_isKeyboard ? sensibilityXMouse : sensibilityXGamepad), 0);
	owner->SetActorRotation(_newRotator);
}

void UPK_MovementBehaviour::ClampVerticalView(USceneComponent& _component)
{
	if (!clampVertical) return;
	FRotator _characterRotator = _component.GetRelativeRotation(); 
	const bool _tooHigh = _characterRotator.Roll > baseClampVertical + clampUp;
	const bool _tooLow = _characterRotator.Roll < baseClampVertical + clampDown;
	if (_tooHigh) _characterRotator.Roll = baseClampVertical + clampUp;
	else if (_tooLow) _characterRotator.Roll = baseClampVertical + clampDown;
	_component.SetRelativeRotation(_characterRotator);
}

