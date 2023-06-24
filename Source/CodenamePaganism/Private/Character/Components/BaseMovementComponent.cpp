// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Components/BaseMovementComponent.h"
#include "Character/BaseCharacter.h"

float UBaseMovementComponent::GetMaxSpeed() const
{
	const float MaxSpeed = Super::GetMaxSpeed();
	const ABaseCharacter* Player = Cast<ABaseCharacter>(GetPawnOwner());
	if (Player)
	{
		if (Player->IsRunning) return MaxSpeed * RunModifier;
		if (Player->IsWalking) return MaxSpeed * WalkModifier;
	}
	return MaxSpeed;
}