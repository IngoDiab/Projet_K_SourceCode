// Fill out your copyright notice in the Description page of Project Settings.


#include "PK_MainCharacter.h"
#include "PK_MovementBehaviour.h"
#include "PK_WallRunSystem.h"
#include "PK_SlideSystem.h"
#include "PK_ClimbSystem.h"
#include "PK_AutoMoveSystem.h"
#include "Shoot/PK_ShootSystem.h"
#include "PK_GPEUsingSystem.h"
#include "PK_GrappleSystem.h"
#include "PK_TimeDilationPower.h"
#include "Projet_K/MainCharacter/Camera/PK_Camera.h"
#include "Projet_K/MainCharacter/Animations/PK_MCAnimation.h"
#include "Projet_K/UI/PK_HUD.h"

#include "DrawDebugHelpers.h"

#include "InputCoreTypes.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "../../../Plugins/Runtime/CableComponent/Source/CableComponent/Classes/CableComponent.h"
#include "Projet_K/Projet_KGameModeBase.h"
#include "Projet_K/Utils/Debug.h"


// Sets default values
APK_MainCharacter::APK_MainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	movementBehaviour = CreateDefaultSubobject<UPK_MovementBehaviour>(TEXT("MovementBehaviour"));
	AddOwnedComponent(movementBehaviour);
	
	wallRunSystem = CreateDefaultSubobject<UPK_WallRunSystem>(TEXT("WallRunSystem"));
	AddOwnedComponent(wallRunSystem);
	
	slideSystem = CreateDefaultSubobject<UPK_SlideSystem>(TEXT("SlideSystem"));
	AddOwnedComponent(slideSystem);
	
	climbSystem = CreateDefaultSubobject<UPK_ClimbSystem>(TEXT("ClimbSystem"));
	AddOwnedComponent(climbSystem);

	autoMoveSystem = CreateDefaultSubobject<UPK_AutoMoveSystem>(TEXT("AutoMoveSystem"));
	AddOwnedComponent(autoMoveSystem);

	shootSystem = CreateDefaultSubobject<UPK_ShootSystem>(TEXT("ShootSystem"));
	AddOwnedComponent(shootSystem);
	
	timeDilationPower = CreateDefaultSubobject<UPK_TimeDilationPower>(TEXT("TimeDilationPower"));
	AddOwnedComponent(timeDilationPower);

	GPESystem = CreateDefaultSubobject<UPK_GPEUsingSystem>(TEXT("GPESystem"));
	AddOwnedComponent(GPESystem);
	
	grappleSystem = CreateDefaultSubobject<UPK_GrappleSystem>(TEXT("GrappleSystem"));
	AddOwnedComponent(grappleSystem);

	cameraSlot = CreateDefaultSubobject<USceneComponent>(TEXT("CameraTarget"));
	cameraSlot->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), FName("camera"));

	endCable = CreateDefaultSubobject<USceneComponent>(TEXT("EndCable"));
	endCable->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	cable = CreateDefaultSubobject<UCableComponent>(TEXT("Cable"));
	cable->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), FName("grapple"));
}

// Called when the game starts or when spawned
void APK_MainCharacter::BeginPlay()
{
	Super::BeginPlay();
	InitMainCharacter();
}

// Called every frame
void APK_MainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	onUpdate.Broadcast(DeltaTime);
}

// Called to bind functionality to input
void APK_MainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("InputController", IE_Pressed, this, &APK_MainCharacter::SetIsKeyboard);

	PlayerInputComponent->BindAxis("Vertical", this, &APK_MainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Horizontal", this, &APK_MainCharacter::MoveLateral);

	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &APK_MainCharacter::Shoot);
	PlayerInputComponent->BindAction("TimeDilate", IE_Pressed, this, &APK_MainCharacter::TimeDilate);

	PlayerInputComponent->BindAction("Grapple", IE_Pressed, this, &APK_MainCharacter::ShootGrapple);

	PlayerInputComponent->BindAxis("VerticalLook", this, &APK_MainCharacter::LookVertical);
	PlayerInputComponent->BindAxis("HorizontalLook", this, &APK_MainCharacter::LookHorizontal);

	PlayerInputComponent->BindAction("Jump",IE_Pressed, this, &APK_MainCharacter::JumpAction);

	PlayerInputComponent->BindAction("Slide",IE_Pressed, this, &APK_MainCharacter::Slide);
	PlayerInputComponent->BindAction("Slide",IE_Released, this, &APK_MainCharacter::EndSlide);

	PlayerInputComponent->BindAction("Interact",IE_Pressed, this, &APK_MainCharacter::InteractWithGPE);
}

