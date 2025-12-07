#include "Day02.h"
#include "Util.h"

ADay02::ADay02()
{
	PrimaryActorTick.bCanEverTick = true;
}

bool IsInvalid(int64 number)
{
	FString numStr = FString::Printf(TEXT("%lld"), number);
	int len = numStr.Len();
	if (len % 2 != 0) return false;
	return (numStr.Left(len / 2) == numStr.Right(len / 2));
}

bool IsInvalidWithCount(FString numStr, int count)
{
	int len = numStr.Len();
	if (len % count != 0) return false;

	int segmentLen = len / count;
	FString first = numStr.Left(segmentLen);
	for (int i = 1; i < count; i++) {
		if (numStr.Mid(i * segmentLen, segmentLen) != first) {
			return false;
		}
	}
	return true;
}

bool IsInvalidNewRule(int64 number)
{
	FString numStr = FString::Printf(TEXT("%lld"), number);
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

	Util::selectInto(Segments, Ranges, [](const FString& segment) {
		FString Left, Right;
		segment.Split(TEXT("-"), &Left, &Right);
		return std::make_tuple(FCString::Atoi64(*Left), FCString::Atoi64(*Right));
	});

	Part1 = SolvePart1();
	Part2 = SolvePart2();
}

void ADay02::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

