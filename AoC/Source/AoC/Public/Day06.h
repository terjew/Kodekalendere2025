#pragma once

#include "DayBase.h"
#include "Day06.generated.h"

UCLASS()
class AOC_API ADay06 : public ADayBase
{
	GENERATED_BODY()
	
public:	
	ADay06();

protected:
	virtual void BeginPlay() override;

private:
	TArray<FString> Operators;
	TArray<TArray<int64>> Operands;
	virtual FString DayName() const override { return "Day06"; }
	virtual int64 SolvePart1() override;
	virtual int64 SolvePart2() override;
};