void APK_MainCharacter::InitMainCharacter()
{
	moveComponent = GetCharacterMovement();
	InitDelegates();
	InitParams();
	InitAnimations();
	InitHUD();
	InitGameMod();
	CreateCamera();
}

void APK_MainCharacter::InitHUD()
{
	APlayerController* _controller = GetWorld()->GetFirstPlayerController();
	if (!_controller)return;
	hud = Cast<APK_HUD>(_controller->GetHUD());
}

void APK_MainCharacter::InitAnimations()
{
	USkeletalMeshComponent* _mesh = GetMesh();
	if (!_mesh) return;
	animations = Cast<UPK_MCAnimation>(_mesh->GetAnimInstance());
}

void APK_MainCharacter::InitParams()
{
	ignore.Add(this);
	UCharacterMovementComponent* _movement = GetCharacterMovement();
	if (!_movement) return;
	_movement->MaxWalkSpeed = characterMaxSpeed;
	_movement->MaxAcceleration = characterMaxAcceleration;
	_movement->GravityScale = characterGravity;
}

void APK_MainCharacter::InitDelegates()
{
	onUpdate.AddLambda([this](float _tick)
	{
		RefreshIsFalling();
		RefreshAnimations();
	});
	LandedDelegate.AddDynamic(this, &APK_MainCharacter::ResetJumpCount);
}

void APK_MainCharacter::InitGameMod()
{
	AProjet_KGameModeBase* _gameMode = Cast<AProjet_KGameModeBase>(GetWorld()->GetAuthGameMode());
	FString _result = "GameMode == ";
	_result.Append(_gameMode ? "Valid" : "Invalid");
	Debug::ScreenMessage(_result, FColor::Red, 20.0f);
	if (!_gameMode) return;
	_gameMode->SetMainCharacter(this);
}

void APK_MainCharacter::CreateCamera()
{
	camera = GetWorld()->SpawnActor<APK_Camera>(GetActorLocation(), GetActorRotation());
	if (!camera) return;
	camera->InitCameraSettings(cameraSettings);
	camera->SetTargetToFollow(cameraSlot);
	camera->SetTargetToFocus(cameraSlot);
	APlayerController* _controller = GetWorld()->GetFirstPlayerController();
	if (!_controller)return;
	_controller->SetViewTarget(camera);
}

void APK_MainCharacter::SetIsKeyboard(FKey _key)
{
	isKeyboard = !_key.IsGamepadKey();
}

void APK_MainCharacter::RefreshAnimations()
{
	if(!animations) return;
	animations->SetIsJumping(isJumping);
	animations->SetIsFalling(isFalling);
	if(slideSystem) animations->SetIsSliding(slideSystem->IsSliding());
	if(GetCharacterMovement())
	animations->SetVelocity(GetCharacterMovement()->Velocity.Size());
}

void APK_MainCharacter::RefreshIsFalling()
{
	RefreshIsFallingGround();
	RefreshIsFallingWallrun();
}

void APK_MainCharacter::RefreshIsFallingGround() 
{
	if (!moveComponent) return;
	isFalling = !moveComponent->IsMovingOnGround() && moveComponent->Velocity.Z < 0;
	isJumping = !moveComponent->IsMovingOnGround() && moveComponent->Velocity.Z > 0;
}

void APK_MainCharacter::RefreshIsFallingWallrun() 
{
	if (!wallRunSystem) return;
	isFalling &= !wallRunSystem->IsWallRuning();
	isJumping &= !wallRunSystem->IsWallRuning();
}

void APK_MainCharacter::MoveForward(const float _direction)
{
	if (wallRunSystem && wallRunSystem->IsWallRuning())
		wallRunSystem->MoveOnWall(_direction);
	else if (movementBehaviour)
		movementBehaviour->MoveForward(_direction);
}

