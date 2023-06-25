// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Components/WeaponComponent.h"
#include "Weapons/Melee/BaseMeleeWeapon.h"
#include "GameFramework/Character.h"

UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UWeaponComponent::Attack()
{
	MeleeWeapon->Attack();
}

void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	SpawnWeapons();
	EquipWeapon(MeleeWeapon);
}

void UWeaponComponent::SpawnWeapons()
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character || !GetWorld()) return;

	auto Weapon = GetWorld()->SpawnActor<ABaseMeleeWeapon>();

	Weapon->SetOwner(Character);

	AttachToWeaponSocket(Weapon, Character->GetMesh(), WeaponEquipSocketName);
}

void UWeaponComponent::AttachToWeaponSocket(ABaseMeleeWeapon* Weapon, USceneComponent* SceneComponent, FName& SocketName)
{
	if (!Weapon || !SceneComponent) return;

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);
}

void UWeaponComponent::EquipWeapon(ABaseMeleeWeapon* CurrentWeapon)
{
	if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
	{
		AttachToWeaponSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);
	}
}

