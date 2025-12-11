#pragma once

#include "DayBase.h"
#include "Day10.generated.h"

struct ElvenMachine
{
	ElvenMachine() {
		DesiredState.Empty();
		Buttons.Empty();
		DesiredJolts.Empty();
		TargetTotalJolts = 0;
	}

	ElvenMachine(TArray<bool> InDesiredState, TArray<TArray<int>> InButtons, TArray<int> InJolts)
		: DesiredState(InDesiredState), Buttons(InButtons), DesiredJolts(InJolts)
	{
		TargetTotalJolts = 0;
		for (int i = 0; i < DesiredJolts.Num(); i++)
		{
			TargetTotalJolts += DesiredJolts[i] * (1LL << i);
		}
	}

	TArray<bool> DesiredState;
	TArray<TArray<int>> Buttons;
	TArray<int> DesiredJolts;
	int64 TargetTotalJolts;
};

struct MachineState
{
	TArray<bool> CurrentState;
	TArray<int> ButtonsPressed;
	TArray<int> JoltsGenerated;

	MachineState PressButton(const ElvenMachine & MachineDefinition, int ButtonNo) const
	{
		MachineState NextState = *this;
		for (int idx : MachineDefinition.Buttons[ButtonNo])
		{
			NextState.CurrentState[idx] = !NextState.CurrentState[idx];
			NextState.JoltsGenerated[idx] += 1;
		}
		NextState.ButtonsPressed[ButtonNo]++;
		return NextState;
	}

	bool IsValidJolts(const ElvenMachine& machineDefinition) const
	{
		for (int i = 0; i < machineDefinition.DesiredJolts.Num(); i++)
		{
			if (JoltsGenerated[i] > machineDefinition.DesiredJolts[i])
			{
				return false;
			}
		}
		return true;
	}
};

UCLASS()
class AOC_API ADay10 : public ADayBase
{
	GENERATED_BODY()
	
public:	
	ADay10();

protected:
	virtual void BeginPlay() override;

private:
	TArray<ElvenMachine> Machines;
	virtual FString DayName() const override { return "Day10"; }
	virtual int64 SolvePart1() override;
	virtual int64 SolvePart2() override;
};
