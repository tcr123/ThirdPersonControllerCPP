// Fill out your copyright notice in the Description page of Project Settings.


#include "ThirdPersonController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
AThirdPersonController::AThirdPersonController()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

// Called when the game starts or when spawned
void AThirdPersonController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AThirdPersonController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AThirdPersonController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AThirdPersonController::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AThirdPersonController::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AThirdPersonController::Turn);
	PlayerInputComponent->BindAxis(TEXT("TurnRate"), this, &AThirdPersonController::TurnRate);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AThirdPersonController::LookUp);
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AThirdPersonController::LookUpRate);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Released, this, &ACharacter::StopJumping);
}

void AThirdPersonController::MoveForward(float AxisValue)
{
	FVector forward = UKismetMathLibrary::GetForwardVector(FRotator(0, GetControlRotation().Yaw, 0));
	AddMovementInput(forward, AxisValue);
}

void AThirdPersonController::MoveRight(float AxisValue)
{
	FVector right = UKismetMathLibrary::GetRightVector(FRotator(0, GetControlRotation().Yaw, 0));
	AddMovementInput(right, AxisValue);
}

void AThirdPersonController::Turn(float AxisValue)
{
	AddControllerYawInput(AxisValue);
}

void AThirdPersonController::LookUp(float AxisValue)
{
	AddControllerPitchInput(AxisValue);
}

void AThirdPersonController::TurnRate(float AxisValue)
{
	AddControllerYawInput(AxisValue * baseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AThirdPersonController::LookUpRate(float AxisValue)
{
	AddControllerPitchInput(AxisValue * baseLookUpRate * GetWorld()->GetDeltaSeconds());
}

