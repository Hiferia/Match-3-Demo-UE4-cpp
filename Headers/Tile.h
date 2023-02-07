// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

class AGridMgr;

UENUM()
enum class ETileType : uint16
{
	ORANGE,
	BLACK,
	WHITE,
	BLUE,
	BROWN,
	RED,
	BOMB,
	STRIPE,
	LAST UMETA(Hidden)
};


UCLASS()
class REDBITPROJ_API ATile : public AActor
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

public:	
	// Sets default values for this actor's properties
	ATile();
	AGridMgr* Root;
public:
	
	UPROPERTY(VisibleInstanceOnly, Category = "Tile")
		FIntPoint TileIndex;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tile")
		ETileType TileType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tile")
		class UStaticMeshComponent* TileMesh;
	
	UPROPERTY(EditAnywhere, Category = "Tile")
		TArray<UMaterialInterface*> Materials;

	UPROPERTY(EditAnywhere, Category = "Tile")
		TArray<UMaterialInterface*> SpecialMaterials;

	UPROPERTY(EditAnywhere, Category = "Tile")
		float ShiftDelay;

	UPROPERTY(EditAnywhere, Category = "Sounds")
		class USoundBase* MatchSound;

	/*
	 * This function will set the material of the tile based on the TileType
	 */
	void SetMaterialForType();

private:
	FVector TilePosition;
	FVector RightDirection;
	FVector LeftDirection;
	FVector UpDirection;
	FVector DownDirection;
	FTimerHandle MyTimer;
	bool bIsMatchFound;
	void NotifyActorOnClicked(FKey ButtonPressed = EKeys::LeftMouseButton) override;

	/*
	 * This function will call 4 raycast in Up, Down, Right and Left directions, making the tile bomb and the adjacent tiles explode
	 */
	void BombExplodes();

	/*
	 * This function will call 2 raycast in Right and Left directions, making the tile stripe and all the tiles int that row explode
	 */
	void StripeExplodes();

	/*
	 * This function will call 1 raycast in given direction, if it finds a tile it will continue to raycast in that direction.
	 * Every tile found will be deleted
	 * @param Direction the direction the raycast will be cast in
	 */
	void RayCastStripe(FVector Direction);

	/*
	 * This function will call 1 raycast in given direction, if it finds a tile it will cast 2 other rays in the directions perpendicular
	 * to the given direction.
	 * Every tile found will be deleted
	 * @param Direction the direction the raycast will be cast in
	 */
	void RayCastbomb(FVector Direction);

	/*
	 * This function will call the "LookForNullTiles" from the root component that is the GridMgr. 
	 * This will make the grid refills every empty slots.
	 */
	void ExecuteGridRefill();

	/*
	 * This function will call 1 raycast in given direction, from the given position. If it founds a tile, that one will be deleted.
	 * @param Position the starting position of the raycast
	 * @param Direction the direction the raycast will be cast in
	 */
	void SingleRayCast(FVector Position, FVector Direction);

	/*
	 * This function will call 1 raycast in given direction, from the given position.
	 * If it founds a tile with the same TileType of the Tile where the player clicked on, that will be a match and the tiles will be deleted
	 * @param Position the starting position of the raycast
	 * @param Direction the direction the raycast will be cast in
	 */
	TArray<ATile*> Match(FVector Position, FVector Direction);

	/*
	 * This function will call the Match() method for every adjacent tile with the same TileType of the tile the player clicked on
	 */
	void DeleteAllMatches();
};
