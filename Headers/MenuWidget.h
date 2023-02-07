// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.generated.h"

/**
 *
 */
UCLASS()
class REDBITPROJ_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	void NativeConstruct() override;

	UFUNCTION()
		void OnStartButtonClicked();

	UFUNCTION()
		void OnExitButtonClicked();

	UPROPERTY(meta = (BindWidget))
		class UButton* StartButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* ExitButton;

};
