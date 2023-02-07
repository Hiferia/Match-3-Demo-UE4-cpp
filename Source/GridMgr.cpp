// Fill out your copyright notice in the Description page of Project Settings.


#include "GridMgr.h"
#include "Tile.h"
#include "Kismet/GameplayStatics.h"
#include "DemoPlayer.h"


// Sets default values
AGridMgr::AGridMgr()
{
}

void AGridMgr::LookForNullTiles()
{
	int tilesDestroyed = 0;
	for (int y = 0; y < SizeY; y++)
	{
		for (int x = 0; x < SizeX; x++)
		{
			if (Grid[x][y]->TileMesh->GetVisibleFlag() == false)
			{
				SetTileType(Grid[x][y]);
				Grid[x][y]->SetMaterialForType();

				tilesDestroyed++;
			}
		}
	}
	ADemoPlayer* player = static_cast<ADemoPlayer*>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	player->UpdateScore(tilesDestroyed);
	player->UpdateTimer(tilesDestroyed);
}

// Called when the game starts or when spawned
void AGridMgr::BeginPlay()
{
	Super::BeginPlay();

	Grid.SetNumZeroed(SizeX);

	for (int i = 0; i < Grid.Num(); i++)
	{
		Grid[i].SetNumZeroed(SizeX);
	}

	// Starting from 1 because positions set to 0 give problems with raycasts
	for (int y = 1; y <= SizeY; y++)
	{
		for (int x = 1; x <= SizeX; x++)
		{
			ATile* newTile = GetWorld()->SpawnActor<ATile>(TileToSpawn2, FVector(FIntPoint(x * Offset, y * Offset)), FRotator(-90, 0, 0));
			float rand = FMath::RandRange(0.0f, 1.0f);
			SetTileType(newTile);
			newTile->Root = this;
			

			// -1 to use indexes from 0
			newTile->TileIndex = FIntPoint(x - 1, y - 1);
			Grid[x - 1][y - 1] = newTile;
		}
	}

}

void AGridMgr::SetTileType(ATile* Tile)
{
	float rand = FMath::RandRange(0.0f, 1.0f);
	if (rand <= ChanceOfSpecialTile)
	{
		Tile->TileType = static_cast<ETileType>(FMath::RandRange(static_cast<int>(ETileType::BOMB), static_cast<int>(ETileType::STRIPE)));
	}
	else
	{
		Tile->TileType = static_cast<ETileType>(FMath::RandRange(0, static_cast<int>(ETileType::RED)));
	}
}



