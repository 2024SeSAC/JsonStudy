// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainUI.generated.h"

/**
 * 
 */
UCLASS()
class JSONSTUDY_API UMainUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta=(BindWidget))
	class UCanvasPanel* mainCanvas;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UDownloadImageUI> downloadImageUIFactory;

	void AddDownloadImage(FString url);

};
