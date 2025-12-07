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
	ADay04();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int64 Part1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int64 Part2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int64 Width;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int64 Height;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* AccessibleMaterial;

protected:
	virtual void BeginPlay() override;

private:
	TArray<UStaticMeshComponent*> Cubes;
	TArray<TArray<TCHAR>> Data;	
	UStaticMesh* CubeMesh;
	FTimerHandle MyTimerHandle;

	int64 SolvePart1();
	int64 SolvePart2();
	void Step();
	bool IsAccessible(int x, int y);
};
