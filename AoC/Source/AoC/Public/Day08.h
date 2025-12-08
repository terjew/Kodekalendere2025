#pragma once

#include "DayBase.h"
#include "Day08.generated.h"


struct FVertex
{
	FVertex(int n, FVector v) : VertexNumber(n), Position(v) {}
	FVertex() : VertexNumber(0), Position(FVector::Zero()) {}
	int VertexNumber;
	FVector Position;
	TArray<int32> ConnectedVertices;
};

UCLASS()
class AOC_API ADay08 : public ADayBase
{
	GENERATED_BODY()
	
public:	
	ADay08();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* ConnectedMaterial;

protected:
	virtual void BeginPlay() override;

private:
	TArray<UStaticMeshComponent*> Cells;
	UStaticMesh* CubeMesh;
	TArray<FVertex> Vertices;
	TArray<TSet<int32>> FindComponents() const;
	virtual FString DayName() const override { return "Day08"; }
	virtual int64 SolvePart1() override;
	virtual int64 SolvePart2() override;
};
