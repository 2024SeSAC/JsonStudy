// Fill out your copyright notice in the Description page of Project Settings.


#include "JsonPawn.h"
#include "JsonUtilities.h"
#include "ShapeActor.h"
#include "Http.h"
#include "MainUI.h"

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

	mainUI = CreateWidget<UMainUI>(GetWorld(), mainUIFactory);
	mainUI->AddToViewport();
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

	for (int32 i = 0; i < allUser.Num(); i++)
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

void AJsonPawn::CreateShape()
{
	FVector pos = GetRandLocation();
	FRotator rot = GetRandRotation();
	FVector scale = FVector(FMath::RandRange(0.5f, 3.0f));
	int32 randType = FMath::RandRange(0, 2);

	CreateShape(randType, pos, rot, scale);
}

void AJsonPawn::CreateShape(int32 type, FVector pos, FRotator rot, FVector scale)
{
	AShapeActor* shape = GetWorld()->SpawnActor<AShapeActor>(shapeFactory[type]);
	shape->SetActorLocation(pos);
	shape->SetActorRotation(rot);
	shape->SetActorScale3D(scale);
	shape->type = type;

	allShape.Add(shape);
}

void AJsonPawn::SaveData()
{
	// allShape 의 있는 모양의
	// 위치, 회전, 스케일, type 을 Json 형태로 바꾸자.

	/*TArray<FShapeInfo> array;
	for (int32 i = 0; i < allShape.Num(); i++)
	{
		FShapeInfo shape;
		shape.pos = allShape[i]->GetActorLocation();
		shape.rot = allShape[i]->GetActorRotation();
		shape.scale = allShape[i]->GetActorScale3D();
		shape.type = allShape[i]->type;
		array.Add(shape);
	}

	FShapeInfoArray info;
	info.data = array;
	FString jsonString;
	FJsonObjectConverter::UStructToJsonObjectString(
		FShapeInfoArray::StaticStruct(),
		&info,
		jsonString
	);*/

	TArray<TSharedPtr<FJsonValue>> jsonArray;
	for (int32 i = 0; i < allShape.Num(); i++)
	{
		TSharedPtr<FJsonObject> jsonObject = MakeShared<FJsonObject>();
		// 위치
		jsonObject->SetNumberField(TEXT("pos_x"), allShape[i]->GetActorLocation().X);
		jsonObject->SetNumberField(TEXT("pos_y"), allShape[i]->GetActorLocation().Y);
		jsonObject->SetNumberField(TEXT("pos_z"), allShape[i]->GetActorLocation().Z);
		// 회전
		jsonObject->SetNumberField(TEXT("rot_pitch"), allShape[i]->GetActorRotation().Pitch);
		jsonObject->SetNumberField(TEXT("rot_yaw"), allShape[i]->GetActorRotation().Yaw);
		jsonObject->SetNumberField(TEXT("rot_roll"), allShape[i]->GetActorRotation().Roll);
		// 크기
		jsonObject->SetNumberField(TEXT("scale"), allShape[i]->GetActorScale3D().X);
		// 모양
		jsonObject->SetNumberField(TEXT("type"), allShape[i]->type);

		TSharedPtr<FJsonValueObject> valueObj = MakeShared<FJsonValueObject>(jsonObject);
		jsonArray.Add(valueObj);
	}

	// jsonArray 를 FString 변환
	FString jsonString;
	TSharedRef<TJsonWriter<>> jsonWriter = TJsonWriterFactory<>::Create(&jsonString);
	FJsonSerializer::Serialize(jsonArray, jsonWriter);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *jsonString);

	// jsonString 을 text file 로 저장하자
	FString path = FPaths::ProjectDir() + TEXT("text.txt");
	FFileHelper::SaveStringToFile(jsonString, *path);
}

void AJsonPawn::LoadData()
{
	FString path = FPaths::ProjectDir() + TEXT("text.txt");
	FString jsonString;
	FFileHelper::LoadFileToString(jsonString, *path);

	/*FShapeInfoArray info;
	FJsonObjectConverter::JsonObjectStringToUStruct(jsonString, &info);

	for (int32 i = 0; i < info.data.Num(); i++)
	{
		FShapeInfo shape = info.data[i];
		CreateShape(shape.type, shape.pos, shape.rot, shape.scale);
	}*/

	TSharedRef<TJsonReader<>> jsonReader = TJsonReaderFactory<>::Create(jsonString);
	TArray<TSharedPtr<FJsonValue>> jsonArray;
	FJsonSerializer::Deserialize(jsonReader, jsonArray);

	for (int32 i = 0; i < jsonArray.Num(); i++)
	{
		TSharedPtr<FJsonObject> jsonObject = jsonArray[i]->AsObject();

		FVector pos;
		pos.X = jsonObject->GetNumberField(TEXT("pos_x"));
		pos.Y = jsonObject->GetNumberField(TEXT("pos_y"));
		pos.Z = jsonObject->GetNumberField(TEXT("pos_z"));

		FRotator rot;
		rot.Pitch = jsonObject->GetNumberField(TEXT("rot_pitch"));
		rot.Yaw = jsonObject->GetNumberField(TEXT("rot_yaw"));
		rot.Roll = jsonObject->GetNumberField(TEXT("rot_roll"));

		float scaleValue = jsonObject->GetNumberField(TEXT("scale"));
		FVector scale = FVector(scaleValue);

		int32 type = jsonObject->GetNumberField(TEXT("type"));

		CreateShape(type, pos, rot, scale);
	}
}

