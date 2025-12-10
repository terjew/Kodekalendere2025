#include "Day10.h"
#include "Util.h"
#include "Linq.h"
#include "Internationalization/Regex.h"

ADay10::ADay10()
{

}

int64 SolveMachine(const ElvenMachine& Machine)
{
	//solve as BFS:
	TQueue<MachineState> queue;

	MachineState initialState;
	initialState.CurrentState.SetNumZeroed(Machine.DesiredState.Num());
	initialState.ButtonsPressed.Empty();
	queue.Enqueue(initialState);

	while (!queue.IsEmpty())
	{
		MachineState current;
		queue.Dequeue(current);

		for (int buttonNo = 0; buttonNo < Machine.Buttons.Num(); buttonNo++)
		{
			MachineState newState = current.PressButton(Machine, buttonNo);
			if (newState.CurrentState == Machine.DesiredState)
			{
				return newState.ButtonsPressed.Num();
			}
			queue.Enqueue(newState);
		}
	}
	return -1;
}

int64 ADay10::SolvePart1()
{
	int64 total = 0;
	for (const auto& Machine : Machines)
	{
		total += SolveMachine(Machine);
	}
	return total;
}

int64 ADay10::SolvePart2()
{
	return 0;
}

void ADay10::BeginPlay()
{
	Super::BeginPlay();

	auto Lines = Util::FileAsLines(GetInputFilePath());
	for(auto & Line : Lines)
	{
		FRegexMatcher Matcher(FRegexPattern(TEXT("^\\[(?<desiredstate>(?:#|\\.)+)\\] (?<buttons>(?:\\((?:\\d|,)+\\) )+)\\{(?<jolts>(?:(?:\\d+)|,)+)\\}$")), Line);
		if (Matcher.FindNext())
		{
			ElvenMachine machine;

			FString DesiredState = Matcher.GetCaptureGroup(1);
			machine.DesiredState = UnrealLinq::Select(Util::StringToCharArray(DesiredState), [](const TCHAR& c)
				{return c == _T('#'); });

			FString Buttons = Matcher.GetCaptureGroup(2);
			TArray<FString> ButtonStrings = Util::StringSplitBy(Buttons, TEXT(" "));
			machine.Buttons = UnrealLinq::Select(ButtonStrings, [](const FString& ButtonStr) {
				FString Trimmed = ButtonStr.Mid(1, ButtonStr.Len() - 2);
				TArray<FString> IndicesStr = Util::StringSplitBy(Trimmed, TEXT(","));
				return UnrealLinq::Select(IndicesStr, [](const FString& IdxStr) {
					return FCString::Atoi(*IdxStr);
				});
			});

			FString Jolts = Matcher.GetCaptureGroup(3);
			machine.Jolts = UnrealLinq::Select(Util::StringSplitBy(Jolts, TEXT(",")), [](const FString& JoltStr) {
				return FCString::Atoi(*JoltStr);
			});

			Machines.Add(machine);
		}
	}		

	Part1 = SolvePart1();
	Part2 = SolvePart2();
}
