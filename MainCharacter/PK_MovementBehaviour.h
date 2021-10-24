// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PK_MovementBehaviour.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJET_K_API UPK_MovementBehaviour : public UActorComponent
{
	GENERATED_BODY()

	#pragma region Owner
	UPROPERTY()
	class APK_MainCharacter* owner = nullptr;

	UPROPERTY()
	class UCharacterMovementComponent* ownerMovement = nullptr;
	
	UPROPERTY()
	class UPK_WallRunSystem* ownerWallrunSystem = nullptr;
	#pragma endregion

protected:
	#pragma region Movements
	UPROPERTY(VisibleAnywhere, Category = "Movements", meta = (DisplayName = "Player is running"), BlueprintReadWrite)
	bool isRunning = false;

	UPROPERTY(EditAnywhere, Category = "Movements", meta = (DisplayName = "Player can move"))
	bool canMove = true;

	UPROPERTY(EditAnywhere, Category = "Movements", meta = (DisplayName = "Player can move right/left"))
	bool canMoveOnY = true;
	
	UPROPERTY(EditAnywhere, Category = "Movements", meta = (DisplayName = "Player forward/backward speed"))
	float speedMoveY = 1;
	
	UPROPERTY(EditAnywhere, Category = "Movements", meta = (DisplayName = "Player can move forward/backward"))
	bool canMoveOnX = true;

	UPROPERTY(EditAnywhere, Category = "Movements", meta = (DisplayName = "Player lateral speed"))
	float speedMoveX = 1;

	UPROPERTY(EditAnywhere, Category = "Movements", meta = (DisplayName = "Player can Jump"))
	bool canJump = true;
	
	UPROPERTY(EditAnywhere, Category = "Movements", meta = (DisplayName = "Multiply velocity on X at jump"))
	float jumpOnX = 1;
	
	UPROPERTY(EditAnywhere, Category = "Movements", meta = (DisplayName = "Multiply velocity on Y at jump"))
	float jumpOnY = 1;
	
	UPROPERTY(EditAnywhere, Category = "Movements", meta = (DisplayName = "Jump height"))
	float jumpHeight = 750;
	#pragma endregion

	#pragma region Rotation
	UPROPERTY(EditAnywhere, Category = "Rotation", meta = (DisplayName = "Player can rotate"))
	bool canTurn = true;
	#pragma endregion

	#pragma region Vertical
	UPROPERTY(EditAnywhere, Category = "Rotation | Clamp", meta = (DisplayName = "View Up/Down is clamped"))
	bool clampVertical = true;

	UPROPERTY()
	float baseClampVertical = 0;

	UPROPERTY(EditAnywhere, Category = "Rotation | Clamp", meta = (DisplayName = "View up max"))
	float clampUp = 75;

	UPROPERTY(EditAnywhere, Category = "Rotation | Clamp", meta = (DisplayName = "View down max"))
	float clampDown = -75;
	#pragma endregion

	UPROPERTY(EditAnywhere, Category = "Sensi | Mouse", meta = (DisplayName = "Sensibility on X Mouse"))
	float sensibilityXMouse = 1.8f;
	
	UPROPERTY(EditAnywhere, Category = "Sensi | Mouse", meta = (DisplayName = "Sensibility on Y Mouse"))
	float sensibilityYMouse = 1.8f;

	UPROPERTY(EditAnywhere, Category = "Sensi | Gamepad", meta = (DisplayName = "Sensibility on X Controller"))
	float sensibilityXGamepad = 2;

	UPROPERTY(EditAnywhere, Category = "Sensi | Gamepad", meta = (DisplayName = "Sensibility on Y Controller"))
	float sensibilityYGamepad = 1.5f;
	#pragma endregion


protected:
	DECLARE_EVENT(UPK_MovementBehaviour, Update)
	Update onUpdate;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FJump);
	UPROPERTY(BlueprintAssignable)
		FJump onJump;

public:	
	// Sets default values for this component's properties
	UPK_MovementBehaviour();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


public:
	FORCEINLINE FJump* OnJump() { return &onJump; };

	FORCEINLINE void SetClampVertical(const bool _clamp) { clampVertical = _clamp; };
	FORCEINLINE bool GetClampVertical() const { return clampVertical; };
	FORCEINLINE bool IsRunning() const { return isRunning; };

	FORCEINLINE void SetCanMove(const bool _canMove) { canMove = _canMove; };
	FORCEINLINE void SetCanMoveY(const bool _canMove) { canMoveOnY = _canMove; };
	FORCEINLINE void SetSpeedY(const float _speed) { speedMoveY = _speed; };
	FORCEINLINE void SetCanMoveX(const bool _canMove) { canMoveOnX = _canMove; };
	FORCEINLINE void SetSpeedX(const float _speed) { speedMoveX = _speed; };
	FORCEINLINE void SetCanJump(const bool _canJump) { canJump = _canJump; };


public:
	void InitOwner();

	void InitOwnerDelegate();

	void InitDelegate();

	void InitOwnerComponents();

	//Movements
	void MoveForward(const float _direction);
	void MoveLateral(const float _direction);
	void CharacterJump();

	//TurnAround
	void LookVertical(const float _direction);
	void LookHorizontal(const float _direction);

	void ClampVerticalView(USceneComponent& _component);
};
