#pragma once

#include "DayBase.h"
#include "Day05.generated.h"

UCLASS()
class AOC_API ADay05 : public ADayBase
{
	GENERATED_BODY()
	
public:	
	ADay05();

protected:
	virtual void BeginPlay() override;

private:
	TArray<std::tuple<int64, int64>> Ranges;
	TArray<int64> Ingredients;

	virtual FString DayName() const override { return "Day05"; }
	int64 SolvePart1() override;
	int64 SolvePart2() override;
};
