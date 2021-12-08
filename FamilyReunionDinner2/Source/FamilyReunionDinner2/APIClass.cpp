// Fill out your copyright notice in the Description page of Project Settings.


#include "APIClass.h"
#include "MyGameStateBase.h"
#include "FamilyReunionDinner2GameMode.h"
#include "MyPlayerState.h"

TArray<FRecipeCardStruct> UAPIClass::makeRecipeCards() 
{
	FString dataString = "Name Type Flavor_Range Heat_Range Utensil_Size Points\nSpicy_Potatoes Spicy 1-2 3 2 2\nSpicy_Tofu_Bowl Spicy 2-3 2-3 2 2\nSpicy_Fries Spicy 3 1-2 2 3\nThai_Curry Spicy/Sour 3-4 3 3 3\nSpicy_Hot_Pot Spicy 4-5 3-4 5 4\nFruit_Bowl Sweet 1-2 0 2 2\nDonuts Sweet 2-3 1-2 1 2\nYogurt_Bowl Sweet 2-3 2-3 2 3\nCreamy_Mac Sweet 3-4 3-5 4 5\nCheese_Pizza Sweet/Salty 4-5 5 6 4\nEight_TreasureCongee Sweet 2-3 3-4 4 3\nMiso_Soup Salty 1-2 2-3 5 0\nTacco Salty 2-3 2 2 3\nSandwich Salty 3 1-2 2 3\nBorscht Salty/Spicy 2-4 3-5 3 3\nStew Salty 3-4 5 5 4\nSourdough_Bread Sour 1-2 2-3 1 2\nSalad Sour/Sweet 2-4 1 4 1\nSour_Fish Sour 3-4 2-3 2 4\nHot_Sour_Noodels Sour 4 3-4 3 4\nKimiji_Fried_Rice Sour 4-5 4 4 4\nDolma Sour/Spicy 3-4 3-4 3 4\n";
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
		curData.path = TEXT("Texture2D'/Game/Assets/Texture/uv.uv'");

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
		curData.path = TEXT("Texture2D'/Game/Assets/Texture/uv.uv'");

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
		curData.path = TEXT("Texture2D'/Game/Assets/Texture/uv.uv'");
		curData.typeHinted = false;
		curData.degreeHinted = false;

		arr.Add(curData);
	}
	return arr;
}

TArray<FMonsterPreferenceStruct> UAPIClass::makeMonsterPreference()
{
	FString dataString = "Card_Name Card_Information\nVegetarian No_meat\nSeafood_Allergy No_seafood\nÐÁµ³[¤«¤é¤È¤¦] At_least_finish_1_spicy_dish\nLemon_Squeezer At_least_finish_1_sour_dish\n¸Êµ³[¤¢¤Þ¤È¤¦] At_least_finish_1_sweet_dish\nSupertaster At_least_finish_1_salty_dish\nMouth_Ulcer If_the_dish_is_spicy or sour,_reduce_the_degree_of_flavour_with_1.\nSpitfire_Mouth All dishes¡¯ heat requirement increases by 1 degree.\nRaw_is_healthy All dishes¡¯ heat requirement decreases by 1 degree.\nGo_on_a_diet If_the_dish_is_sweet or salty_reduce_the_degree_of_flavour_with_1.\n";
	TArray<TSharedPtr<FJsonObject>> data = FStringToJson(dataString);

	TArray<FMonsterPreferenceStruct> arr;

	for (int i = 0; i < data.Num(); i += 1)
	{
		FMonsterPreferenceStruct curData;

		curData.name = data[i]->GetStringField(TEXT("Card_Name"));
		curData.effect = data[i]->GetStringField(TEXT("Card_Information"));
		curData.path = TEXT("Texture2D'/Game/Assets/Texture/uv.uv'");
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