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
TArray<FIngredientCardStruct> UAPIClass::makeIngredientCards()
{
	FString dataString =
		"Name Type Size Points\nChicken M 2 3\nBeef M 3 4\nMutton M 3 Get_2_extra_points_when_cooked_with_seafoods\nTomatoes V 1 1\nPotatoes V 2 2\nCabbage V 2 Get_2_extra_points_when_cooked_with_meat\nShrimps S 3 4\nSeafish S 2 Get_4_points_when_cooked_with_meat\nLobster S 3 The_dish_must_be_heated_over_4_or_the_bonus_point_is_zero";
	TArray<TSharedPtr<FJsonObject>> data = FStringToJson(dataString);

	TArray<FIngredientCardStruct> arr;

	for (int i = 0; i < data.Num(); i += 1)
	{
		FIngredientCardStruct curData;

		curData.name = data[i]->GetStringField(TEXT("Name"));
		curData.type = data[i]->GetStringField(TEXT("Type"));
		curData.size = data[i]->GetStringField(TEXT("Size"));
		curData.point = data[i]->GetStringField(TEXT("Points"));

		arr.Add(curData);
	}
	return arr;
}
TArray<FCookingCardStruct> UAPIClass::makeCookingCards()
{
	FString dataString =
		"Name Type Degree\nSpicy+1 Spicy 1\nSpicy+2 Spicy 2\nSpicy+3 Spicy 3\nSalty+1 Salty 1\nSalty+2 Salty 2\nSalty+3 Salty 3\nSour+1 Sour 1\nSour+2 Sour 2\nSour+3 Sour 3\nSweet+1 Sweet 1\nSweet+2 Sweet 2\nSweet+3 Sweet 3\nheat+1 heat 1\nheat+2 heat 2\nheat+3 heat 3";
	TArray<TSharedPtr<FJsonObject>> data = FStringToJson(dataString);

	TArray<FCookingCardStruct> arr;

	for (int i = 0; i < data.Num(); i += 1)
	{
		FCookingCardStruct curData;

		curData.name = data[i]->GetStringField(TEXT("Name"));
		curData.type = data[i]->GetStringField(TEXT("Type"));
		curData.degree = data[i]->GetStringField(TEXT("Degree"));

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

bool UAPIClass::checkServerListEqual(TArray<FServerInfoStruct> serverInfo1, TArray<FServerInfoStruct> serverInfo2)
{
	if (serverInfo1.Num() != serverInfo2.Num()) 
	{
		return false;
	}

	for (int i = 0; i < serverInfo1.Num(); i += 1)
	{
		if (serverInfo1[i].currentPlayers.Num() != serverInfo2[i].currentPlayers.Num())
		{
			return false;
		}
	}

	return true;
}


FString UAPIClass::getIPAddress() 
{
	return TEXT("127.0.0.1");
}