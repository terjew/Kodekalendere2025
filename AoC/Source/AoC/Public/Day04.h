// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Misc/Char.h"
#include "Day04.generated.h"

UCLASS()
class AOC_API ADay04 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADay04();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int64 Part1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int64 Part2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int64 Width;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int64 Height;

	TArray<UStaticMeshComponent*> Cubes;

	TArray<TArray<TCHAR>> Data;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* AccessibleMaterial;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UStaticMesh* CubeMesh;
	FTimerHandle MyTimerHandle;
	int64 SolvePart1();
	int64 SolvePart2();
	void Step();
	bool IsAccessible(int x, int y);
};
