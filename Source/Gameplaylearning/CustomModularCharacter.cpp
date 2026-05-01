// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomModularCharacter.h"


// Sets default values
ACustomModularCharacter::ACustomModularCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACustomModularCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACustomModularCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACustomModularCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

