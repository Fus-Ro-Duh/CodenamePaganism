// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "MainPlayerController.h"
#include "Components/InputComponent.h"
#include "Character/Components/HealthComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"
#include "Character/Components/BaseMovementComponent.h"
#include "Camera/PlayerCameraManager.h"

ABaseCharacter::ABaseCharacter(const FObjectInitializer& ObjInit) :
	Super(ObjInit.SetDefaultSubobjectClass<UBaseMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...
	//GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	CameraCollisionComponent = CreateDefaultSubobject<USphereComponent>("CameraCollisionComponent");
	CameraCollisionComponent->SetupAttachment(CameraComponent);
	CameraCollisionComponent->SetSphereRadius(10.0f);
	CameraCollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");

	LandedDelegate.AddDynamic(this, &ABaseCharacter::OnGroundLanded);
}

void ABaseCharacter::OnDeath()
{
	GetCharacterMovement()->DisableMovement();
	SetLifeSpan(3.0f);

	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetSimulatePhysics(true);

	if (Controller)
	{
		Controller->ChangeState(NAME_Spectating);
	}
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	check(HealthComponent);
	check(GetMesh());

	check(CameraCollisionComponent);

	CameraCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ABaseCharacter::OnCameraCollisionBeginOverlap);
	CameraCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ABaseCharacter::OnCameraCollisionEndOverlap);

	SetupPlayerInput();

	HealthComponent->OnDeath.AddUObject(this, &ABaseCharacter::OnDeath);
	//HealthComponent->OnHealthChanged.AddUObject(this, &ABaseCharacter::OnHealthChanged);
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IAMove, ETriggerEvent::Triggered, this, &ABaseCharacter::Move);
		EnhancedInputComponent->BindAction(IALook, ETriggerEvent::Triggered, this, &ABaseCharacter::Look);
		EnhancedInputComponent->BindAction(IAJump, ETriggerEvent::Started, this, &ABaseCharacter::Jump);
<<<<<<< HEAD
		EnhancedInputComponent->BindAction(IACrouch, ETriggerEvent::Started, this, &ABaseCharacter::Crouch);
		EnhancedInputComponent->BindAction(IACrouch, ETriggerEvent::Completed, this, &ABaseCharacter::UnCrouch);
		EnhancedInputComponent->BindAction(IAWalk, ETriggerEvent::Started, this, &ABaseCharacter::Walk);
		EnhancedInputComponent->BindAction(IARun, ETriggerEvent::Started, this, &ABaseCharacter::Run);
		EnhancedInputComponent->BindAction(IARun, ETriggerEvent::Completed, this, &ABaseCharacter::StopRun);
=======
>>>>>>> bf63e29b5d9d7bd4497ce49e09473e69e8401525
	}
}

void ABaseCharacter::Move(const FInputActionValue& Value)
{
	//if (ActionState != EActionState::EAS_Unoccupied) return;
<<<<<<< HEAD
=======
	bUseControllerRotationYaw = true;
>>>>>>> bf63e29b5d9d7bd4497ce49e09473e69e8401525
	const FVector2D MoveDirection = Value.Get<FVector2D>();
	
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, MoveDirection.Y);
	const FVector SideRotation = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(SideRotation, MoveDirection.X);
}

void ABaseCharacter::Look(const FInputActionValue& Value)
{
	//if (ActionState != EActionState::EAS_Unoccupied) return;

	const FVector2D LookDirection = Value.Get<FVector2D>();
	bUseControllerRotationYaw = !GetVelocity().IsZero();

	//if (!GetVelocity().IsZero())
	//{
	//	FRotator CurRotation;
	//	FVector CurLocation;
	//	UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraViewPoint(CurLocation, CurRotation);
	//	FRotator CameraRotation = FRotator(0.0f, CurRotation.Yaw, 0.0f);
	//	RootComponent->MoveComponent(FVector::ZeroVector, CameraRotation, true);
	//}

	if (GetController())	
	{
		AddControllerYawInput(LookDirection.X);
		AddControllerPitchInput(LookDirection.Y);

	}
}

void ABaseCharacter::Jump(const FInputActionValue& Value)
{
	Super::Jump();
}

void ABaseCharacter::Crouch(const FInputActionValue& Value)
{
	Super::Crouch();
}
void ABaseCharacter::UnCrouch(const FInputActionValue& Value)
{
	Super::UnCrouch();
}

void ABaseCharacter::Walk(const FInputActionValue& Value)
{
	IsWalking = !IsWalking;
	UE_LOG(LogLoad, Warning, TEXT("Walking"));
}

void ABaseCharacter::Run(const FInputActionValue& Value)
{
	IsRunning = !IsRunning;
	IsWalking = false;
	UE_LOG(LogLoad, Warning, TEXT("Running"));
}
void ABaseCharacter::StopRun(const FInputActionValue& Value)
{
	IsRunning = false;
	UE_LOG(LogLoad, Warning, TEXT("not Running"), IsRunning);
}

void ABaseCharacter::OnCameraCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckCameraOverlap();
}
void ABaseCharacter::OnCameraCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CheckCameraOverlap();
}

void ABaseCharacter::CheckCameraOverlap()
{
	const auto HideMesh = CameraCollisionComponent->IsOverlappingComponent(GetCapsuleComponent());
	GetMesh()->SetOwnerNoSee(HideMesh);

	TArray<USceneComponent*> MeshChildren;
	GetMesh()->GetChildrenComponents(true, MeshChildren);

	for (auto MeshChild : MeshChildren)
	{
		const auto MeshChildGeometry = Cast<UPrimitiveComponent>(MeshChild);
		if (MeshChildGeometry)
		{
			MeshChildGeometry->SetOwnerNoSee(HideMesh);
		}
	}
}

void ABaseCharacter::SetupPlayerInput()
{
	if (AMainPlayerController* PlayerController = Cast<AMainPlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(OnFootMappingContext, 0);
		}
	}
}

void ABaseCharacter::OnGroundLanded(const FHitResult& Hit)
{
	const auto FallVelocityZ = -GetVelocity().Z;

	if (FallVelocityZ > LandedDamageVelocity.X)
	{
		const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocityZ);
		TakeDamage(FinalDamage, FDamageEvent(), nullptr, nullptr);
	}
}