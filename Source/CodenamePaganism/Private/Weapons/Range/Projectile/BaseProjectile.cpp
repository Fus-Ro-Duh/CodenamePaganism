// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Range/Projectile/BaseProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ABaseProjectile::ABaseProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMesh = CreateDefaultSubobject<USkeletalMeshComponent>("ProjectileMesh");
	ProjectileMesh->SetupAttachment(RootComponent);
	ProjectileMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionComponent");
	CollisionComponent->InitSphereRadius(5.0f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	CollisionComponent->bReturnMaterialOnMove = true;
	CollisionComponent->SetupAttachment(RootComponent);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	MovementComponent->ProjectileGravityScale = 0.0f;
}

void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	check(ProjectileMesh);
	check(CollisionComponent);
	check(MovementComponent);

	MovementComponent->InitialSpeed = 0.f;
	CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);
	CollisionComponent->OnComponentHit.AddDynamic(this, &ABaseProjectile::OnProjectileHit);
}
void ABaseProjectile::ShootArrow(float Shotpower)
{
	MovementComponent->SetVelocityInLocalSpace(FVector(0, 0, CalculateVelocity(Shotpower)));
	MovementComponent->ProjectileGravityScale = 1.0f;
}

float ABaseProjectile::CalculateVelocity(float Shotpower)
{
	return FMath::GetMappedRangeValueClamped(ShotPower, ProjectileVelocity, Shotpower);
}

void ABaseProjectile::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (GetWorld())
	{
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
	}
	
	Destroy();
}

AController* ABaseProjectile::GetController() const
{
	const auto Pawn = Cast<APawn>(GetOwner());
	return Pawn ? Pawn->GetController() : nullptr;
}
