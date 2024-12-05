// Fill out your copyright notice in the Description page of Project Settings.


#include "JsonPawn.h"
#include "JsonUtilities.h"

// Sets default values
AJsonPawn::AJsonPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AJsonPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJsonPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AJsonPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AJsonPawn::DataToJsonExample()
{
	TArray<FUserInfo> allUser;

	// 가상데이터
	for (int32 i = 0; i < 10; i++)
	{
		FUserInfo info;
		info.name = FString::Printf(TEXT("SeSAC_%d"), i);
		info.age = 3 + i;
		info.height = 170.5f + i * 5;
		info.weight = 77.7f + i * 2;
		info.gender = (bool)(i % 2);
		info.favoriteFood = { TEXT("김치찌개"), TEXT("스팸"), TEXT("삼겹살") };

		allUser.Add(info);
	}
	

	// FJsonObjectConverter 이용해서 Json 만들자.
	FString jsonString2;
	FUserInfoArray jsonArray2;
	jsonArray2.data = allUser;
	FJsonObjectConverter::UStructToJsonObjectString<FUserInfoArray>(jsonArray2, jsonString2);
	UE_LOG(LogTemp, Warning, TEXT("JsonString2 : %s"), *jsonString2);


	// Data ---> JsonObject ---> FString (json 형태)
	
	TArray<TSharedPtr<FJsonValue>> jsonUserInfoArray;

	for(int32 i = 0; i < allUser.Num(); i++)
	{
		FUserInfo info = allUser[i];
		// JsonObject 만들자.
		TSharedPtr<FJsonObject> jsonObject = MakeShared<FJsonObject>();
		jsonObject->SetStringField(TEXT("name"), info.name);
		jsonObject->SetNumberField(TEXT("age"), info.age);
		jsonObject->SetNumberField(TEXT("height"), info.height);
		jsonObject->SetNumberField(TEXT("weight"), info.weight);
		jsonObject->SetBoolField(TEXT("gender"), info.gender);

		// TArray ---> jsonArray
		TArray<TSharedPtr<FJsonValue>> jsonArray;
		for (int32 j = 0; j < info.favoriteFood.Num(); j++)
		{
			TSharedPtr<FJsonValue> value = MakeShared<FJsonValueString>(info.favoriteFood[j]);
			jsonArray.Add(value);
		}
	
		jsonObject->SetArrayField(TEXT("favoriteFood"), jsonArray);

		// jsonObject 을 jsonUserInfoArray 추가
		TSharedPtr<FJsonValue> v = MakeShared<FJsonValueObject>(jsonObject);
		jsonUserInfoArray.Add(v);
	}


	// jsonObject ----> FString 으로 
	FString jsonString;

	TSharedRef<TJsonWriter<>> jsonWriter = TJsonWriterFactory<>::Create(&jsonString);
	FJsonSerializer::Serialize(jsonUserInfoArray, jsonWriter);

	UE_LOG(LogTemp, Warning, TEXT("JsonString : %s"), *jsonString);
}

void AJsonPawn::JsonToDataExample()
{
	FString jsonString = TEXT("{\"name\":\"SeSAC_9\",\"age\":12,\"height\":215.5,\"weight\":95.699996948242188,\"gender\":true,\"favoriteFood\":[\"김치찌개\",\"스팸\",\"삼겹살\"]}");

	// FJsonObjectConverter 이용해서 Json 을 FUserInfo 변수에 셋팅
	FUserInfo info;
	FJsonObjectConverter::JsonObjectStringToUStruct(jsonString, &userInfo);
	
	
	// FString (json 형태) ---> JsonObject ---> Data 
	TSharedRef<TJsonReader<>> jsonReader = TJsonReaderFactory<>::Create(jsonString);
	TSharedPtr<FJsonObject> jsonObject;
	FJsonSerializer::Deserialize(jsonReader, jsonObject);

	userInfo2.name = jsonObject->GetStringField(TEXT("name"));
	userInfo2.age = jsonObject->GetNumberField(TEXT("age"));
	userInfo2.height = jsonObject->GetNumberField(TEXT("height"));
	userInfo2.weight = jsonObject->GetNumberField(TEXT("weight"));
	userInfo2.gender = jsonObject->GetBoolField(TEXT("gender"));

	TArray<TSharedPtr<FJsonValue>> jsonArray = jsonObject->GetArrayField(TEXT("favoriteFood"));
	for (int32 i = 0; i < jsonArray.Num(); i++)
	{
		userInfo2.favoriteFood.Add(jsonArray[i]->AsString());
	}
}

