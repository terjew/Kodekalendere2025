#include "Day09.h"
#include "Util.h"
#include "Linq.h"
#include "GeomTools.h"
#include "Algo/Reverse.h"

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
	TArray<FVector2D> Points = UnrealLinq::Select(CornerPoints, [](const FIntPoint& P) {
		return FVector2D(static_cast<double>(P.X), static_cast<double>(P.Y));
	});

	TArray<IntLineSegment> Edges;
	for (int i = 0; i < CornerPoints.Num(); i++)
	{
		FIntPoint Start = CornerPoints[i];
		FIntPoint End = CornerPoints[(i + 1) % CornerPoints.Num()];
		Edges.Add(IntLineSegment(Start, End));
	}

	int64 bestArea = 0;
	for (int i = 0; i < CornerPoints.Num(); i++)
	{
		FIntVector2 PointA = CornerPoints[i];
		for (int j = i + 1; j < CornerPoints.Num(); j++)
		{
			FIntVector2 PointB = CornerPoints[j];
			int x0 = FMath::Min(PointA.X, PointB.X);
			int x1 = FMath::Max(PointA.X, PointB.X);
			int y0 = FMath::Min(PointA.Y, PointB.Y);
			int y1 = FMath::Max(PointA.Y, PointB.Y);
			
			int64 dx = x1 - x0 + 1LL;
			int64 dy = y1 - y0 + 1LL;
			int64 area = dx * dy;
			if (area > bestArea)
			{
				FIntPoint ul = FIntPoint(x0, y0);
				FIntPoint ur = FIntPoint(x1, y0);
				FIntPoint ll = FIntPoint(x0, y1);
				FIntPoint lr = FIntPoint(x1, y1);

				TArray<FIntPoint> RectPoints = { ul, ur, lr, ll };
				bool allInside = true;
				for (const auto& RectPoint : RectPoints)
				{
					if (!FGeomTools2D::IsPointInPolygon(FVector2D(static_cast<double>(RectPoint.X), static_cast<double>(RectPoint.Y)), Points)) {
						allInside = false;
						break;
					}
				}
				if (!allInside)
				{
					continue;
				}

				TArray<IntLineSegment> RectEdges = {
					IntLineSegment(ul, ur),
					IntLineSegment(ur, lr),
					IntLineSegment(lr, ll),
					IntLineSegment(ll, ul)
				};
				bool bIntersects = false;
				for (const auto& RectEdge : RectEdges)
				{
					for (const auto& Edge : Edges)
					{
						if (RectEdge.Intersects(Edge))
						{
							bIntersects = true;
							break;
						}
					}
					if (bIntersects)
					{
						break;
					}
				}
				if (bIntersects)
				{
					continue;
				}
				bestArea = area;
			}
		}
	}
	return bestArea;
}

void ADay09::BeginPlay()
{
	Super::BeginPlay();
	auto Lines = Util::FileAsLines(GetInputFilePath());
	CornerPoints = UnrealLinq::Select(Lines, [](const FString& Line) {
		FString Left, Right;
		Line.Split(TEXT(","), &Left, &Right);
		return FIntPoint(FCString::Atoi(*Left), FCString::Atoi(*Right));
	});

	Part1 = SolvePart1();
	Part2 = SolvePart2();
}
