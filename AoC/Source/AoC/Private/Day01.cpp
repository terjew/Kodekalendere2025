#include "Day01.h"
#include "Util.h"
#include "Linq.h"

ADay01::ADay01()
{
}

int64 ADay01::SolvePart1()
{
	int count = 0;
	int cur = 50;
	for (int rotation : Rotations) {
		cur += rotation;
		cur %= 100;
		if (cur == 0) {
			count++;
		}
	}
	return count;
}

int64 ADay01::SolvePart2()
{
	int count = 0;
	int cur = 50;
	for (int rotation : Rotations)
	{
		if (abs(rotation) >= 100) {
			int whole = rotation / 100;
			rotation -= whole * 100;
			count += abs(whole);
		}
		bool was0 = cur == 0;
		cur += rotation;
		if (cur < 0) {
			cur += 100;
			if (!was0) count++;
		}
		else if (cur >= 100) {
			cur -= 100;
			count++;
		}
		else if (cur == 0) {
			count++;
		}
	}
	return count;
}

void ADay01::BeginPlay()
{
	Super::BeginPlay();
	TArray<FString> Lines = Util::FileAsLines(TEXT("Day01/input.txt"));

	Rotations = UnrealLinq::Select(Lines, [](const FString& line) {
		auto firstChar = line.Left(1);
		int sign = (firstChar == TEXT("L")) ? -1 : 1;
		auto rest = line.RightChop(1);
		return sign * FCString::Atoi(*rest);
	});

	Part1 = SolvePart1();
	Part2 = SolvePart2();
}


