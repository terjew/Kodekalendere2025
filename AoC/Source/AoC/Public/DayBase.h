#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DayBase.generated.h"

UCLASS(Abstract)
class AOC_API ADayBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ADayBase() { }

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Day;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int64 Part1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int64 Part2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool UseSample = false;

protected:
	const FString InputFilename = TEXT("input.txt");
	const FString SampleFilename = TEXT("sample.txt");
	FString GetInputFilePath() const
	{
		const FString DayFolder = DayName();
		const FString Filename = UseSample ? SampleFilename : InputFilename;
		return DayFolder / Filename;
	}

	virtual void BeginPlay() override {
		Super::BeginPlay();
		Day = DayName();
	}

private:
	virtual int64 SolvePart1() PURE_VIRTUAL(ADayBase::SolvePart1, return 0;);
	virtual int64 SolvePart2() PURE_VIRTUAL(ADayBase::SolvePart2, return 0;);
	virtual FString DayName() const PURE_VIRTUAL(ADayBase::DayName, return "DayXX";);
};
