﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "JsonPawn.generated.h"

USTRUCT()
struct FUserInfo
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FString name;
	UPROPERTY()
	int32 age;
	UPROPERTY()
	float height;
	UPROPERTY()
	float weight;
	UPROPERTY()
	bool gender; // true : 여성, false : 남성
	UPROPERTY()
	TArray<FString> favoriteFood;
};

USTRUCT()
struct FUserInfoArray
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TArray<FUserInfo> data;
};


UCLASS()
class JSONSTUDY_API AJsonPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AJsonPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void DataToJsonExample();
};
