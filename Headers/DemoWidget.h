// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DemoWidget.generated.h"

/**
 *
 */
UCLASS()
class REDBITPROJ_API UDemoWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
		class UButton* ExitButton;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ScoreText;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TimerValueText;

	UFUNCTION()
		FText UpdateTextTimer();

	UFUNCTION()
		FText UpdateScore();

	UFUNCTION()
		void OnExitButtonClicked();

protected:
	bool Initialize() override;
	class ADemoPlayer* player;
};
