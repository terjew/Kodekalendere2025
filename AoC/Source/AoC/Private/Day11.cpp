#include "Day11.h"
#include "Util.h"
#include "Linq.h"
#include "Containers/StringFwd.h"
#include <chrono>

ADay11::ADay11()
{
}

int64 CountPaths(TSharedRef<Device> From, TSharedRef<Device> To)
{
	if (From == To) return 1;
	if (From->Connections.Num() == 0) return 0;
	int64 paths = 0;
	for (auto& child : From->Connections)
	{
		paths += CountPaths(child, To);
	}


	return paths;
}

int64 ADay11::SolvePart1()
{
	return CountPaths(Devices[TEXT("you")], Devices[TEXT("out")]);
}

int64 CountPathsMemo(TSharedRef<Device> From, TSharedRef<Device> To, TMap<TSharedRef<Device>, int64> & PathCounts)
{
	if (PathCounts.Contains(From)) {
		return PathCounts[From];
	}

	int count = 0;
	if (From == To) {
		count = 1;
	}
	else if (From->Connections.Num() == 0) {
		count = 0;
	}

	else {
		count = 0;
		for (auto& child : From->Connections)
		{
			count += CountPathsMemo(child, To, PathCounts);
		}
	}

	PathCounts.Add(From, count);
	return count;
}

int64 ADay11::SolvePart2()
{
	TMap<TSharedRef<Device>, int64> PathCountsStart;
	int64 start = CountPathsMemo(Devices[TEXT("svr")], Devices[TEXT("fft")], PathCountsStart);

	TMap<TSharedRef<Device>, int64> PathCountsMid;
	int64 mid = CountPathsMemo(Devices[TEXT("fft")], Devices[TEXT("dac")], PathCountsMid);

	TMap<TSharedRef<Device>, int64> PathCountsEnd;
	int64 end = CountPathsMemo(Devices[TEXT("dac")], Devices[TEXT("out")], PathCountsEnd);

	return start * mid * end;
}

void ADay11::BeginPlay()
{
	Super::BeginPlay();
	auto Lines = Util::FileAsLines(GetInputFilePath());

	TStringBuilder<128> dia;
	dia << TEXT("digraph G {\n");
	dia << TEXT("    you [style=filled fillcolor=cyan];");
	dia << TEXT("    svr [style=filled fillcolor=magenta];");
	dia << TEXT("    out [style=filled fillcolor=green];");
	dia << TEXT("    fft [style=filled fillcolor=red];");
	dia << TEXT("    dac [style=filled fillcolor=gold];");

	for (const FString& Line : Lines)
	{
		TArray<FString> Parts;
		Line.ParseIntoArray(Parts, TEXT(" "), true);
		FString DeviceName = Parts[0].LeftChop(1);
		TSharedRef<Device> DeviceRef = MakeShared<Device>(DeviceName);
		if (Devices.Contains(DeviceName)) {
			DeviceRef = Devices[DeviceName];
		}
		else {
			Devices.Add(DeviceName, DeviceRef);
		}
		for (int32 i = 1; i < Parts.Num(); ++i)
		{
			FString ConnectionName = Parts[i];
			TSharedRef<Device> ConnectionRef = MakeShared<Device>(ConnectionName);
			if (Devices.Contains(ConnectionName)) {
				ConnectionRef = Devices[ConnectionName];
			}
			else {
				Devices.Add(ConnectionName, ConnectionRef);
			}
			DeviceRef->Connections.Add(ConnectionRef);
			dia << TEXT("    ") <<  DeviceName << TEXT(" -> ") << ConnectionName << TEXT(";\n");
		}
	}

	dia << TEXT("}\n");
	FString Diagram = *dia;
	UE_LOG(LogTemp, Warning, TEXT("Loaded %d devices"), Devices.Num());
	UE_LOG(LogTemp, Warning, TEXT("Dia: \n%s"), *Diagram);

	Part1 = SolvePart1();
	Part2 = SolvePart2();
}
