#pragma once

#include "DayBase.h"
#include "Template.generated.h"

UCLASS()
class AOC_API ATemplate : public ADayBase
{
	GENERATED_BODY()
	
public:	
	ATemplate();

protected:
	virtual void BeginPlay() override;

private:
	virtual FString DayName() const override { return "DayTemplate"; }
	virtual int64 SolvePart1() override;
	virtual int64 SolvePart2() override;
};
