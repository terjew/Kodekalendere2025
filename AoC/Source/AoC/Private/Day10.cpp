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

TArray<int32> EigenToTArrayInt(const Eigen::VectorXd& eigen_vec) {
	TArray<int32> t_array;
	t_array.SetNumUninitialized(eigen_vec.size());
	for (int i = 0; i < eigen_vec.size(); ++i) {
		t_array[i] = static_cast<int32>(FMath::RoundToDouble(eigen_vec(i)));
	}
	return t_array;
}

bool is_integer(double val) {
	return FMath::Abs(val - FMath::RoundToDouble(val)) < 1e-10;
}

TArray<TArray<int32>> iterateAndCheckSolutions_1D(const Eigen::VectorXd& x_p, const Eigen::VectorXd& K_basis) {
	TArray<TArray<int32>> foundSolutions;
	int N = x_p.size();
	double c_lower_bound = std::numeric_limits<double>::lowest();
	double c_upper_bound = std::numeric_limits<double>::max();
	const double EPSILON = 1e-10;

	for (int i = 0; i < N; ++i) {
		double K_i = K_basis(i);
		if (K_i > EPSILON) {
			c_lower_bound = FMath::Max(c_lower_bound, -x_p(i) / K_i);
		}
		else if (K_i < -EPSILON) {
			c_upper_bound = FMath::Min(c_upper_bound, -x_p(i) / K_i);
		}
	}

	int c_start = FMath::CeilToInt(c_lower_bound - EPSILON);
	int c_end = FMath::FloorToInt(c_upper_bound + EPSILON);
	UE_LOG(LogTemp, Log, TEXT("1D Kernel: Checking optimal integer 'c' values from [%d, %d]"), c_start, c_end);

	if (c_start > c_end) return foundSolutions;

	for (int c = c_start; c <= c_end; ++c) {
		Eigen::VectorXd x = x_p + (double)c * K_basis;
		bool all_valid = true;
		for (int i = 0; i < N; ++i) {
			if (!is_integer(x(i)) || x(i) < -EPSILON) {
				all_valid = false; break;
			}
		}
		if (all_valid) {
			foundSolutions.Add(EigenToTArrayInt(x));
		}
	}
	UE_LOG(LogTemp, Log, TEXT("Found %d valid non-negative integer solutions in 1D case."), foundSolutions.Num());
	return foundSolutions;
}

TArray<TArray<int32>> iterateAndCheckSolutions(const Eigen::VectorXd& x_p, const Eigen::MatrixXd& K, int32 SearchRange) {
	int N = x_p.size();
	int K_dims = K.cols();
	TArray<TArray<int32>> foundSolutions;
	std::vector<int> current_c(K_dims, -SearchRange);
	bool done = false;

	UE_LOG(LogTemp, Log, TEXT("Searching a box from [%d, %d]^%d for non-negative integer solutions..."), -SearchRange, SearchRange, K_dims);

	while (!done) {
		Eigen::VectorXd x = x_p;
		for (int k_idx = 0; k_idx < K_dims; ++k_idx) x += (double)current_c[k_idx] * K.col(k_idx);
		bool all_valid = true;
		for (int i = 0; i < N; ++i) {
			if (x(i) < -1e-10 || !is_integer(x(i))) { all_valid = false; break; }
		}
		if (all_valid) { foundSolutions.Add(EigenToTArrayInt(x)); }

		int dim_idx = K_dims - 1;
		while (dim_idx >= 0) {
			current_c[dim_idx]++;
			if (current_c[dim_idx] > SearchRange) {
				current_c[dim_idx] = -SearchRange; dim_idx--;
			}
			else { break; }
		}
		if (dim_idx < 0) done = true;
	}
	UE_LOG(LogTemp, Log, TEXT("Found %d valid non-negative integer solutions within search range."), foundSolutions.Num());
	return foundSolutions;
}

TArray<TArray<int32>> solveSystemAndIterate(Eigen::MatrixXd A, Eigen::VectorXd b) {
	Eigen::FullPivLU<Eigen::MatrixXd> lu_decomp(A);
	TArray<TArray<int32>> SolutionsList;

	if (lu_decomp.isInvertible()) {
		//Only a single unique solution exists
		Eigen::VectorXd x = lu_decomp.solve(b);
		bool is_non_negative = true; bool is_int = true;
		for (int i = 0; i < x.size(); ++i) {
			if (x(i) < -1e-10) is_non_negative = false;
			if (!is_integer(x(i))) is_int = false;
		}
		if (is_non_negative && is_int) {
			SolutionsList.Add(EigenToTArrayInt(x));
			UE_LOG(LogTemp, Log, TEXT("Found unique solution satisfying criteria."));
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("Unique solution did not meet non-negative integer criteria. Returning empty list."));
		}
		return SolutionsList;
	}

	Eigen::VectorXd x_p = lu_decomp.solve(b);
	Eigen::MatrixXd K = lu_decomp.kernel();

	if (!FMath::IsNearlyZero((A * x_p - b).norm(), 1e-10)) {
		UE_LOG(LogTemp, Error, TEXT("System is inconsistent (no solution exists at all). Returning empty list."));
		return SolutionsList;
	}

	UE_LOG(LogTemp, Log, TEXT("Kernel dimension is %d."), K.cols());

	if (K.cols() == 1) {
		return iterateAndCheckSolutions_1D(x_p, K.col(0));
	}

	double max_b_val = b.array().maxCoeff();
	int32 search_range_heuristic = FMath::CeilToInt(max_b_val) + 5;

	search_range_heuristic = FMath::Min(search_range_heuristic, 300);

	SolutionsList = iterateAndCheckSolutions(x_p, K, search_range_heuristic);

	if (SolutionsList.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("No valid solutions found!"));
	}
	return SolutionsList;
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

	UE_LOG(LogTemp, Log, TEXT("A:\n%s"), *toString(A));
	UE_LOG(LogTemp, Log, TEXT("b:\n%s"), *toString(b));

	TArray<TArray<int>> solutions = solveSystemAndIterate(A, b);
	
	auto best = UnrealLinq::MinBy(solutions, [](const TArray<int>& solution) {
		return UnrealLinq::Sum(solution);
	});

	return UnrealLinq::Sum(best);
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

	Part1 = SolvePart1();
	Part2 = SolvePart2();
}
