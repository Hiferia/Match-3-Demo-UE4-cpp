// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridMgr.generated.h"

class ATile;

DECLARE_DELEGATE(FLookForNullTilesDelagate);

UCLASS()
class REDBITPROJ_API AGridMgr : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridMgr();

	/*
	 * This function will iterate the entire grid, checking if some tile is not visible.
	 * If it founds a tile not visible, it will set a random TileType to the tile and call the "SetMaterialForType" on it.
	 * At the end of the iteration the grid will be refilled.
	 * It will update the score and the timer base on how many tiles it have found.
	 */
	void LookForNullTiles();


	FLookForNullTilesDelagate OnExecuteLookForNullTilesDelegate;

	bool bIsShifting;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
protected:

	TArray<TArray<ATile*>> Grid;

	UPROPERTY(EditAnywhere, Category = "Grid")
		int SizeX;

	UPROPERTY(EditAnywhere, Category = "Grid")
		int SizeY;

	UPROPERTY(EditAnywhere, Category = "Grid")
		float Offset;

	UPROPERTY(EditAnywhere, Category = "Grid")
		float ChanceOfSpecialTile;

	UPROPERTY(EditAnywhere, Category = "Grid")
		TSubclassOf<ATile> TileToSpawn2;

	/*
	 * This function will set a random TileType to the given tile. The chance of a special tile can be changed with "ChanceOfSpecialTile"
	 * @param Tile the tile that will get the random TileType
	 */
	void SetTileType(ATile* Tile);
};
