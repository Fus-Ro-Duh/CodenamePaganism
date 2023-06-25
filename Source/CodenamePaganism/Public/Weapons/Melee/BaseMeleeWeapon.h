// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseMeleeWeapon.generated.h"

class USkeletalMeshComponent;
class UAnimMontage;

UCLASS()
class CODENAMEPAGANISM_API ABaseMeleeWeapon : public AActor
{
	GENERATED_BODY()
	
public:
	ABaseMeleeWeapon();

	FTimerHandle AttackTimerHandle;

	virtual void Attack();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sockets")
	FName StartLineSocket = "Start";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sockets")
	FName EndLineSocket = "End";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponData")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* AttackAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float DamageAmount = 10;

private:
	void GetLineTrace(FHitResult& HitResult);
	FVector GetSocketWorldLocation(FName SocketName);
	void MakeDamage(const FHitResult& HitResult);
	void SwingWeapon();
	void InitAnimations();
	void SwingEnd();
};
