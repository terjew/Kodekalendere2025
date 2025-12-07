#pragma once

#include "DayBase.h"
#include "Math/Int128.h"
#include <Math/BigInt.h>
#include "Day07.generated.h"

UCLASS()
class AOC_API ADay07 : public ADayBase
{
	GENERATED_BODY()
	
	typedef TBigInt<128, false> MyLargeInt;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString HugeNumber;

protected:
	virtual void BeginPlay() override;

private:
	TArray<UStaticMeshComponent*> Cells;
	TArray<TArray<TCHAR>> Data;
	TArray<TArray<MyLargeInt>> Paths;

	UStaticMesh* StartMesh;
	UStaticMesh* BeamMesh;
	UStaticMesh* SplitterMesh;

	FTimerHandle MyTimerHandle;
	int CurrentStep = 0;

	virtual FString DayName() const override { return "Day07"; }
	virtual int64 SolvePart1() override;
	virtual int64 SolvePart2() override;
	void ActivateBeam(int x, int y, MyLargeInt timelines);
	void Step();
};
