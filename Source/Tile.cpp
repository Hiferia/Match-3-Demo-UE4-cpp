// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GridMgr.h"
#include "Kismet/GameplayStatics.h"


void ATile::BeginPlay()
{
	SetMaterialForType();

	TilePosition = GetActorLocation();

	RightDirection = GetActorRightVector() + FVector(0, 500, 0);
	LeftDirection = (GetActorRightVector() + FVector(0, 500, 0)) * - 1;
	UpDirection = GetActorUpVector() + FVector(500, 0, 0);
	DownDirection = (GetActorUpVector() + FVector(500, 0, 0)) * - 1;
}

ATile::ATile()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileMesh"));
	TileMesh->SetupAttachment(RootComponent);
	Root = nullptr;
	ShiftDelay = 1.0f;
}

void ATile::SetMaterialForType()
{
	switch (TileType)
	{
	case ETileType::ORANGE:
		TileMesh->SetMaterial(0, Materials[0]);
		break;
	case ETileType::BLACK:
		TileMesh->SetMaterial(0, Materials[1]);
		break;
	case ETileType::WHITE:
		TileMesh->SetMaterial(0, Materials[2]);
		break;
	case ETileType::BLUE:
		TileMesh->SetMaterial(0, Materials[3]);
		break;
	case ETileType::BROWN:
		TileMesh->SetMaterial(0, Materials[4]);
		break;
	case ETileType::RED:
		TileMesh->SetMaterial(0, Materials[5]);
		break;
	case ETileType::BOMB:
		TileMesh->SetMaterial(0, SpecialMaterials[0]);
		break;
	case ETileType::STRIPE:
		TileMesh->SetMaterial(0, SpecialMaterials[1]);
		break;
	default:
		break;
	}
	TileMesh->SetVisibility(true);
}

void ATile::NotifyActorOnClicked(FKey ButtonPressed)
{
	Super::NotifyActorOnClicked(ButtonPressed);
	if (Root->bIsShifting)
	{
		return;
	}
	if (TileType == ETileType::BOMB)
	{
		UE_LOG(LogTemp, Warning, TEXT("BOMB"));
		BombExplodes();
	}
	else if (TileType == ETileType::STRIPE)
	{
		UE_LOG(LogTemp, Warning, TEXT("STRIPE"));
		StripeExplodes();
	}
	else 
	{
		DeleteAllMatches();
		UE_LOG(LogTemp, Warning, TEXT("ELSE"));
	}
}

void ATile::BombExplodes()
{
	Root->bIsShifting = true;

	RayCastbomb(RightDirection); 
	RayCastbomb(LeftDirection); 
	RayCastbomb(UpDirection); 
	RayCastbomb(DownDirection); 

	TileMesh->SetVisibility(false);

	GetWorldTimerManager().SetTimer(MyTimer, this, &ATile::ExecuteGridRefill, ShiftDelay, false);
	UGameplayStatics::PlaySoundAtLocation(this, MatchSound, GetActorLocation());
}

void ATile::StripeExplodes()
{
	Root->bIsShifting = true;

	RayCastStripe(RightDirection); 
	RayCastStripe(LeftDirection); 

	TileMesh->SetVisibility(false);

	GetWorldTimerManager().SetTimer(MyTimer, this, &ATile::ExecuteGridRefill, ShiftDelay, false);
	
	UGameplayStatics::PlaySoundAtLocation(this, MatchSound, GetActorLocation());
}

void ATile::RayCastStripe(FVector Direction)
{
	FHitResult outHit;
	bool isHit = GetWorld()->LineTraceSingleByChannel(outHit, this->GetActorLocation(), this->GetActorLocation() * Direction, ECC_Visibility);
	while (isHit)
	{
		ATile* tile = static_cast<ATile*>(outHit.GetActor());
		tile->TileMesh->SetVisibility(false);
		isHit = GetWorld()->LineTraceSingleByChannel(outHit, outHit.GetActor()->GetActorLocation(), this->GetActorLocation() * Direction, ECC_Visibility);
	}
}

