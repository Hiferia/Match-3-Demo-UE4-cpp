// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverWidget.generated.h"

/**
 *
 */
UCLASS()
class REDBITPROJ_API UGameOverWidget : public UUserWidget
{
	GENERATED_BODY()

	void NativeConstruct() override;

	UFUNCTION()
		FText UpdateScore();

	UFUNCTION()
		void OnStartButtonClicked();

	UFUNCTION()
		void OnExitButtonClicked();

	UFUNCTION()
		void OnResetButtonClicked();

	UPROPERTY(meta = (BindWidget))
		class UButton* StartButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* ExitButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* ResetButton;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* PlayerScoreText;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* HighScoreText;

protected:
	bool Initialize() override;

	bool bIsResetting;

	class ADemoPlayer* player;
};
