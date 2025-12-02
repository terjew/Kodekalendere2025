#include "Day02.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFileManager.h"

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

int64 SolvePart1(TArray<std::tuple<int64, int64>> Ranges)
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

int64 SolvePart2(TArray<std::tuple<int64, int64>> Ranges)
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

// Called when the game starts or when spawned
void ADay02::BeginPlay()
{
	Super::BeginPlay();
	FString FilePath = FPaths::ProjectContentDir() + TEXT("Day02/input.txt");
	FString FileContent;
	if (!FFileHelper::LoadFileToString(FileContent, *FilePath))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load file: %s"), *FilePath);
	}

	TArray<FString> Segments;
	FString Delimiter = TEXT(",");
	FileContent.ParseIntoArray(Segments, *Delimiter, true);

	TArray<std::tuple<int64, int64>> Ranges;
	Ranges.SetNum(Segments.Num());
	std::transform(Segments.GetData(), Segments.GetData() + Segments.Num(), Ranges.GetData(),
		[](FString& segment)
		{
			FString Left, Right;
			segment.Split(TEXT("-"), &Left, &Right);
			return std::make_tuple(FCString::Atoi64(*Left), FCString::Atoi64(*Right));
		});

	Part1 = SolvePart1(Ranges);
	Part2 = SolvePart2(Ranges);
}

void ADay02::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