void ATile::RayCastbomb(FVector Direction)
{
	FHitResult outHit;
	bool isHit = GetWorld()->LineTraceSingleByChannel(outHit, this->GetActorLocation(), this->GetActorLocation() * Direction, ECC_Visibility);
	if (isHit)
	{
		ATile* tile = static_cast<ATile*>(outHit.GetActor());
		tile->TileMesh->SetVisibility(false);

		FVector tileLocation = tile->GetActorLocation();

		if (Direction == RightDirection)
		{
			SingleRayCast(tileLocation, (tileLocation * (tile->GetActorUpVector() + FVector(500, 0, 0)))); //Up
			SingleRayCast(tileLocation, (tileLocation * ((tile->GetActorUpVector() + FVector(500, 0, 0)) * - 1))); // Down
		}
		else if (Direction == LeftDirection)
		{
			SingleRayCast(tileLocation, (tileLocation * (tile->GetActorUpVector() + FVector(500, 0, 0)))); //Up
			SingleRayCast(tileLocation, (tileLocation * ((tile->GetActorUpVector() + FVector(500, 0, 0)) * -1))); // Down
		}
		else if (Direction == UpDirection)
		{
			SingleRayCast(tileLocation, (tileLocation * (tile->GetActorRightVector() + FVector(0, 500, 0)))); // Right
			SingleRayCast(tileLocation, (tileLocation * ((tile->GetActorRightVector() + FVector(0, 500, 0)) * -1))); // Left
		}
		else
		{
			SingleRayCast(tileLocation, (tileLocation * (tile->GetActorRightVector() + FVector(0, 500, 0)))); // Right
			SingleRayCast(tileLocation, (tileLocation * ((tile->GetActorRightVector() + FVector(0, 500, 0)) * -1))); // Left
		}
	}
}

void ATile::ExecuteGridRefill()
{
	Root->LookForNullTiles();
	Root->bIsShifting = false;
}

void ATile::SingleRayCast(FVector Position, FVector Direction)
{
	FHitResult outHit;
	bool isHit = GetWorld()->LineTraceSingleByChannel(outHit, Position, Direction, ECC_Visibility);
	if (isHit)
	{
		ATile* tile = static_cast<ATile*>(outHit.GetActor());
		tile->TileMesh->SetVisibility(false);
	}
}

TArray<ATile*> ATile::Match(FVector Position, FVector Direction)
{
	TArray<ATile*> matchingTiles;
	FHitResult outHit;
	bool isHit = GetWorld()->LineTraceSingleByChannel(outHit, Position, GetActorLocation() * Direction, ECC_WorldStatic);
	while (isHit)
	{
		ATile* tile = static_cast<ATile*>(outHit.GetActor());
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *UEnum::GetValueAsString(tile->TileType));
		if (tile->TileType == TileType)
		{
			matchingTiles.Add(tile);
			tile->TileMesh->SetVisibility(false);
			isHit = GetWorld()->LineTraceSingleByChannel(outHit, outHit.GetActor()->GetActorLocation(), this->GetActorLocation() * Direction, ECC_Visibility);
		}
		else
		{
			break;
		}
	}
	
	return matchingTiles;
}

void ATile::DeleteAllMatches()
{
	TArray<ATile*> matchingTiles;
	TArray<ATile*> adjacentTiles;
	matchingTiles.Append(Match(TilePosition, UpDirection));
	matchingTiles.Append(Match(TilePosition, DownDirection));
	matchingTiles.Append(Match(TilePosition, RightDirection));
	matchingTiles.Append(Match(TilePosition, LeftDirection));

	for (int i = 0; i < matchingTiles.Num(); i++)
	{
		FVector position = matchingTiles[i]->GetActorLocation();
		adjacentTiles.Append(Match(position, (position * (matchingTiles[i]->GetActorUpVector() + FVector(500, 0, 0))))); //Up
		adjacentTiles.Append(Match(position, (position * (matchingTiles[i]->GetActorUpVector() + FVector(500, 0, 0) * - 1)))); //Down
		adjacentTiles.Append(Match(position, (position * (matchingTiles[i]->GetActorRightVector() + FVector(0, 500, 0))))); //Right
		adjacentTiles.Append(Match(position, (position * (matchingTiles[i]->GetActorRightVector() + FVector(0, 500, 0) * -1)))); //Left
	}
	if (matchingTiles.Num() >= 1)
	{
		Root->bIsShifting = true;
		bIsMatchFound = true;
		UGameplayStatics::PlaySoundAtLocation(this, MatchSound, GetActorLocation());
	}
	
	if (bIsMatchFound)
	{
		TileMesh->SetVisibility(false);

		GetWorldTimerManager().SetTimer(MyTimer, this, &ATile::ExecuteGridRefill, ShiftDelay, false);

		bIsMatchFound = false;
	}
	
}




