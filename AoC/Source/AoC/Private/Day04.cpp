#include "Day04.h"
#include "Util.h"
#include "Linq.h"

ADay04::ADay04()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset(TEXT("/Engine/BasicShapes/Cube"));
	CubeMesh = CubeMeshAsset.Object;

	// Create and set the Default Scene Root
	USceneComponent* DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SetRootComponent(DefaultRoot);
}

bool ADay04::IsAccessible(int x, int y)
{
	int occupied = 0;
	for (int dy = -1; dy <= 1; dy++)
	{
		for (int dx = -1; dx <= 1; dx++)
		{
			if (dx == 0 && dy == 0)
			{
				continue;
			}
			int nx = x + dx;
			int ny = y + dy;
			if (nx < 0 || nx >= Width || ny < 0 || ny >= Height)
			{
				continue;
			}
			if (Data[ny][nx] == '@')
			{
				occupied++;
				if (occupied > 3)
				{
					return false;
				}
			}
		}
	}
	Data[y][x] = 'x';
	return true;
}

int64 ADay04::SolvePart1()
{
	int numAccessible = 0;
	for (int y = 0; y < Height; y++)
	{
		for (int x = 0; x < Width; x++)
		{
			char c = Data[y][x];
			if (c != '@')
			{
				continue;
			}
			if (IsAccessible(x, y))
			{
				numAccessible++;
				Cubes[y * Width + x]->SetMaterial(0, AccessibleMaterial);
			}
		}
	}
	return numAccessible;
}

int64 ADay04::SolvePart2()
{
	return 0;
}

void ADay04::BeginPlay()
{
	Super::BeginPlay();
	auto Lines = Util::FileAsLines(TEXT("Day04/input.txt"));
	Width = Lines[0].Len();
	Height = Lines.Num();

	Data = UnrealLinq::Select(Lines, [](const FString& Line) {
		return Line.GetCharArray();
	});

	Cubes.Empty(Width * Height);
	auto root = GetRootComponent();
	for (int y = 0; y < Height; y++)
	{
		for (int x = 0; x < Width; x++)
		{

			UStaticMeshComponent* Cube = NewObject<UStaticMeshComponent>(this);
			Cube->SetStaticMesh(CubeMesh);
			Cube->SetupAttachment(root);
			Cube->RegisterComponent();
			Cube->SetRelativeLocation(FVector(x * 100.0f, y * 100.0f, 0.0f));
			if (Data[y][x] != '@')
			{
				Cube->SetVisibility(false);
			}
			Cubes.Add(Cube);
		}
	}

	Part1 = SolvePart1();
	Part2 = Part1;
	GetWorldTimerManager().SetTimer(MyTimerHandle, this, &ADay04::Step, 0.05f, true, 3.0f);
}

void ADay04::Step()
{
	//cleanup previous step:
	for (int y = 0; y < Height; y++)
		for (int x = 0; x < Width; x++)
		{
			if (Data[y][x] == 'x') {
				Data[y][x] = '.';
				Cubes[y * Width + x]->SetVisibility(false);
			}
		}
	//calculate this step:
	Part2 += SolvePart1();
}


