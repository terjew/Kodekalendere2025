#pragma once

#include "DayBase.h"
#include "Day07.generated.h"

UCLASS()
class AOC_API ADay07 : public ADayBase
{
	GENERATED_BODY()
	
public:	
	ADay07();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int64 Width;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int64 Height;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* ActivatedMaterial;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* CounterMaterial;

protected:
	virtual void BeginPlay() override;

private:
	TArray<UStaticMeshComponent*> Cells;
	TArray<TArray<TCHAR>> Data;
	TArray<TArray<uint64>> Paths;

	UStaticMesh* StartMesh;
	UStaticMesh* BeamMesh;
	UStaticMesh* SplitterMesh;

	FTimerHandle MyTimerHandle;
	int CurrentStep = 0;

	virtual FString DayName() const override { return "Day07"; }
	virtual int64 SolvePart1() override;
	virtual int64 SolvePart2() override;
	void ActivateBeam(int x, int y, uint64 timelines);
	void Step();
};
