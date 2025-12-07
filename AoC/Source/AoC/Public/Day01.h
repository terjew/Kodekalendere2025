#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Day01.generated.h"

UCLASS()
class AOC_API ADay01 : public AActor
{
	GENERATED_BODY()
	
public:	
	ADay01();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> Rotations;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Part1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Part2;

protected:
	virtual void BeginPlay() override;

private:
	int64 SolvePart1();
	int64 SolvePart2();
};
