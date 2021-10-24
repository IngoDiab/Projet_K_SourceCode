// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PK_MCAnimation.generated.h"

/**
 * 
 */
UCLASS()
class PROJET_K_API UPK_MCAnimation : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float velocity = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool isJumping = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool isFalling = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool isSliding = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isShooting = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isGrappling = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float timer = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float timerMax = 0;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FShootAnim);
	UPROPERTY(BlueprintAssignable)
		FShootAnim onShoot;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGrappleAnim);
	UPROPERTY(BlueprintAssignable)
		FGrappleAnim onGrapple;

public:
	FORCEINLINE FShootAnim* GetOnShoot() { return &onShoot; };
	FORCEINLINE FGrappleAnim* GetOnGrapple() { return &onGrapple; };

	FORCEINLINE void SetVelocity(const float _velocity) { velocity = _velocity; };
	FORCEINLINE void SetIsJumping(const bool _isJumping) { isJumping = _isJumping; };
	FORCEINLINE void SetIsFalling(const bool _isFalling) { isFalling = _isFalling; };
	FORCEINLINE void SetIsSliding(const bool _isSliding) { isSliding = _isSliding; };

	FORCEINLINE bool GetIsShooting()const { return isShooting; };
	FORCEINLINE void SetIsShooting(const bool _isShooting) { isShooting = _isShooting; };

	FORCEINLINE bool GetIsGrappling()const { return isGrappling; };
	FORCEINLINE void SetIsGrappling(const bool _isGrappling) { isGrappling = _isGrappling; };

public:
	UFUNCTION(BlueprintCallable)
	void UseShoot();

	UFUNCTION(BlueprintCallable)
	void UseGrapple();
};
