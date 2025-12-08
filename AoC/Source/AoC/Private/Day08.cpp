#include "Day08.h"
#include "Util.h"
#include "Linq.h"

ADay08::ADay08()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset(TEXT("/Engine/BasicShapes/Cube"));
	CubeMesh = CubeMeshAsset.Object;

	USceneComponent* DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SetRootComponent(DefaultRoot);

}

int64 ADay08::SolvePart1()
{
	return 0;
}

int64 ADay08::SolvePart2()
{
	return 0;
}

TArray<TSet<int32>> ADay08::FindComponents() const
{
	//find all connected components in the graph represented by Vertices
	TArray<TSet<int32>> components;
	TSet<int32> visited;

	for (int start = 0; start < Vertices.Num(); ++start)
	{
		if (visited.Contains(start))
		{
			continue;
		}
		//first time encountering this vertex, start a new component
		TSet<int32> component;
		TArray<int32> stack;
		stack.Add(start);
		visited.Add(start);
		while (stack.Num() > 0)
		{
			int32 current = stack.Pop();
			component.Add(current);
			for (auto neighbor : Vertices[current].ConnectedVertices)
			{
				if (!visited.Contains(neighbor))
				{
					visited.Add(neighbor);
					stack.Add(neighbor);
				}
			}
		}
		components.Add(component);
	}

	components.Sort([](const TSet<int32>& a, const TSet<int32>& b) {
		return a.Num() > b.Num();
	});

	return components;
}

void ADay08::BeginPlay()
{
	Super::BeginPlay();
	auto Lines = Util::FileAsLines(GetInputFilePath());
	auto Coordinates = UnrealLinq::Select(Lines, [](const FString& Line) {
		TArray<FString> Parts;
		Line.ParseIntoArray(Parts, TEXT(","));
		auto nums = UnrealLinq::Select(Parts, Util::StringToInt64);
		return FVector(nums[0], nums[1], nums[2]);
	});

	auto maxVec = UnrealLinq::MaxBy(Coordinates, [](const FVector vec) { return (float)vec.X; });
	float maxX = maxVec.X;
	float scale = FMath::Sqrt(maxX) / 3;

	Vertices = UnrealLinq::Select(Coordinates, [n = 0](const FVector vec) mutable {
		return FVertex(n++, vec);
	});

	Cells.Empty(Coordinates.Num());
	auto root = GetRootComponent();
	for (auto & Coordinate : Coordinates)
	{
		auto scaleVec = FVector(scale, scale, scale);
		UStaticMeshComponent* Cell = NewObject<UStaticMeshComponent>(this);
		Cell->SetStaticMesh(CubeMesh);
		Cell->SetRelativeScale3D(scaleVec);
		Cell->SetupAttachment(root);
		Cell->RegisterComponent();
		Cell->SetRelativeLocation(Coordinate * 10.0f);
		Cells.Add(Cell);
	}

	TArray<std::tuple<int32, int32, double>> Distances;
	for (int32 i = 0; i < Coordinates.Num(); ++i)
	{
		for (int32 j = i+1; j < Coordinates.Num(); ++j)
		{
			Distances.Add(std::make_tuple(i, j, FVector::Dist(Coordinates[i], Coordinates[j])));
		}
	}
	Distances.Sort([](const std::tuple<int32, int32, double>& a, const std::tuple<int32, int32, double>& b) {
		return std::get<2>(a) < std::get<2>(b);
	});

	auto transform = GetActorTransform();
	for (int i = 0; i < (UseSample ? 10 : 1000) ; ++i)
	{
		auto shortestDistance = Distances[i];
		int32 a = std::get<0>(shortestDistance);
		int32 b = std::get<1>(shortestDistance);
		Vertices[a].ConnectedVertices.Add(b);
		Vertices[b].ConnectedVertices.Add(a);
		//draw all the connections using DrawDebugLine
		DrawDebugLine(GetWorld(), transform.TransformPosition(Coordinates[a] * 10.0f), transform.TransformPosition(Coordinates[b] * 10.0f), FColor::Yellow, true, -1, 0, transform.GetScale3D()[0] * scale * 10);
		Cells[a]->SetMaterial(0, ConnectedMaterial);
		Cells[b]->SetMaterial(0, ConnectedMaterial);
	}

	auto components = FindComponents();

	int product = 1;
	for (int i = 0; i < 3; ++i)
	{
		product *= components[i].Num();
	}
	Part1 = product;
}
