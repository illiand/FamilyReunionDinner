// Fill out your copyright notice in the Description page of Project Settings.


#include "APIClass.h"

TArray<FRecipeCardStruct> UAPIClass::makeRecipeCards() 
{
	FString dataString =
		"Name Type Flavor_Range Heat_Range Utensil_Size Points\nSpicy_Potatoes Spicy 1-2 3 2 2\nSpicy_Tofu_Bowl Spicy 2-3 2-3 2 2\nSpicy_Fries Spicy 3 1-2 2 3\nThai_Curry Spicy 3-4 3 3 3\nSpicy_Hot_Pot Spicy 4-5 3-4 5 7\n\nDonuts Sweet 2-3 1-2 1 2\n\nSandwich Salty 3 1-2 2 3\n\nSour_Fish Sour 3-4 2-3 3 5\n";
	TArray<TSharedPtr<FJsonObject>> data = FStringToJson(dataString);

	TArray<FRecipeCardStruct> arr;

	for (int i = 0; i < data.Num(); i += 1) 
	{
		FRecipeCardStruct curData;

		curData.name = data[i]->GetStringField(TEXT("Name"));
		curData.type = data[i]->GetStringField(TEXT("Type"));
		curData.flavorRange = data[i]->GetStringField(TEXT("Flavor_Range"));
		curData.heatRange = data[i]->GetStringField(TEXT("Heat_Range"));
		curData.size = data[i]->GetStringField(TEXT("Utensil_Size"));
		curData.point = data[i]->GetStringField(TEXT("Points"));

		arr.Add(curData);
	}

	return arr;
}

//TArray<TSharedPtr<FJsonObject>> UAPIClass::readFile(FString path)
//{
//	FString file = FPaths::ProjectConfigDir();
//	file.Append(path);
//
//	IPlatformFile& fileManager = FPlatformFileManager::Get().GetPlatformFile();
//	FString fileContent;
//
//	if (fileManager.FileExists(*file))
//	{
//		UE_LOG(LogTemp, Warning, TEXT("file exists"));
//
//		if (FFileHelper::LoadFileToString(fileContent, *file, FFileHelper::EHashOptions::None))
//		{
//			return FStringToJson(fileContent);
//		}
//	}
//	else 
//	{
//		UE_LOG(LogTemp, Warning, TEXT("read file error"));
//	}
//
//	return TArray<TSharedPtr<FJsonObject>>();
//}

TArray<TSharedPtr<FJsonObject>> UAPIClass::FStringToJson(FString data)
{
	TArray<TSharedPtr<FJsonObject>> jsonData;
	TArray<FString> lineData;
	data.ParseIntoArray(lineData, TEXT("\n"), true);

	TArray<FString> types;
	lineData[0].ParseIntoArray(types, TEXT(" "), true);

	for (int i = 1; i < lineData.Num(); i++)
	{
		TArray<FString> curLineString;
		lineData[i].ParseIntoArray(curLineString, TEXT(" "), true);

		TSharedPtr<FJsonObject> curData = MakeShareable(new FJsonObject());

		for (int j = 0; j < types.Num(); j++)
		{
			curLineString[j] = curLineString[j].Replace(TEXT("_"), TEXT(" "));

			curData->SetStringField(types[j], curLineString[j]);
		}
		
		jsonData.Add(curData);
	}

	return jsonData;
}

TArray<APlayerState*> UAPIClass::getPlayerControllersInGame(UWorld* world) 
{
	return Cast<AMyGameStateBase>(world->GetGameState())->PlayerArray;
}
