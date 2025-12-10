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

int64 SolveJolts(const ElvenMachine& Machine)
{
	//Idea: 
	// Let's say we have a machine with the following buttons:
	// Button 1: (3)
	// Button 2: (1,3)
	// Button 3: (2)
	// Button 4: (2,3)
	// Button 5: (0,2)
	// Button 6: (0,1)
	// Desired jolts: 3,5,4,7
	// 
	
	//Each of the buttons can be seens as a binary number, where the bits that are set correspond to the indices of the jolts that button adds.
	//For the buttons above, we have:
	// b1: (3)     ->  0001000  -> 8
	// b2: (1,3)   ->  0001010  -> 10
	// b3: (2)     ->  0000100  -> 4
	// b4: (2,3)   ->  0001100  -> 12
	// b5: (0,2)   ->  0000101  -> 5
	// b6: (0,1)   ->  0000011  -> 3

	//Looking at each bit in the desired state, we can derive constraints on the number of times buttons that set that bit are pressed. Each bit in the target can only be set by the buttons that have that bit set. Thus, the sum of presses for the buttons that have that bit set must equal the desired value for that bit.
	//For the above example, we have:
	//bit 0: Can be set by b5 or b6. That gives: x5 + x6 = 3
	//bit 1: Can be set by b2 or b6. That gives: x2 + x6 = 5
	//bit 2: Can be set by b3 or b4 or b5. That gives: x3 + x4 + x5 = 4
	//bit 3: Can be set by b1 or b2 or b4. That gives: x1 + x2 + x4 = 7
	//Generalizing this we have:
	//for each bit i in 0 to N-1:
	//   sum(for each button j that sets bit i: xj) = Jolts[i]

	//That gives us a system of equations that can be solved using standard techniques.

	//The desired jolts for the machine can be interpreted as a sum of multiples of powers of two. The first number is the least significant bit.
	//Eg, for the desired state 3,5,4,7  we need to be able to produce:
	//2 * 2^0 + 5 * 2^1 + 4 * 2^2 + 7 * 2^3 = 3 + 10 + 16 + 56 = 85 jolts
	//In a more general form we have:
	//TargetJolts = sum(for i in 0 to N-1: Jolts[i] * 2^i)

	//We can then construct a diophantine equation, where the unknowns are the number of times each button is pressed and the constants are the decimal values of the buttons. The target value is the desired jolt value.
	//For the above example, we have:
	//8 x1 + 10 x2 + 4 x3 + 12 x4 + 5 x5 + 3 x6 = 85
	//Generalized this can be written as:
	//buttonDecimalValue[0] * x0 + buttonDecimalValue[1] * x1 + ... + buttonDecimalValue[M-1] * xM-1 = TargetJolts
	//
	//That will be the last equation in our system.
	// 
	//Additionally, we have the constraint that all xi >= 0 and integer.
	//We also know that there are multiple solutions, and we want the one that minimizes the sum of all xi.

	//Let's write a function that constructs and solves this system of equations:

	TArray<int> ButtonDecimalValues = UnrealLinq::Select(Machine.Buttons, [&](const TArray<int>& Button) {
		int value = 0;
		for (int idx : Button)
		{
			value |= (1 << idx);
		}
		return value;
	});

	//Make a sorted list of buttons by decimal value, descending
	TArray<std::pair<int, int>> ButtonsByDecimalValue; //pair<decimal value, button index>
	for (int buttonNo = 0; buttonNo < Machine.Buttons.Num(); buttonNo++)
	{
		ButtonsByDecimalValue.Add(std::make_pair(ButtonDecimalValues[buttonNo], buttonNo));
	}
	ButtonsByDecimalValue.Sort([](const std::pair<int, int>& a, const std::pair<int, int>& b) {
		return a.first > b.first;

	});

	int NumBits = Machine.DesiredState.Num();
	int64 TargetJolts = 0;
	for (int bitIdx = 0; bitIdx < NumBits; bitIdx++)
	{
		TargetJolts += (Machine.Jolts[bitIdx] << bitIdx);
	}

	TArray<TArray<int>> BitButtonMap;
	for (int bitIdx = 0; bitIdx < NumBits; bitIdx++)
	{
		TArray<int> ButtonsSettingBit;
		for (int buttonNo = 0; buttonNo < Machine.Buttons.Num(); buttonNo++)
		{
			if (Machine.Buttons[buttonNo].Contains(bitIdx))
			{
				ButtonsSettingBit.Add(buttonNo);
			}
		}
		BitButtonMap.Add(ButtonsSettingBit);
	}

	//Now we have ButtonDecimalValues and BitButtonMap, we can construct the system of equations.
	//Let's represent the system as a matrix, where each row is a linear equation on the form ax1 + bx2 + ... + kxM = C. Each row will have M+1 columns, where the first M columns are the coefficients of the variables and the last column is the constant C.
	TArray<TArray<int64>> CoefficientMatrix;

	//First we add the bit constraints
	for (int bitIdx = 0; bitIdx < NumBits; bitIdx++)
	{
		//add a row for this bit
		TArray<int64> Row;
		Row.SetNumZeroed(Machine.Buttons.Num() + 1);
		for (int buttonNo : BitButtonMap[bitIdx])
		{
			Row[buttonNo] = 1;
		}
		Row.Last() = Machine.Jolts[bitIdx];
	}

	//Then we add the jolts equation
	{
		TArray<int64> Row;
		Row.SetNumZeroed(Machine.Buttons.Num() + 1);
		for (int buttonNo = 0; buttonNo < Machine.Buttons.Num(); buttonNo++)
		{
			Row[buttonNo] = ButtonDecimalValues[buttonNo];
		}
		Row.Last() = TargetJolts;
	}

	//We will try a greedy approach to find a solution that minimizes the total number of button presses.
	//Starting from the button with the highest decimal value, we will press it as many times as possible without exceeding the target jolts or violating the bit constraints.
	//Then we will move to the next button and repeat until we reach the target jolts or find that we cannot reach the target.
	//Then we will backtrack and reduce the number of presses for the last button and try again.

}

int64 ADay10::SolvePart2()
{
	int64 total = 0;
	for (const auto& Machine : Machines)
	{
		total += SolveJolts(Machine);
	}
	return total;
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
