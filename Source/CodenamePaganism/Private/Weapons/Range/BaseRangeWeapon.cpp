// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Range/BaseRangeWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraShakeBase.h"
#include "Weapons/Range/Projectile/BaseProjectile.h"
#include "Animations/AnimUtils.h"
#include "Animations/ArrowSpawnAnimNotify.h"
#include "Animations/LoadAnimNotify.h"
#include "Engine/EngineTypes.h"
#include "Weapons/Melee/BaseMeleeWeapon.h"

ABaseRangeWeapon::ABaseRangeWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateAbstractDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	SetRootComponent(WeaponMesh);
}

void ABaseRangeWeapon::Load()
{
	UE_LOG(LogLoad, Warning, TEXT("Load"));
	if (const auto Character = Cast<ACharacter>(GetOwner()))
	{
		Character->PlayAnimMontage(LoadAnimation);
	}
}

void ABaseRangeWeapon::Release()
{
	UE_LOG(LogLoad, Warning, TEXT("Release"));
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(LoadTimerHandle);	
	}



	if (CurArrowProjectile)
	{

		CurArrowProjectile->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurArrowProjectile->ShootArrow(ShotPower);
		if (const auto Character = Cast<ACharacter>(GetOwner()))
		{
			UE_LOG(LogLoad, Warning, TEXT("Check"));
			Character->PlayAnimMontage(ReleaseAnimation);
		}
	}
	ShotPower = 0.0f;
	CameraShake = 1.0f;

}

void ABaseRangeWeapon::BeginPlay()
{

	Super::BeginPlay();

	check(WeaponMesh);
	InitAnimations();
}

void ABaseRangeWeapon::IncreaseShotPower()
{
	UE_LOG(LogLoad, Warning, TEXT("Increase"));
	ShotPower = FMath::Clamp(ShotPower++, 0.0f, MaxShotPower);
	if (FMath::IsNearlyEqual(ShotPower,MaxShotPower))
	{
		IncreaseCameraShake();
	}
}

void ABaseRangeWeapon::IncreaseCameraShake()
{
	CameraShake = FMath::Clamp(CameraShake++, 0.0f, MaxShotPower);
	if (const auto CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0))
	{
		if (CameraManager) {
			CameraManager->PlayWorldCameraShake(
				GetWorld(),
				CameraShakeBase,
				GetOwner()->GetActorLocation(),
				0.0f,
				50.0f,
				1,
				false
			);
		}
	}

}

void ABaseRangeWeapon::StartIncreasingPower()
{
	GetWorld()->GetTimerManager().SetTimer(LoadTimerHandle, this, &ABaseRangeWeapon::IncreaseShotPower, PowerIncreaseRate, true);
}

void ABaseRangeWeapon::InitAnimations()
{
	auto ArrowSpawnNotify = AnimUtils::FindNotifyByClass<UArrowSpawnAnimNotify>(LoadAnimation);
	if (ArrowSpawnNotify)
	{
		ArrowSpawnNotify->OnNotified.AddUObject(this, &ABaseRangeWeapon::SpawnArrow);
	}
	auto LoadNotify = AnimUtils::FindNotifyByClass<ULoadAnimNotify>(LoadAnimation);
	if (LoadNotify)
	{
		LoadNotify->OnNotified.AddUObject(this, &ABaseRangeWeapon::StartIncreasingPower);
	}
	if(!ArrowSpawnNotify || !LoadNotify)
	{
		checkNoEntry();
	}
}

void ABaseRangeWeapon::SpawnArrow()
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character || !GetWorld()) return;
	
	CurArrowProjectile = GetWorld()->SpawnActor<ABaseProjectile>(ArrowProjectile);
	if (!CurArrowProjectile) return;

	CurArrowProjectile->SetOwner(Character);
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);

	CurArrowProjectile->AttachToComponent(Character->GetMesh(), AttachmentRules, ArrowSocketName);
}
