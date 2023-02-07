// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "DemoPlayer.h"
#include "DemoSaveGame.h"


void UGameOverWidget::NativeConstruct()
{
	Super::NativeConstruct();


	StartButton->OnClicked.AddUniqueDynamic(this, &UGameOverWidget::OnStartButtonClicked);
	ExitButton->OnClicked.AddUniqueDynamic(this, &UGameOverWidget::OnExitButtonClicked);
	ResetButton->OnClicked.AddUniqueDynamic(this, &UGameOverWidget::OnResetButtonClicked);
}



void UGameOverWidget::OnStartButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), "GameLevel");
}

void UGameOverWidget::OnExitButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), "MenuLevel");
}

void UGameOverWidget::OnResetButtonClicked()
{
	if (UDemoSaveGame* SaveGameInstance = Cast<UDemoSaveGame>(UGameplayStatics::CreateSaveGameObject(UDemoSaveGame::StaticClass())))
	{
		SaveGameInstance->HighScore = 0;
		//HighScoreText->Text = FText::FromString(FString::FromInt(SaveGameInstance->HighScore));
		UGameplayStatics::SaveGameToSlot(SaveGameInstance, "SaveSlot_1", 0);
	}
	bIsResetting = true;
}

bool UGameOverWidget::Initialize()
{
	bool success = Super::Initialize();

	if (UDemoSaveGame* LoadedGame = Cast<UDemoSaveGame>(UGameplayStatics::LoadGameFromSlot("SaveSlot_1", 0)))
	{
		HighScoreText->Text = FText::FromString(FString::FromInt(LoadedGame->HighScore));
		PlayerScoreText->Text = FText::FromString(FString::FromInt(LoadedGame->CurrentGameScore));

	}
	HighScoreText->TextDelegate.BindUFunction(this, "UpdateScore");
	bIsResetting = false;
	return true;
}

FText UGameOverWidget::UpdateScore()
{
	if (bIsResetting)
	{
		return FText::FromString("0");
	}
	else 
	{
		return PlayerScoreText->Text;
	}
}

