// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Melee/BaseMeleeWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"
#include "Engine/DamageEvents.h"
#include "Utils/AnimUtils.h"
#include "Animations/SwingEndAnimNotify.h"
#include "Animations/SwingStartAnimNotify.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeapon, All, All);

ABaseMeleeWeapon::ABaseMeleeWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	SetRootComponent(WeaponMesh);
}

void ABaseMeleeWeapon::Attack()
{
	if (const auto Character = Cast<ACharacter>(GetOwner()))
	{

		Character->PlayAnimMontage(AttackAnimation);

		GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &ABaseMeleeWeapon::SwingWeapon, 0.01f, true, 0.5f);
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
		DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 3.0f, 0, 3.0f);
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
	UE_LOG(LogLoad, Warning, TEXT("SWstart"));
}

void ABaseMeleeWeapon::InitAnimations()
{
	//auto SwingStartNotify = AnimUtils::FindNotifyByClass<USwingStartAnimNotify>(AttackAnimation);
	//if (SwingStartNotify)
	//{
	//	SwingStartNotify->OnNotified.AddUObject(this, &ABaseMeleeWeapon::SwingWeapon);
	//}
	//auto SwingEndNotify = AnimUtils::FindNotifyByClass<USwingEndAnimNotify>(AttackAnimation);
	//if (SwingEndNotify)
	//{
	//	SwingEndNotify->OnNotified.AddUObject(this, &ABaseMeleeWeapon::SwingEnd);
	//}
	//else
	//{
	//	UE_LOG(LogWeapon, Error, TEXT("Swing end anim notify is forgotten to set!"));
	//	checkNoEntry();
	//}
}

void ABaseMeleeWeapon::SwingEnd()
{
	UE_LOG(LogLoad, Warning, TEXT("SWend"));
	GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
}

