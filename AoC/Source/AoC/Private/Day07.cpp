#include "Day07.h"
#include "Util.h"
#include "Linq.h"

ADay07::ADay07()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> TorusMeshAsset(TEXT("/Engine/BasicShapes/Sphere"));
	StartMesh = TorusMeshAsset.Object;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ConeMeshAsset(TEXT("/Engine/BasicShapes/Cone"));
	SplitterMesh = ConeMeshAsset.Object;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderMeshAsset(TEXT("/Engine/BasicShapes/Cylinder"));
	BeamMesh = CylinderMeshAsset.Object;

	// Create and set the Default Scene Root
	USceneComponent* DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SetRootComponent(DefaultRoot);
}

int64 ADay07::SolvePart1()
{
	return 0;
}

int64 ADay07::SolvePart2()
{
	return 0;
}

void ADay07::BeginPlay()
{
	Super::BeginPlay();
	auto Lines = Util::FileAsLines(GetInputFilePath());
	Width = Lines[0].Len();
	Height = Lines.Num();

	Data = UnrealLinq::Select(Lines, [](const FString& Line) {
		return Line.GetCharArray();
	});

	Cells.Empty(Width * Height);
	Paths.Empty(Height);
	auto root = GetRootComponent();
	for (int y = 0; y < Height; y++)
	{
		Paths.Add(TArray<uint64>());
		for (int x = 0; x < Width; x++)
		{
			Paths.Last().Add(0);
			UStaticMeshComponent* Cell = NewObject<UStaticMeshComponent>(this);
			switch (Data[y][x])
			{
				case _T('S'):
					Paths[CurrentStep][x] = 1; //only one timeline for the start
					Cell->SetStaticMesh(StartMesh);
					Cell->SetMaterial(0, ActivatedMaterial);
					break;
				case _T('^'):
					Cell->SetStaticMesh(SplitterMesh);
					Cell->SetRelativeScale3D(FVector(2.0f, 1.0f, 1.0f));
					Cell->SetRelativeRotation(FRotator(0.0f, 0.0f, -90.0f));
					break;
				case _T('.'):
					Cell->SetStaticMesh(BeamMesh);
					Cell->SetVisibility(false);
					Cell->SetRelativeScale3D(FVector(0.2f, 0.2f, 1.0f));
					Cell->SetRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));
					break;
				default:
					break;
			}
			Cell->SetupAttachment(root);
			Cell->RegisterComponent();
			Cell->SetRelativeLocation(FVector(x * 100.0f, y * 100.0f, 0.0f));
			Cells.Add(Cell);
		}
	}
	Part1 = 0;
	Part2 = 0;
	GetWorldTimerManager().SetTimer(MyTimerHandle, this, &ADay07::Step, 0.01f, true, 3.0f);
}

void ADay07::ActivateBeam(int x, int y, uint64 timelines)
{
	Data[y][x] = _T('|');
	Paths[y][x] += timelines; //accumulate number of possible timelines leading to this beam
	int index = y * Width + x;
	Cells[index]->SetVisibility(true);
	Cells[index]->SetMaterial(0, ActivatedMaterial);
}

void ADay07::Step()
{
	if (CurrentStep >= Height - 1)
	{
		GetWorldTimerManager().ClearTimer(MyTimerHandle);
		uint64 totalTimelines = 0;

		for (int x = 0; x < Width; x++)
		{
			totalTimelines += Paths[CurrentStep][x];
		}
		Part2 = totalTimelines;
		return;
	}

	for (int x = 0; x < Width; x++)
	{		
		TCHAR& cell = Data[CurrentStep][x];

		if (cell == _T('S') || cell == _T('|'))
		{
			uint64 timelines = Paths[CurrentStep][x];
			auto below = Data[CurrentStep + 1][x];
			if (below == '^') {
				Part1++;
				Cells[(CurrentStep + 1) * Width + x]->SetMaterial(0, ActivatedMaterial);
				ActivateBeam(x - 1, CurrentStep + 1, timelines);
				ActivateBeam(x + 1, CurrentStep + 1, timelines);
			}
			else { // . | or whatever
				ActivateBeam(x, CurrentStep + 1, timelines);
			}
			
		}
	}
	CurrentStep++;
}