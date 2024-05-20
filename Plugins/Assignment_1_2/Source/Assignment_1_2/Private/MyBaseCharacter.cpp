// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBaseCharacter.h"

AMyBaseCharacter::AMyBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AMyBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMyBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

