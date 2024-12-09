// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DownloadImageUI.generated.h"

/**
 * 
 */
UCLASS()
class JSONSTUDY_API UDownloadImageUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta=(BindWidget))
	class UImage* downloadImage;

	void SetUrl(FString url);

	UFUNCTION()
	void OnDownloadComplete(class UTexture2DDynamic* Texture);
};
