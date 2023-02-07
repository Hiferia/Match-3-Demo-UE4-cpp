// Fill out your copyright notice in the Description page of Project Settings.


#include "DemoWidget.h"
#include "DemoPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"


void UDemoWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ExitButton->OnClicked.AddUniqueDynamic(this, &UDemoWidget::OnExitButtonClicked);
}

FText UDemoWidget::UpdateTextTimer()
{
	return FText::FromString(FString::FromInt(player->Timer));
}

FText UDemoWidget::UpdateScore()
{
	return FText::FromString(FString::FromInt(player->Score));
}

bool UDemoWidget::Initialize()
{
	bool success = Super::Initialize();

	player = static_cast<ADemoPlayer*>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	
	if (!success)
	{
		return false;
	}
	TimerValueText->TextDelegate.BindUFunction(this, "UpdateTextTimer");
	ScoreText->TextDelegate.BindUFunction(this, "UpdateScore");
	return true;
}

void UDemoWidget::OnExitButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), "GameOverLevel");
}
