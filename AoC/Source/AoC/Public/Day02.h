#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Day02.generated.h"

UCLASS()
class AOC_API ADay02 : public AActor
{
	GENERATED_BODY()
	
public:	
	ADay02();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int64 Part1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int64 Part2;

protected:
	virtual void BeginPlay() override;

private:
	TArray<std::tuple<int64, int64>> Ranges; 
	
	int64 SolvePart1();
	int64 SolvePart2();
};
