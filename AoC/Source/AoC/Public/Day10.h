#pragma once

#include "DayBase.h"
#include "Day10.generated.h"

struct ElvenMachine
{
	TArray<bool> DesiredState;
	TArray<TArray<int>> Buttons;
	TArray<int> Jolts;
};

struct MachineState
{
	TArray<bool> CurrentState;
	TArray<int> ButtonsPressed;
	MachineState PressButton(const ElvenMachine & machineDefinition, int buttonNo) const
	{
		MachineState newState = *this;
		for (int idx : machineDefinition.Buttons[buttonNo])
		{
			newState.CurrentState[idx] = !newState.CurrentState[idx];
		}
		newState.ButtonsPressed.Add(buttonNo);
		return newState;
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
