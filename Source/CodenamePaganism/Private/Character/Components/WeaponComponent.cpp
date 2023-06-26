// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Components/WeaponComponent.h"
#include "Weapons/Melee/BaseMeleeWeapon.h"
#include "GameFramework/Character.h"
#include "UObject/UObjectBaseUtility.h"

UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UWeaponComponent::Attack()
{
	CurrentWeapon->Attack();
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

	CurrentWeapon = GetWorld()->SpawnActor<ABaseMeleeWeapon>(MeleeWeapon);

	CurrentWeapon->SetOwner(Character);

	if (!CurrentWeapon) return;
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	CurrentWeapon->AttachToComponent(Character->GetMesh(), AttachmentRules, WeaponEquipSocketName);

	//AttachToWeaponSocket(Weapon, Character->GetMesh(), WeaponEquipSocketName);
}



/*void UWeaponComponent::AttachToWeaponSocket(ABaseMeleeWeapon* Weapon, USceneComponent* SceneComponent, FName& SocketName)
{
	if (!CurrentWeapon || !SceneComponent) return;

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);
}*/

/*void UWeaponComponent::EquipWeapon(ABaseMeleeWeapon* CurrentWeapon)
{
	if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
	{
		AttachToWeaponSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);
	}
}*/

