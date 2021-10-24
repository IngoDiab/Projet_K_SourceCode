// Fill out your copyright notice in the Description page of Project Settings.


#include "PK_MCAnimation.h"

void UPK_MCAnimation::UseShoot()
{
	onShoot.Broadcast();
}

void UPK_MCAnimation::UseGrapple()
{
	onGrapple.Broadcast();
}
