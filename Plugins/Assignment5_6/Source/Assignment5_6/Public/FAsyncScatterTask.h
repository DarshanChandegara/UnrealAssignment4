// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class AMeshGenerator;

class ASSIGNMENT5_6_API FAsyncScatterTask : public FNonAbandonableTask
{

public:
	FAsyncScatterTask(AMeshGenerator* InActor , FVector InLocation , FVector InDimesnion);

	void DoWork();
	FVector GeneraterandomPointInSphere(FVector Origin, float Radius);
	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FAsyncScatterTask, STATGROUP_ThreadPoolAsyncTasks);
	}

private:

	TWeakObjectPtr<AMeshGenerator> MeshGenerator;

	FVector Location;

	FVector Dimension;
};