FVector AJsonPawn::GetRandLocation()
{
	FVector pos;
	pos.X = FMath::RandRange(-500.0f, 500.0f);
	pos.Y = FMath::RandRange(-500.0f, 500.0f);
	pos.Z = FMath::RandRange(-500.0f, 500.0f);
	return pos;
}

FRotator AJsonPawn::GetRandRotation()
{
	FRotator rot;
	rot.Pitch = FMath::RandRange(0.0f, 360.0f);
	rot.Yaw = FMath::RandRange(0.0f, 360.0f);
	rot.Roll = FMath::RandRange(0.0f, 360.0f);
	return rot;
}

void AJsonPawn::HttpRequestGet()
{
	// 서버에게 요청하는 객체 만들자.
	FHttpRequestRef httpRequest = FHttpModule::Get().CreateRequest();
	// 요청 URL - 서버가 만들어서 알려줌.
	httpRequest->SetURL(TEXT("https://jsonplaceholder.typicode.com/photos"));
	// 요청 방식 설정 - 서버에 GET, POST, PUT, DELETE 어떤것으로 해야하는지 알려줌
	httpRequest->SetVerb(TEXT("GET"));
	// 헤더를 설정 
	httpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	// 서버에서 응답이 오면 호출되는 함수 등록
	httpRequest->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully) {
		// 응답 오는 곳
		if (bConnectedSuccessfully)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s"), *Response->GetContentAsString());
			FString jsonString = Response->GetContentAsString();
			TSharedRef<TJsonReader<>> jsonReader = TJsonReaderFactory<>::Create(jsonString);
			TArray<TSharedPtr<FJsonValue>> jsonArray;
			FJsonSerializer::Deserialize(jsonReader, jsonArray);

			for (int32 i = 0; i < 100; i++)
			{
				TSharedPtr<FJsonObject> jsonObject = jsonArray[i]->AsObject();
				mainUI->AddDownloadImage(jsonObject->GetStringField(TEXT("url")));
			}



			//jsonString = FString::Printf(TEXT("{\"data\":%s}"), *jsonString);
			/*FPostInfoArray info;
			FJsonObjectConverter::JsonObjectStringToUStruct(jsonString, &info);
			allPost = info.data;*/
		}
		else
		{
			//Response->GetResponseCode() : 200 정상,    400번대, 500번대 오류
			UE_LOG(LogTemp, Warning, TEXT("통신 실패 : %d"), Response->GetResponseCode());
		}
		}
	);

	// 요청을 보내자.
	httpRequest->ProcessRequest();
}

void AJsonPawn::HttpRequestPost()
{
	// 서버에게 요청하는 객체 만들자.
	FHttpRequestRef httpRequest = FHttpModule::Get().CreateRequest();
	// 요청 URL - 서버가 만들어서 알려줌.
	httpRequest->SetURL(TEXT("https://jsonplaceholder.typicode.com/posts"));
	// 요청 방식 설정 - 서버에 GET, POST, PUT, DELETE 어떤것으로 해야하는지 알려줌
	httpRequest->SetVerb(TEXT("POST"));
	// 헤더를 설정 
	httpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	// 서버에게 보내고 싶은 데이값 (Json)
	httpRequest->SetContentAsString(TEXT("Json 형태의 문자열을 만들어서 셋팅"));

	// 서버에서 응답이 오면 호출되는 함수 등록
	httpRequest->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully) {
			// 응답 오는 곳
			if (bConnectedSuccessfully)
			{
				UE_LOG(LogTemp, Warning, TEXT("%s"), *Response->GetContentAsString());
			
			}
			else
			{
				//Response->GetResponseCode() : 200 정상,    400번대, 500번대 오류
				UE_LOG(LogTemp, Warning, TEXT("통신 실패 : %d"), Response->GetResponseCode());
			}
		}
	);

	// 요청을 보내자.
	httpRequest->ProcessRequest();
}

void AJsonPawn::HttpRequestImageDownload()
{
	// 서버에게 요청하는 객체 만들자.
	FHttpRequestRef httpRequest = FHttpModule::Get().CreateRequest();
	// 요청 URL - 서버가 만들어서 알려줌.
	httpRequest->SetURL(TEXT("https://ssl.pstatic.net/melona/libs/1482/1482857/2e170a50469c9f109ad3_20241202173623747.jpg"));
	// 요청 방식 설정 - 서버에 GET, POST, PUT, DELETE 어떤것으로 해야하는지 알려줌
	httpRequest->SetVerb(TEXT("GET"));
	// 서버에서 응답이 오면 호출되는 함수 등록
	httpRequest->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully) {
			// 응답 오는 곳
			if (bConnectedSuccessfully)
			{
				// jsonString 을 text file 로 저장하자
				FString path = FPaths::ProjectDir() + TEXT("image.jpg");
				TArray<uint8> array = Response->GetContent();
				FFileHelper::SaveArrayToFile(array, *path);
			}
			else
			{
				//Response->GetResponseCode() : 200 정상,    400번대, 500번대 오류
				UE_LOG(LogTemp, Warning, TEXT("통신 실패 : %d"), Response->GetResponseCode());
			}
		}
	);

	// 요청을 보내자.
	httpRequest->ProcessRequest();
}

void AJsonPawn::DownloadImage()
{
	FString url = TEXT("https://ssl.pstatic.net/melona/libs/1482/1482857/2e170a50469c9f109ad3_20241202173623747.jpg");
	mainUI->AddDownloadImage(url);
}

