// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();


	StartButton->OnClicked.AddUniqueDynamic(this, &UMenuWidget::OnStartButtonClicked);
	ExitButton->OnClicked.AddUniqueDynamic(this, &UMenuWidget::OnExitButtonClicked);
}



void UMenuWidget::OnStartButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), "GameLevel");
}

void UMenuWidget::OnExitButtonClicked()
{
	GetWorld()->GetFirstPlayerController()->ConsoleCommand("quit");
}
