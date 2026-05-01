// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <ModularCharacter.h>

#include "CoreMinimal.h"
#include "CustomModularCharacter.generated.h"

UCLASS()
class GAMEPLAYLEARNING_API ACustomModularCharacter : public AModularCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACustomModularCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
