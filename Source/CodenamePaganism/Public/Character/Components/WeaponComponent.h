// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"

class UAnimMontage;
class ABaseMeleeWeapon;
class ABaseRangeWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CODENAMEPAGANISM_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UWeaponComponent();
	void Attack();
	void StartAiming();
	void Shoot();
	ABaseRangeWeapon* GetCurrentWeapon() { return CurrentWeapon; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName WeaponEquipSocketName = "WeaponSocket";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<ABaseRangeWeapon> Weapon;

private:	
	UPROPERTY()
	ABaseRangeWeapon* CurrentWeapon = nullptr;

	void SpawnWeapons();
	//void AttachToWeaponSocket(ABaseMeleeWeapon* Weapon, USceneComponent* SceneComponent, FName& SocketName);
	void AttachToWeaponSocket(ABaseRangeWeapon* Weapon, USceneComponent* SceneComponent, FName& SocketName);
	//void EquipWeapon(ABaseMeleeWeapon* CurrentWeapon);
};
