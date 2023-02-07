// Fill out your copyright notice in the Description page of Project Settings.


#include "DemoPlayer.h"
#include "Tile.h"
#include "DemoSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "DemoWidget.h"


// Sets default values
ADemoPlayer::ADemoPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	HighScore = 0;
	Score = 0;
	Timer = 120.0f;
}

// Called when the game starts or when spawned
void ADemoPlayer::BeginPlay()
{
	Super::BeginPlay();


	if (UDemoSaveGame* LoadedGame = Cast<UDemoSaveGame>(UGameplayStatics::LoadGameFromSlot("SaveSlot_1", 0)))
	{
		HighScore = LoadedGame->HighScore;
		LoadedGame->CurrentGameScore = 0;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Start Highscore: %d"), HighScore);
}

// Called every frame
void ADemoPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Timer -= DeltaTime;

	if (Timer <= 0)
	{
		UGameplayStatics::OpenLevel(GetWorld(), "GameOverLevel");
	}
}

// Called to bind functionality to input
void ADemoPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ADemoPlayer::UpdateScore(int TilesDestroyed)
{
	float pow = FMath::Pow((TilesDestroyed - 2) / 5, 2);
	int points = (TilesDestroyed - 1) * 80 + pow;
	Score += points;
	if (HighScore < Score)
	{
		HighScore = Score;
	}

	// Save
	if (UDemoSaveGame* SaveGameInstance = Cast<UDemoSaveGame>(UGameplayStatics::CreateSaveGameObject(UDemoSaveGame::StaticClass())))
	{
		SaveGameInstance->HighScore = HighScore;
		SaveGameInstance->CurrentGameScore = Score;

		UGameplayStatics::SaveGameToSlot(SaveGameInstance, "SaveSlot_1", 0);
	}
}

void ADemoPlayer::UpdateTimer(int TilesDestroyed)
{
	float pow = FMath::Pow((TilesDestroyed - 2) / 3, 2);
	Timer += 10 + pow * 20;
	UE_LOG(LogTemp, Warning, TEXT("Timer is: %f"), Timer);
}
	


