#include "Day05.h"
#include "Util.h"
#include "Linq.h"

ADay05::ADay05()
{
}

int64 ADay05::SolvePart1()
{
	return 0;
}

int64 ADay05::SolvePart2()
{
	return 0;
}

void ADay05::BeginPlay()
{
	Super::BeginPlay();
	auto Lines = Util::FileAsLines(GetInputFilePath(), false);
	int32 SeparatorIndex;
	Lines.Find(TEXT(""), SeparatorIndex);
}
