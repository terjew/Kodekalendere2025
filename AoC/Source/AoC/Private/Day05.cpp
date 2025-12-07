#include "Day05.h"
#include "Util.h"
#include "Linq.h"

ADay05::ADay05()
{
}

bool IsFresh(int64 Ingredient, TArray<std::pair<int64, int64>> Ranges)
{
	for (auto& [Start, End] : Ranges)
	{
		if (Ingredient >= Start && Ingredient <= End) return true;
	}
	return false;
}

int64 ADay05::SolvePart1()
{
	int fresh = 0;
	for (auto& Ingredient : Ingredients)
	{
		if (IsFresh(Ingredient, Ranges)) fresh++;
	}
	return fresh;
}

int64 ADay05::SolvePart2()
{
	int64 sum = 0;
	std::pair<int64,int64> previous = std::make_pair<int64, int64>(-1, -1);
	Ranges.Sort();
	for (auto& Range : Ranges)
	{
		int64 current = Range.second - Range.first + 1;
		if (previous.second <= Range.second) { //otherwise, current is fully contained in previous
			sum += current;

			if (previous.second > 0 && previous.second >= Range.first) { //some overlap
				auto overlap = previous.second - Range.first + 1;
				sum -= overlap;
			}
		}
		previous = Range;
	}
	return sum;
}

void ADay05::BeginPlay()
{
	Super::BeginPlay();
	auto Lines = Util::FileAsLines(GetInputFilePath(), false);
	int32 SeparatorIndex;

	Lines.Find(TEXT(""), SeparatorIndex);

	Ranges = UnrealLinq::Select(
		UnrealLinq::Take(Lines, (size_t)SeparatorIndex), 
		Util::StringToRange
	);

	Ingredients = UnrealLinq::Select(
		UnrealLinq::Skip(Lines, SeparatorIndex + 1),
		Util::StringToInt64
	);

	Part1 = SolvePart1();
	Part2 = SolvePart2();
}
