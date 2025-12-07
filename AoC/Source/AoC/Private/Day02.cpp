#include "Day02.h"
#include "Util.h"
#include "Linq.h"

ADay02::ADay02()
{
}

bool IsInvalid(int64 Number)
{
	FString numStr = FString::Printf(TEXT("%lld"), Number);
	int len = numStr.Len();
	if (len % 2 != 0) return false;
	return (numStr.Left(len / 2) == numStr.Right(len / 2));
}

bool IsInvalidWithCount(FString NumStr, int Count)
{
	int len = NumStr.Len();
	if (len % Count != 0) return false;

	int segmentLen = len / Count;
	FString first = NumStr.Left(segmentLen);
	for (int i = 1; i < Count; i++) {
		if (NumStr.Mid(i * segmentLen, segmentLen) != first) {
			return false;
		}
	}
	return true;
}

bool IsInvalidNewRule(int64 Number)
{
	FString numStr = FString::Printf(TEXT("%lld"), Number);
	int len = numStr.Len();
	for (int segmentCount = 2; segmentCount <= len; segmentCount ++) {
		if (IsInvalidWithCount(numStr, segmentCount)) {
			return true;
		}
	}
	return false;
}

int64 ADay02::SolvePart1()
{
	int64 sum = 0;
	for (auto& [start, end] : Ranges) {
		for (int64 i = start; i <= end; i++) {
			if (IsInvalid(i)) {
				sum += i;
			}
		}
	}
	return sum;
}

int64 ADay02::SolvePart2()
{
	int64 sum = 0;
	for (auto& [start, end] : Ranges) {
		for (int64 i = start; i <= end; i++) {
			if (IsInvalidNewRule(i)) {
				sum += i;
			}
		}
	}
	return sum;
}

void ADay02::BeginPlay()
{
	Super::BeginPlay();

	auto Input = Util::FileAsString(TEXT("Day02/input.txt"));

	TArray<FString> Segments;
	Input.ParseIntoArray(Segments, TEXT(","), true);

	Ranges = UnrealLinq::Select(Segments, Util::StringToRange);

	Part1 = SolvePart1();
	Part2 = SolvePart2();
}


