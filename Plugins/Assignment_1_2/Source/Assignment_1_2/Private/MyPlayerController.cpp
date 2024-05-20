// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "MyFirstPersonPawn.h"
#include "Engine/World.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "TP_ThirdPerson/TP_ThirdPersonCharacter.h"
#include "TP_TopDown/TP_TopDownCharacter.h"

AMyPlayerController::AMyPlayerController()
{
	bShowMouseCursor = true;
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent(); 
	mapping = NewObject<UInputMappingContext>(); 
	possesAction = NewObject<UInputAction>(); 
	possesAction->ValueType = EInputActionValueType::Boolean; 

	FEnhancedActionKeyMapping map =  mapping->MapKey(possesAction , EKeys::P); 
	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent); 

	EIC->BindAction(possesAction , ETriggerEvent::Completed , this , &AMyPlayerController::spawn); 

	ULocalPlayer* LocalPlayer = GetLocalPlayer();  
	check(LocalPlayer); 
	UEnhancedInputLocalPlayerSubsystem* Subsystem = 
		LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(); 
	check(Subsystem); 

	Subsystem->AddMappingContext(mapping, 0); 
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Press P KEY To Possees and Toggle Another Character"); 
	bShowMouseCursor = true;
	spawn();
}

void AMyPlayerController::spawn()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, "Spawn"); 

	Table = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/Assignment2/PawnDataTable.PawnDataTable'"));
	if (Table)
	{
		TArray<FName> RowNames = Table->GetRowNames();

		FName RandomRowName = RowNames[Index % RowNames.Num()];  
		Index++; 

		FPawnDataTable* RowData = Table->FindRow<FPawnDataTable>(RandomRowName, TEXT(""));

		if (RowData) {
			auto oldType = pawnType;
			pawnType = RowData->PawnType;
			UClass* characterToSpawn = RowData->Pawn;
			if (characterToSpawn) {
				FActorSpawnParameters params;
				params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				FVector location(50, 50, 50);
				FRotator rotate = FRotator::ZeroRotator;

				AActor* spawnActor = GetWorld()->SpawnActor<AActor>(characterToSpawn, location, rotate, params);
				if (spawnActor) {
					APawn* reference = Cast<APawn>(spawnActor); 
						if (reference) {

							UEnhancedInputLocalPlayerSubsystem* Subsystem =
								ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

						
							auto old = currentPawn;
							Possess(reference);

							if(old)
							old->Destroy();
							currentPawn = reference;  

							switch (pawnType) { 
							case EPawnType::FirstPerson: 

								//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, "FPP"); 
								if (Cast<AMyFirstPersonPawn>(currentPawn)) {   
									DisplayAttributes(Cast<AMyFirstPersonPawn>(currentPawn)->PawnAttributeAsset);  
								}
								break;


							case EPawnType::ThirdPerson: 

								GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "TPP");
								if (Cast<ATP_ThirdPersonCharacter>(currentPawn))  { 
									DisplayAttributes(Cast<ATP_ThirdPersonCharacter>(currentPawn)->PawnAttributeAsset); 
								}
								break;

							case EPawnType::TopDown:

								//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, "TopDown"); 
								if (Cast<ATP_TopDownCharacter>(currentPawn)) { 
									DisplayAttributes(Cast<ATP_TopDownCharacter>(currentPawn)->PawnAttributeAsset);  
								}
								break;

							}
						
						}
					}
				}

			}
		}
	
}

void AMyPlayerController::OnPossess(APawn* pawn) {
	Super::OnPossess(pawn);

	ULocalPlayer* LocalPlayer = GetLocalPlayer(); 
	check(LocalPlayer); 
	UEnhancedInputLocalPlayerSubsystem* Subsystem = 
		LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(); 
	check(Subsystem);

	Subsystem->AddMappingContext(mapping, 0); 
}