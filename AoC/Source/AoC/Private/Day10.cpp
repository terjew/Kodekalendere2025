#include "Day10.h"
#include "Util.h"
#include "Linq.h"
#include "Internationalization/Regex.h"
#include <Eigen/Dense>

ADay10::ADay10()
{

}

int64 SolveMachine(const ElvenMachine& Machine)
{
	//solve as BFS:
	TQueue<MachineState> queue;

	MachineState initialState;
	initialState.CurrentState.SetNumZeroed(Machine.DesiredState.Num());
	initialState.ButtonsPressed.SetNumZeroed(Machine.Buttons.Num());
	initialState.JoltsGenerated.SetNumZeroed(Machine.DesiredJolts.Num());
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
				return UnrealLinq::Sum(newState.ButtonsPressed);
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

static FString toString(const Eigen::MatrixXd& mat) {
	std::stringstream ss;
	ss << mat;
	return FString(ANSI_TO_TCHAR(ss.str().c_str()));
}


static FString toString(const Eigen::VectorXd& vec) {
	std::stringstream ss;
	ss << vec;
	return FString(ANSI_TO_TCHAR(ss.str().c_str()));
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

	//Looking at each bit in the desired state, we can derive constraints on the number of times buttons that set that bit are pressed. Each bit in the target can only be set by the buttons that have that bit set. Thus, the sum of presses for the buttons that have that bit set must equal the desired value for that bit.
	//For the above example, we have:
	//bit 0: Can be set by b5 or b6.		That gives: x5 + x6 = 3
	//bit 1: Can be set by b2 or b6.		That gives: x2 + x6 = 5
	//bit 2: Can be set by b3 or b4 or b5.	That gives: x3 + x4 + x5 = 4
	//bit 3: Can be set by b1 or b2 or b4.	That gives: x1 + x2 + x4 = 7
	//Generalizing this we have:
	//for each bit i in 0 to N-1:
	//   sum(for each button j that sets bit i: xj) = Jolts[i]

	//That gives us a system of equations that can be solved using standard techniques.
	//Let's write a function that constructs and solves this system of equations:

	int NumButtons = Machine.Buttons.Num();
	int NumBits = Machine.DesiredState.Num();

	// A should be NumBits x NumButtons (each row = one bit equation)
	Eigen::MatrixXd A(NumBits, NumButtons);
	Eigen::VectorXd b(NumBits);

	// Fill A and b
	for (int bitIdx = 0; bitIdx < NumBits; bitIdx++)
	{
		for (int buttonNo = 0; buttonNo < NumButtons; buttonNo++)
		{
			// note the swapped indices: row = bitIdx, col = buttonNo
			if (Machine.Buttons[buttonNo].Contains(bitIdx))
				A(bitIdx, buttonNo) = 1;
			else
				A(bitIdx, buttonNo) = 0;
		}
		b(bitIdx) = Machine.DesiredJolts[bitIdx];
	}

	UE_LOG(LogTemp, Warning, TEXT("Eigen coefficients: \n%s"), *toString(A));
	UE_LOG(LogTemp, Warning, TEXT("Eigen constants: \n%s"), *toString(b));

	// Solve for x (NumButtons x 1)
	Eigen::VectorXd x = A.fullPivLu().solve(b);

	UE_LOG(LogTemp, Warning, TEXT("Eigen solved: \n%s"), *toString(x));

	
	return -1;
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
			//ElvenMachine machine;

			FString DesiredStateString = Matcher.GetCaptureGroup(1);
			auto DesiredState = UnrealLinq::Select(Util::StringToCharArray(DesiredStateString), [](const TCHAR& c)
				{return c == _T('#'); });

			FString ButtonString = Matcher.GetCaptureGroup(2);
			TArray<FString> ButtonStrings = Util::StringSplitBy(ButtonString, TEXT(" "));
			auto Buttons = UnrealLinq::Select(ButtonStrings, [](const FString& ButtonStr) {
				FString Trimmed = ButtonStr.Mid(1, ButtonStr.Len() - 2);
				TArray<FString> IndicesStr = Util::StringSplitBy(Trimmed, TEXT(","));
				return UnrealLinq::Select(IndicesStr, [](const FString& IdxStr) {
					return FCString::Atoi(*IdxStr);
				});
			});

			FString JoltString = Matcher.GetCaptureGroup(3);
			auto Jolts = UnrealLinq::Select(Util::StringSplitBy(JoltString, TEXT(",")), [](const FString& JoltStr) {
				return FCString::Atoi(*JoltStr);
			});

			ElvenMachine machine(DesiredState, Buttons, Jolts);
			Machines.Add(machine);
		}
	}		

	//Part1 = SolvePart1();
	Part2 = SolvePart2();
}
