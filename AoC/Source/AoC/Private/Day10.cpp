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

// Helper function to check if a double is close to an integer
bool is_integer(double val) {
	return FMath::Abs(val - FMath::RoundToDouble(val)) < 1e-10;
}

// Helper function to convert an Eigen vector expression to FString
template<typename T>
FString EigenVectorToFString(const T& vector_expr) {
	std::stringstream ss;
	ss << vector_expr;
	return FString(UTF8_TO_TCHAR(ss.str().c_str()));
}


// Function to handle both unique and infinite solution cases
void solveSystemAndIterate(Eigen::MatrixXd A, Eigen::VectorXd b) {
	Eigen::FullPivLU<Eigen::MatrixXd> lu_decomp(A);
	int N = A.cols();

	if (lu_decomp.isInvertible()) {
		// --- CASE 1: UNIQUE SOLUTION ---
		UE_LOG(LogTemp, Log, TEXT("System has a unique solution."));
		Eigen::VectorXd x = lu_decomp.solve(b);
		Eigen::IOFormat fmt(4, 0, ", ", "\n", "[", "]");

		bool is_non_negative = true; // Renamed for clarity
		bool is_int = true;

		for (int i = 0; i < N; ++i) {
			// Check if greater than or equal to 0
			if (x(i) < -1e-10) is_non_negative = false;
			if (!is_integer(x(i))) is_int = false;
		}

		if (is_non_negative && is_int) {
			FString x_str = EigenVectorToFString(x.cast<int>().format(fmt));
			UE_LOG(LogTemp, Log, TEXT("The unique solution is a non-negative integer vector (0 allowed): %s"), *x_str);
		}
		else {
			FString x_str = EigenVectorToFString(x.format(fmt));
			UE_LOG(LogTemp, Warning, TEXT("The unique solution does not meet non-negative integer criteria: %s"), *x_str);
		}

	}
	else {
		// --- CASE 2: INFINITE SOLUTIONS (Rank Deficient) ---
		UE_LOG(LogTemp, Log, TEXT("System is rank deficient (infinite solutions possible)."));

		Eigen::VectorXd x_p = lu_decomp.solve(b);
		Eigen::MatrixXd K = lu_decomp.kernel();
		Eigen::IOFormat fmt(4, 0, ", ", "\n", "[", "]");

		if (!FMath::IsNearlyZero((A * x_p - b).norm(), 1e-10)) {
			UE_LOG(LogTemp, Error, TEXT("System is inconsistent (no solution exists at all)."));
			return;
		}

		UE_LOG(LogTemp, Log, TEXT("Finding non-negative integer solutions using 'c' iteration (0 allowed):"));

		if (K.cols() != 1) {
			UE_LOG(LogTemp, Warning, TEXT("Kernel has %d dimensions. Cannot iterate with simple 'c' range."), K.cols());
			return;
		}

		Eigen::VectorXd K_basis = K.col(0);
		double c_lower_bound = std::numeric_limits<double>::lowest();
		double c_upper_bound = std::numeric_limits<double>::max();

		// Calculate continuous bounds for 'c' to ensure non-negative solutions (x_i >= 0)
		for (int i = 0; i < N; ++i) {
			double K_i = K_basis(i);
			// We solve the inequality: xp_i + c * K_i >= 0
			if (K_i > 0) {
				// c >= -xp_i / K_i
				c_lower_bound = FMath::Max(c_lower_bound, -x_p(i) / K_i);
			}
			else if (K_i < 0) {
				// c <= -xp_i / K_i (inequality flips)
				c_upper_bound = FMath::Min(c_upper_bound, -x_p(i) / K_i);
			}
			// If K_i == 0, that component must satisfy x_p(i) >= 0 independently of c.
			// If it doesn't, this entire problem has no solution meeting criteria.
		}

		// Tolerance is used here to allow c to fall exactly on the boundary that yields 0
		int c_start = FMath::CeilToInt(c_lower_bound - 1e-10);
		int c_end = FMath::FloorToInt(c_upper_bound + 1e-10);

		UE_LOG(LogTemp, Log, TEXT("Checking integer 'c' values from [%d, %d]"), c_start, c_end);

		if (c_start > c_end) {
			UE_LOG(LogTemp, Log, TEXT("No integer 'c' values fall within the non-negative range."));
			return;
		}

		bool found_solutions = false;
		for (int c = c_start; c <= c_end; ++c) {
			Eigen::VectorXd x = x_p + (double)c * K_basis;
			bool all_integers = true;
			bool all_non_negative = true;

			for (int i = 0; i < N; ++i) {
				if (!is_integer(x(i))) {
					all_integers = false;
				}
				// Double check the non-negativity constraint after forming the final vector
				if (x(i) < -1e-10) {
					all_non_negative = false;
				}
			}

			if (all_integers && all_non_negative) {
				FString x_str = EigenVectorToFString(x.cast<int>().format(fmt));
				UE_LOG(LogTemp, Display, TEXT("Found valid non-negative solution for c = %d: %s"), c, *x_str);
				found_solutions = true;
			}
		}

		if (!found_solutions) {
			UE_LOG(LogTemp, Log, TEXT("Did not find any non-negative integer solutions for x within the integer range of c."));
		}
	}
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

	// Solve for x
	Eigen::VectorXd x = A.fullPivLu().solve(b);
	UE_LOG(LogTemp, Warning, TEXT("Eigen solved: \n%s"), *toString(x));

	solveSystemAndIterate(A, b);
	
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