void APK_MainCharacter::MoveLateral(const float _direction)
{
	if (slideSystem && slideSystem->IsSliding())
		slideSystem->MoveLateralOnSlide(_direction);	
	else if (movementBehaviour)
		movementBehaviour->MoveLateral(_direction);
}

void APK_MainCharacter::Shoot()
{
	if (!shootSystem) return;
	shootSystem->Shoot();
}

void APK_MainCharacter::TimeDilate()
{
	if (!timeDilationPower) return;
	timeDilationPower->IsTimeDilated() ? timeDilationPower->StopTimeDilate() : timeDilationPower->TimeDilate();
}

void APK_MainCharacter::ShootGrapple()
{
	if (!grappleSystem) return;
	grappleSystem->ShootGrapple();
}

void APK_MainCharacter::JumpAction()
{
	if(CheckSlideOO()) return;
	Jump();
}

void APK_MainCharacter::Jump() 
{
	if (countJump >= countJumpMax)return;

	if (wallRunSystem && wallRunSystem->IsWallRuning())
		wallRunSystem->WallJump();
	else if (movementBehaviour)
		movementBehaviour->CharacterJump();

	isJumping = true;
	countJump++;
}

void APK_MainCharacter::ResetJumpCount(const FHitResult& _hit)
{
	isJumping = false;
	countJump = 0;
}

void APK_MainCharacter::ResetJumpCount()
{
	isJumping = false;
	countJump = 0;
}

void APK_MainCharacter::Slide()
{
	if (!slideSystem)return;
	slideSystem->PlayerSlide();
}

void APK_MainCharacter::EndSlide()
{
	if (!slideSystem)return;
	slideSystem->PlayerEndSlide();
}

bool APK_MainCharacter::CheckSlideOO()
{
	if (!slideSystem)return false;
	return slideSystem->CheckObstacle();
}

void APK_MainCharacter::InteractWithGPE()
{
	if (!GPESystem) return;
	GPESystem->Interact();
}

void APK_MainCharacter::LookVertical(const float _direction)
{
	if (!movementBehaviour) return;
	movementBehaviour->LookVertical(_direction);
}

void APK_MainCharacter::LookHorizontal(const float _direction)
{
	if (!movementBehaviour) return;
	movementBehaviour->LookHorizontal(_direction);
}

void APK_MainCharacter::ActivateAllMovements(const bool _activate)
{
	ActivateMove(_activate);
	ActivateSlide(_activate);
	ActivateWallrun(_activate);
	ActivateClimb(_activate);
	ActivateGrapple(_activate);
}

void APK_MainCharacter::ActivateMove(const bool _activate) 
{
	if (!movementBehaviour) return;
	movementBehaviour->SetCanMove(_activate);
	movementBehaviour->SetCanJump(_activate);
}

void APK_MainCharacter::ActivateSlide(const bool _activate)
{
	if (!slideSystem) return;
	slideSystem->SetCanSlide(_activate);
	slideSystem->SetCanSlideOO(_activate);
}

void APK_MainCharacter::ActivateWallrun(const bool _activate)
{
	if (wallRunSystem) wallRunSystem->SetCanWallRun(_activate);
}

void APK_MainCharacter::ActivateClimb(const bool _activate)
{
	if (climbSystem) climbSystem->SetCanClimb(_activate);
}

void APK_MainCharacter::ActivateGrapple(const bool _activate)
{
	if (grappleSystem) grappleSystem->SetCanGrapple(_activate);
}

void APK_MainCharacter::SetCooldownForSlowMo()
{
	mIsInCooldownSlowMo = true;
	FTimerDelegate _delegate;
	_delegate.BindUFunction(this, "ResetCooldownForSlowMo");
	GetWorld()->GetTimerManager().SetTimer(mHandleSlowMo, _delegate, mCooldownSlowMo, false);
}

void APK_MainCharacter::ResetCooldownForSlowMo()
{
	mIsInCooldownSlowMo = false;
}


bool APK_MainCharacter::IsInCooldownSlowMo() const
{
	return mIsInCooldownSlowMo;
}