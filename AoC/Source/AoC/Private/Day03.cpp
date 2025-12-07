#include "Day03.h"
#include "Util.h"

ADay03::ADay03()
{
	PrimaryActorTick.bCanEverTick = true;
}

int64 ADay03::SolvePart1()
{
	int64 sum = 0;
	for (auto & bank : Ranges) {
		auto firstDigitIndex = Util::maxIndex(bank, 0, bank.Num() - 1);
		auto secondDigitIndex = Util::maxIndex(bank, firstDigitIndex + 1, bank.Num());
		sum += bank[firstDigitIndex] * 10 + bank[secondDigitIndex];
	}
	return sum;
}

int64 ADay03::SolvePart2()
{
	int64 sum = 0;
	for (auto& bank : Ranges) {
		size_t currentIndex = -1;
		int64 banksum = 0;
		for (int i = 11; i >= 0; i--) {
			currentIndex = Util::maxIndex(bank, currentIndex + 1, bank.Num() - i);
			banksum = banksum * 10 + bank[currentIndex];
		}
		sum += banksum;
	}
	return sum;
}

void ADay03::BeginPlay()
{
	Super::BeginPlay();
	auto Lines = Util::FileAsLines(TEXT("Day03/input.txt"));

	Util::selectInto(Lines, Ranges, [](const FString& line) {
		FString trimmed = line.TrimEnd();
		TArray<int> Bank;
		Util::selectInto(*trimmed, trimmed.Len(), Bank, [](TCHAR c) {
			return c - _T('0');
		});
		return Bank;
	});

	Part1 = SolvePart1();
	Part2 = SolvePart2();
}

void ADay03::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

