// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseProjectile.generated.h"

class USphereComponent;
class USTU_WeaponFXComponent;
class UProjectileMovementComponent;


UCLASS()
class CODENAMEPAGANISM_API ABaseProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseProjectile();
	void SetShotDirection(const FVector& Direction) { ShotDirection = Direction; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shot")
	USkeletalMeshComponent* ProjectileMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = "Shot")
	USphereComponent* CollisionComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Shot")
	FVector2D ShotPower = FVector2D(1.0f, 10.f);

	UPROPERTY(EditDefaultsOnly, Category = "Shot")
	FVector2D ProjectileVelocity = FVector2D(200.0f, 1200.0f);

	UPROPERTY(VisibleDefaultsOnly, Category = "Shot")
	UProjectileMovementComponent* MovementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Shot")
	float DamageRadius = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Shot")
	float DamageAmount = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Shot")
	float LifeSeconds = 5.0f;

public:	
	void ShootArrow(float Shotpower);

private:
	float CalculateVelocity(float Shotpower);

	FVector ShotDirection;

	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	AController* GetController() const;
};
