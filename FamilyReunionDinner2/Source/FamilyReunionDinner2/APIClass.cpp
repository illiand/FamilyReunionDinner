// Fill out your copyright notice in the Description page of Project Settings.


#include "APIClass.h"

TArray<ARecipeCard*> UAPIClass::callXRC()
{
	TArray<ARecipeCard*> arr = TArray<ARecipeCard*>();

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
		if (FFileHelper::LoadFileToString(fileContent, *file, FFileHelper::EHashOptions::None))
		{
			return FStringToJson(fileContent);
		}
	}

	throw "File ERROR";
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