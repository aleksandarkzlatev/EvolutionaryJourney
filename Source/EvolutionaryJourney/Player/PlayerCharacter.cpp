#include "EvolutionaryJourney/Player/PlayerCharacter.h"
#include "Camera/CameraComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);
	Camera->bUsePawnControlRotation = true;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::LookUp);

}

void APlayerCharacter::MoveForward(float InputNumber)
{
	// Gets the direction at which the character is looking
	FVector ForwardDirection = GetActorForwardVector();
	AddMovementInput(ForwardDirection, InputNumber);
}

void APlayerCharacter::MoveRight(float InputNumber)
{
	FVector RightDirection = GetActorRightVector();
	AddMovementInput(RightDirection, InputNumber);
}

void APlayerCharacter::Turn(float InputNumber)
{
	AddControllerYawInput(InputNumber);
}

void APlayerCharacter::LookUp(float InputNumber)
{
	AddControllerPitchInput(InputNumber);
}