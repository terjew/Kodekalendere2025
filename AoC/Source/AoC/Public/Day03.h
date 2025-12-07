#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Day03.generated.h"

UCLASS()
class AOC_API ADay03 : public AActor
{
	GENERATED_BODY()
	
public:	
	ADay03();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int64 Part1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int64 Part2;

protected:
	virtual void BeginPlay() override;

private:	
	TArray<TArray<int>> Ranges;
	int64 SolvePart1();
	int64 SolvePart2();
};
