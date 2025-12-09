#pragma once

#include "DayBase.h"
#include "Day09.generated.h"

struct IntLineSegment
{
	FIntPoint Start;
	FIntPoint End;
	bool bIsVertical;
	IntLineSegment() : Start(0),End(0),bIsVertical(false) {}
	IntLineSegment(const FIntPoint& InStart, const FIntPoint& InEnd)
	{
		Start = FIntPoint(FMath::Min(InStart.X, InEnd.X), FMath::Min(InStart.Y, InEnd.Y));
		End = FIntPoint(FMath::Max(InStart.X, InEnd.X), FMath::Max(InStart.Y, InEnd.Y));
		bIsVertical = (Start.X == End.X);
	}

	bool Intersects(const IntLineSegment& Other) const
	{
		if (Other.bIsVertical == bIsVertical)
		{
			return false;
		}
		const IntLineSegment& VerticalSeg = bIsVertical ? *this : Other;
		const IntLineSegment& HorizontalSeg = bIsVertical ? Other : *this;
		if (VerticalSeg.Start.X <= HorizontalSeg.Start.X || VerticalSeg.Start.X >= HorizontalSeg.End.X)
		{
			return false;
		}
		if (HorizontalSeg.Start.Y <= VerticalSeg.Start.Y || HorizontalSeg.Start.Y >= VerticalSeg.End.Y)
		{
			return false;
		}
		return true;
	}
};

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
	TArray<FIntPoint> CornerPoints;

	virtual FString DayName() const override { return "Day09"; }
	virtual int64 SolvePart1() override;
	virtual int64 SolvePart2() override;
};
