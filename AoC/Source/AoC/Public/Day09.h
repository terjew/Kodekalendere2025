#pragma once

#include "DayBase.h"
#include "Day09.generated.h"

UCLASS()
class AOC_API ADay09 : public ADayBase
{
	GENERATED_BODY()
	
public:	
	ADay09();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int64 Width;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int64 Height;

protected:
	virtual void BeginPlay() override;

private:
	TArray<TArray<TCHAR>> Data;
	TArray<FIntVector2> CornerPoints;

	virtual FString DayName() const override { return "Day09"; }
	virtual int64 SolvePart1() override;
	virtual int64 SolvePart2() override;
};
