#pragma once

#include "DayBase.h"
#include "Day11.generated.h"

struct Device
{
	Device(const FString& InName) : Name(InName) {}
	FString Name;
	TArray<TSharedRef<Device>> Connections;
	int64 TotalPaths = -1;
};

UCLASS()
class AOC_API ADay11 : public ADayBase
{
	GENERATED_BODY()
	
public:	
	ADay11();

protected:
	virtual void BeginPlay() override;

private:
	TMap<FString, TSharedRef<Device>> Devices;

	virtual FString DayName() const override { return "Day11"; }
	virtual int64 SolvePart1() override;
	virtual int64 SolvePart2() override;
};
