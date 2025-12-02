// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Day02.generated.h"

UCLASS()
class AOC_API ADay02 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADay02();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int64 Part1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int64 Part2;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
