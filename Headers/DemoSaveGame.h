// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "DemoSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class REDBITPROJ_API UDemoSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UDemoSaveGame();

	UPROPERTY(VisibleAnywhere, Category = "Score")
		int HighScore;

	UPROPERTY(VisibleAnywhere, Category = "Score")
		int CurrentGameScore;
};
