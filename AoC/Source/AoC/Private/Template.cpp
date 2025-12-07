#include "Template.h"
#include "Util.h"
#include "Linq.h"

ATemplate::ATemplate()
{
}

int64 ATemplate::SolvePart1()
{
	return 0;
}

int64 ATemplate::SolvePart2()
{
	return 0;
}

void ATemplate::BeginPlay()
{
	Super::BeginPlay();
	auto Lines = Util::FileAsLines(GetInputFilePath());
}
