// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Melee/BaseMeleeWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"
#include "Engine/DamageEvents.h"
#include "Animations/AnimUtils.h"
#include "Animations/SwingEndAnimNotify.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeapon, All, All);

ABaseMeleeWeapon::ABaseMeleeWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateAbstractDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	SetRootComponent(WeaponMesh);
}

void ABaseMeleeWeapon::Attack()
{
	if (const auto Character = Cast<ACharacter>(GetOwner()))
	{
		Character->PlayAnimMontage(AttackAnimation);

		GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &ABaseMeleeWeapon::SwingWeapon, 0.1f, true, 0.0f);
	}
}

void ABaseMeleeWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	InitAnimations();

	check(WeaponMesh);
}

void ABaseMeleeWeapon::GetLineTrace(FHitResult& HitResult)
{
	if (GetWorld())
	{
		const auto Start = GetSocketWorldLocation(StartLineSocket);
		const auto End = GetSocketWorldLocation(EndLineSocket);

		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(GetOwner());
		CollisionParams.bReturnPhysicalMaterial = true;

		GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility, CollisionParams);
	}
}

FVector ABaseMeleeWeapon::GetSocketWorldLocation(FName SocketName)
{
	return WeaponMesh->GetSocketLocation(SocketName);
}

void ABaseMeleeWeapon::MakeDamage(const FHitResult& HitResult)
{
	if (AActor* HitActor = HitResult.GetActor())
	{
		HitActor->TakeDamage(DamageAmount, FDamageEvent(), nullptr, this);
	}
}

void ABaseMeleeWeapon::SwingWeapon()
{
	if (!GetWorld()) return;

	FHitResult HitResult;
	GetLineTrace(HitResult);

	if (HitResult.bBlockingHit)
	{
		MakeDamage(HitResult);
	}
}

void ABaseMeleeWeapon::InitAnimations()
{
	auto SwingEndNotify = AnimUtils::FindNotifyByClass<USwingEndAnimNotify>(AttackAnimation);
	if (SwingEndNotify)
	{
		SwingEndNotify->OnNotified.AddUObject(this, &ABaseMeleeWeapon::SwingEnd);
	}
	else
	{
		UE_LOG(LogWeapon, Error, TEXT("Swing end anim notify is forgotten to set!"));
		checkNoEntry();
	}
}

void ABaseMeleeWeapon::SwingEnd()
{
	GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
}


