// Fill out your copyright notice in the Description page of Project Settings.


#include "Day01.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFileManager.h" // Needed for FPlatformFileManager

// Sets default values
ADay01::ADay01()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

int SolvePart1(TArray<int32> Rotations)
{
	int count = 0;
	int cur = 50;
	for (int rotation : Rotations) {
		cur += rotation;
		cur %= 100;
		if (cur == 0) {
			count++;
		}
	}
	return count;
}

int SolvePart2(TArray<int32> Rotations)
{
	int count = 0;
	int cur = 50;
	for (int rotation : Rotations)
	{
		if (abs(rotation) >= 100) {
			int whole = rotation / 100;
			rotation -= whole * 100;
			count += abs(whole);
		}
		bool was0 = cur == 0;
		cur += rotation;
		if (cur < 0) {
			cur += 100;
			if (!was0) count++;
		}
		else if (cur >= 100) {
			cur -= 100;
			count++;
		}
		else if (cur == 0) {
			count++;
		}
	}
	return count;
}

// Called when the game starts or when spawned
void ADay01::BeginPlay()
{
	Super::BeginPlay();
	FString FilePath = FPaths::ProjectContentDir() + TEXT("Day01/input.txt");
	FString FileContent;
	if (FFileHelper::LoadFileToString(FileContent, *FilePath))
	{
		// File content successfully loaded into FileContent FString
		UE_LOG(LogTemp, Warning, TEXT("File content: %s"), *FileContent);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load file: %s"), *FilePath);
	}

	TArray<FString> Lines;
	FString Delimiter = TEXT("\n");
	FileContent.ParseIntoArray(Lines, *Delimiter, true);

	Rotations.SetNum(Lines.Num());
	std::transform(Lines.GetData(), Lines.GetData() + Lines.Num(), Rotations.GetData(), 
		[](FString& line)
		{
			auto firstChar = line.Left(1);
			int sign = (firstChar == TEXT("L")) ? -1 : 1;
			auto rest = line.RightChop(1);
			return sign * FCString::Atoi(*rest);
		});

	Part1 = SolvePart1(Rotations);
	Part2 = SolvePart2(Rotations);
}

// Called every frame
void ADay01::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

