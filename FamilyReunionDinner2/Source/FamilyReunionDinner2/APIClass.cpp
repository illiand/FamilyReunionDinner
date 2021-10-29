// Fill out your copyright notice in the Description page of Project Settings.


#include "APIClass.h"

TArray<FRecipeCardStruct> UAPIClass::makeRecipeCards(TArray<TSharedPtr<FJsonObject>> data) 
{
	TArray<FRecipeCardStruct> arr;

	for (int i = 0; i < data.Num(); i += 1) 
	{
		FRecipeCardStruct curData;

		curData.name = data[i]->GetStringField(TEXT("Name"));
		curData.type = data[i]->GetStringField(TEXT("Type"));
		curData.flavorRange = data[i]->GetStringField(TEXT("Flavor_Range"));
		curData.heatRange = data[i]->GetStringField(TEXT("Heat_Range"));
		curData.size = data[i]->GetStringField(TEXT("Utensil_Size"));
		curData.point = data[i]->GetStringField(TEXT("Points "));

		arr.Add(curData);
	}

	return arr;
}

TArray<TSharedPtr<FJsonObject>> UAPIClass::readFile(FString path)
{
	FString file = FPaths::ProjectConfigDir();
	file.Append(path);

	IPlatformFile& fileManager = FPlatformFileManager::Get().GetPlatformFile();
	FString fileContent;

	if (fileManager.FileExists(*file))
	{
		UE_LOG(LogTemp, Warning, TEXT("file exists"));

		if (FFileHelper::LoadFileToString(fileContent, *file, FFileHelper::EHashOptions::None))
		{
			return FStringToJson(fileContent);
		}
	}
	else 
	{
		UE_LOG(LogTemp, Warning, TEXT("read file error"));
	}

	return TArray<TSharedPtr<FJsonObject>>();
}

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