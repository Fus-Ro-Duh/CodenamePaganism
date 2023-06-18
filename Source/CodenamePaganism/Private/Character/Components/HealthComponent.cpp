// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Components/HealthComponent.h"
#include "TimerManager.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	check(MaxHealth > 0);
	SetHealth(MaxHealth);

	AActor* ComponentOwner = GetOwner();
	if (ComponentOwner)
	{
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnTakeAnyDamage);
	}
}

void UHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage > 0.0f && !IsDead() && GetWorld())
	{
		SetHealth(CurrentHealth - Damage);
		GetWorld()->GetTimerManager().ClearTimer(RegerateTimerHandle);
		if (IsDead())
		{
			OnDeath.Broadcast();
		}
		else if (AutoRegenerate)
		{
			GetWorld()->GetTimerManager().SetTimer(RegerateTimerHandle, this, &UHealthComponent::RegenarateHealth, RegenerateRate, true, RegenerateLag);
		}
	}
}

void UHealthComponent::RegenarateHealth()
{
	if (!IsDead())
	{
		SetHealth(CurrentHealth + RegHealth);

		if (IsHealthFull() && GetWorld())
		{
			GetWorld()->GetTimerManager().ClearTimer(RegerateTimerHandle);
		}
	}
}
void UHealthComponent::SetHealth(float NewHealth)
{
	const auto NextHealth = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
	const auto HealthDelta = NextHealth - CurrentHealth;
	CurrentHealth = NextHealth;
	OnHealthChanged.Broadcast(CurrentHealth, HealthDelta);
}

bool UHealthComponent::IsHealthFull()
{
	return FMath::IsNearlyEqual(CurrentHealth, MaxHealth);
}

bool UHealthComponent::TryToAddHealth(float HealthAmount)
{
	if (IsHealthFull() || IsDead()) return false;

	SetHealth(CurrentHealth + HealthAmount);
	return true;
}