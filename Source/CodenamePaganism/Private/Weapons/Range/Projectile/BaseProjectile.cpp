// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Range/Projectile/BaseProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ABaseProjectile::ABaseProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	CollisionComponent->InitSphereRadius(5.0f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	CollisionComponent->bReturnMaterialOnMove = true;
	SetRootComponent(CollisionComponent);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	MovementComponent->ProjectileGravityScale = 1.0f;

}

void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	check(CollisionComponent);
	check(MovementComponent);

	MovementComponent->Velocity = ShotDirection * MovementComponent->InitialSpeed;
	CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);
	CollisionComponent->OnComponentHit.AddDynamic(this, &ABaseProjectile::OnProjectileHit);

	SetLifeSpan(LifeSeconds);
}
void ABaseProjectile::ShootArrow(float Shotpower)
{
	MovementComponent->InitialSpeed = CalculateVelocity(Shotpower);
}

float ABaseProjectile::CalculateVelocity(float Shotpower)
{
	return FMath::GetMappedRangeValueClamped(ShotPower, ProjectileVelocity, Shotpower);
}

void ABaseProjectile::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!GetWorld()) return;

	MovementComponent->StopMovementImmediately();

	UGameplayStatics::ApplyRadialDamage(GetWorld(),
		DamageAmount,
		GetActorLocation(),
		DamageRadius,
		UDamageType::StaticClass(),
		{},
		this,
		GetController(),
		true);

	Destroy();
}

AController* ABaseProjectile::GetController() const
{
	const auto Pawn = Cast<APawn>(GetOwner());
	return Pawn ? Pawn->GetController() : nullptr;
}
