// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "JsonPawn.generated.h"

USTRUCT()
struct FUserInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere)
	FString name;
	UPROPERTY(VisibleAnywhere)
	int32 age;
	UPROPERTY(VisibleAnywhere)
	float height;
	UPROPERTY(VisibleAnywhere)
	float weight;
	UPROPERTY(VisibleAnywhere)
	bool gender; // true : 여성, false : 남성
	UPROPERTY(VisibleAnywhere)
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

USTRUCT()
struct FShapeInfo
{
	GENERATED_BODY()
public:
	UPROPERTY()

	FVector pos;
	UPROPERTY()

	FRotator rot;
	UPROPERTY()

	FVector scale;
	UPROPERTY()

	int32 type;
};

USTRUCT()
struct FShapeInfoArray
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TArray<FShapeInfo> data;
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

	UFUNCTION(BlueprintCallable)
	void JsonToDataExample();

	UPROPERTY(VisibleAnywhere)
	FUserInfo userInfo;
	
	UPROPERTY(VisibleAnywhere)
	FUserInfo userInfo2;

public:
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<class AShapeActor>> shapeFactory;
	UPROPERTY()
	TArray<class AShapeActor*> allShape;

	UFUNCTION(BlueprintCallable)
	void CreateShape();
	UFUNCTION(BlueprintCallable)
	void SaveData();
	UFUNCTION(BlueprintCallable)
	void LoadData();


	FVector GetRandLocation();
	FRotator GetRandRotation();
};
