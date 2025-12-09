#include "Day09.h"
#include "Util.h"
#include "Linq.h"

ADay09::ADay09()
{
}

int64 ADay09::SolvePart1()
{
	int64 bestArea = 0;
	for (int i = 0; i < CornerPoints.Num(); i++)
	{
		FIntVector2 PointA = CornerPoints[i];
		for (int j = i + 1; j < CornerPoints.Num(); j++)
		{
			FIntVector2 PointB = CornerPoints[j];
			int64 dx = FMath::Abs(PointB.X - PointA.X) + 1LL;
			int64 dy = FMath::Abs(PointB.Y - PointA.Y) + 1LL;
			int64 area = dx * dy;
			if (area > bestArea)
			{
				bestArea = area;
			}
		}
	}
	return bestArea;
}

int64 ADay09::SolvePart2()
{
	return 0;
}

void ADay09::BeginPlay()
{
	Super::BeginPlay();
	auto Lines = Util::FileAsLines(GetInputFilePath());
	CornerPoints = UnrealLinq::Select(Lines, [](const FString& Line) {
		FString Left, Right;
		Line.Split(TEXT(","), &Left, &Right);
		return FIntVector2(FCString::Atoi(*Left), FCString::Atoi(*Right));
	});

	Part1 = SolvePart1();
}
