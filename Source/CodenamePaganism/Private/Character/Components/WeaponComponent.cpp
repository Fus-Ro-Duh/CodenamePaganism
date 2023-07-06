// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Components/WeaponComponent.h"
#include "Weapons/Melee/BaseMeleeWeapon.h"
#include "Weapons/Range/BaseRangeWeapon.h"
#include "GameFramework/Character.h"
#include "UObject/UObjectBaseUtility.h"

UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UWeaponComponent::Attack()
{
	//CurrentWeapon->Attack();
}

void UWeaponComponent::StartAiming()
{
	CurrentWeapon->Load();
}

void UWeaponComponent::Shoot()
{
	CurrentWeapon->Release();
}

void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	SpawnWeapons();
	//EquipWeapon(CurrentWeapon);
}

void UWeaponComponent::SpawnWeapons()
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character || !GetWorld()) return;

	CurrentWeapon = GetWorld()->SpawnActor<ABaseRangeWeapon>(Weapon);

	CurrentWeapon->SetOwner(Character);

	if (!CurrentWeapon) return;
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	CurrentWeapon->AttachToComponent(Character->GetMesh(), AttachmentRules, WeaponEquipSocketName);
}


void UWeaponComponent::AttachToWeaponSocket(ABaseRangeWeapon* Weap, USceneComponent* SceneComponent, FName& SocketName)
{
	if (!Weap || !SceneComponent) return;

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	Weap->AttachToComponent(SceneComponent, AttachmentRules, SocketName);
}

