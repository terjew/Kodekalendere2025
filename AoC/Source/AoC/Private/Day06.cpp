#include "Day06.h"
#include "Util.h"
#include "Linq.h"

ADay06::ADay06()
{
}

int64 ADay06::SolvePart1()
{
	int64 total = 0;
	for (int i = 0; i < Operators.Num(); ++i) {
		const FString& Operator = Operators[i];
		int64 sum = Operator == "+" ? 0 : 1;


		for (auto OperandLine : Operands) {
			auto Operand = OperandLine[i];

			if (Operator == "+") {
				sum += Operand;
			}
			else if (Operator == "*") {
				sum *= Operand;
			}
		}
		total += sum;
	}
	return total;
}

int64 Calculate(TArray<FString>& Buffer)
{
	auto lastIndex = Buffer.Num() - 1;
	FString Operator = Buffer[lastIndex].Right(1);
	Buffer[lastIndex] = Buffer[lastIndex].LeftChop(1);
	auto Numbers = UnrealLinq::Select(Buffer, Util::StringToInt64);
	if (Operator == TEXT("+")) {
		return UnrealLinq::Aggregate(Numbers, (int64)0, Util::Add);
	}
	else if (Operator == TEXT("*")) {
		return UnrealLinq::Aggregate(Numbers, (int64)1, Util::Mul);
	}
	return 0;
}

int64 ADay06::SolvePart2()
{
	auto Lines = Util::FileAsLines(GetInputFilePath());
	auto NumColumns = Lines[0].Len();
	
	TArray<FString> Transposed;
	Transposed.SetNum(0);
	for (int x = NumColumns - 1; x >= 0; x--)
	{
		TArray<TCHAR> Chars = UnrealLinq::Select(Lines, [x](const FString& Line) {
			return Line[x];
		});
		Chars.Add(TEXT('\0'));
		Transposed.Add(FString(Chars.GetData()));
	}

	int64 total = 0;
	TArray<FString> Buffer;
	for (int i = 0; i < Transposed.Num(); ++i) {
		if (Transposed[i].TrimEnd() == TEXT("")) {
			total += Calculate(Buffer);
			Buffer.SetNum(0);
			continue;
		}
		Buffer.Add(Transposed[i]);
	}
	if (Buffer.Num() != 0) total += Calculate(Buffer);

	return total;
}

void ADay06::BeginPlay()
{
	Super::BeginPlay();
	auto Lines = Util::FileAsLines(GetInputFilePath());
	
	auto Segmented = UnrealLinq::Select(Lines, [](const FString& Line) {
		TArray<FString> Segments;
		Line.ParseIntoArray(Segments, TEXT(" "), true);
		return Segments;
	});

	Operators = Segmented.Last();
	Operands = UnrealLinq::Select(
		UnrealLinq::Take(Segmented, Segmented.Num() - 1), 
		[](const TArray<FString>& Segments) {
			return UnrealLinq::Select(Segments, Util::StringToInt64);
		}
	);

	Part1 = SolvePart1();
	Part2 = SolvePart2();
}
