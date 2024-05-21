#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FenceMeshActor.generated.h"

UCLASS()
class ASSIGNMENT5_6_API AFenceMeshActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AFenceMeshActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
