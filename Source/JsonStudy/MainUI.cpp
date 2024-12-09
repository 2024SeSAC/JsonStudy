// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUI.h"
#include "DownloadImageUI.h"
#include "Components/CanvasPanel.h"
#include "Components/ScrollBox.h"

void UMainUI::AddDownloadImage(FString url)
{
	UDownloadImageUI* image = CreateWidget<UDownloadImageUI>(GetWorld(), downloadImageUIFactory);
	image->SetUrl(url);
	imageList->AddChild(image);
}
