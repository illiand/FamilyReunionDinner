// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMenuPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Misc/OutputDeviceNull.h"
#include "GameMenuGameMode.h"
#include "GameMenuLevelScriptActor.h"

void AGameMenuPlayerState::BeginPlay() 
{
	Super::BeginPlay();

	if (GetLocalRole() != 3 && GetOwner() != NULL)
	{
		this->generateFamilyReunionDinner2UserID();
		this->requestServerInfo();

		this->generateGameHelper();
	}
}

void AGameMenuPlayerState::generateFamilyReunionDinner2UserID_Implementation()
{
	FamilyReunionDinner2PlayerID = FMath::RandRange(10000000, 99999999);
	setFamilyReunionDinner2UserID(FamilyReunionDinner2PlayerID);
}

void AGameMenuPlayerState::setFamilyReunionDinner2UserID_Implementation(int id)
{
	FamilyReunionDinner2PlayerID = id;
	call_createMainMenu();
}

void AGameMenuPlayerState::call_createMainMenu() 
{
	FOutputDeviceNull ar;

	if (GetLevel() != NULL)
	{
		AGameMenuLevelScriptActor* levelBluePrint = Cast<AGameMenuLevelScriptActor>(GetLevel()->GetLevelScriptActor());
		levelBluePrint->CallFunctionByNameWithArguments(TEXT("initMainMenu"), ar, NULL, true);
	}	
}

void AGameMenuPlayerState::updateServerInfoUpdate_Implementation(const TArray<FServerListInfoStruct>& data)
{
	for (int i = 0; i < data.Num(); i += 1) 
	{
		handleServerCreateEvent_Implementation(data[i]);
	}
}

void AGameMenuPlayerState::requestServerInfo_Implementation() 
{
	TArray<FServerListInfoStruct> data;

	for (int i = 0; i < Cast<AGameMenuGameMode>(GetWorld()->GetAuthGameMode())->serverInfo.Num(); i++)
	{
		data.Add(Cast<AGameMenuGameMode>(GetWorld()->GetAuthGameMode())->serverInfo[i].serverInfo);
	}

	sendServerInfo(data);
}

void AGameMenuPlayerState::sendServerInfo_Implementation(const TArray<FServerListInfoStruct>& data)
{
	for (int i = 0; i < data.Num(); i += 1)
	{
		handleServerCreateEvent_Implementation(data[i]);
	}
}

void AGameMenuPlayerState::updateRoomInfoUpdate_Implementation(const TArray<FRoomPlayerInfoStruct>& data)
{
	for (int i = 0; i < data.Num(); i += 1)
	{
		handlePlayerJoinRoomEvent_Implementation(data[i]);
	}
}

void AGameMenuPlayerState::handlePlayerJoinRoomEvent_Implementation(FRoomPlayerInfoStruct data)
{
	Cast<AGameMenuCharacter>(GetPawn())->GameMenuUI->addRoomPlayerUI(data.ID, data.name);
}

void AGameMenuPlayerState::handlePlayerLeaveRoomEvent_Implementation(int id)
{
	Cast<AGameMenuCharacter>(GetPawn())->GameMenuUI->removeRoomPlayerUI(id);
}

void AGameMenuPlayerState::handleRoomPlayerChangedEvent_Implementation(int curNum, int maxNum)
{
	Cast<AGameMenuCharacter>(GetPawn())->GameMenuUI->roomPlayerChangedUI(curNum, maxNum);
}

void AGameMenuPlayerState::handleServerListRoomPlayerChangedEvent_Implementation(int id, int curNum, int maxNum)
{
	Cast<AGameMenuCharacter>(GetPawn())->GameMenuUI->serverListRoomPlayerChangedUI(id, curNum, maxNum);
}

void AGameMenuPlayerState::handleServerCreateEvent_Implementation(FServerListInfoStruct data)
{
	Cast<AGameMenuCharacter>(GetPawn())->GameMenuUI->addServerListUI(data.ID, data.name, data.curPlayerNum, data.maxPlayerNum);
}

void AGameMenuPlayerState::handleServerDeleteEvent_Implementation(int id)
{
	Cast<AGameMenuCharacter>(GetPawn())->GameMenuUI->removeServerListUI(id);
}

void AGameMenuPlayerState::setTransformingStatus_Implementation(bool isIn)
{
	inEnteringTransforming = true;
}

void AGameMenuPlayerState::sendErrorMessage_Implementation(const FString& text, const FVector& color)
{
	Cast<AGameMenuCharacter>(GetPawn())->GameMenuUI->showWorldMessage(text, color);
}

void AGameMenuPlayerState::generateGameHelper()
{
	FGameHelperStruct cur0;
	cur0.title = "Game Goal";
	cur0.path = "GameGoal";
	cur0.text = "* To win the game, the players should finish Nian's requirement\n";

	FGameHelperStruct cur1;
	cur1.title = "Time Limit";
	cur1.path = "TimeLimit";
	cur1.text = "* The players need to finish all Nian's requirement in a specific time period\n* The total turn is determined by the player number\n* Also, each player's turn has its time limit\n";

	FGameHelperStruct cur2;
	cur2.title = "Pot / Recipe Card";
	cur2.path = "RecipeCard";
	cur2.text = "* To finish the monster requirement, the players should create recipes\n* To finish recipes, the player need to add cooking card and ingredient card into it\n* By clicking the pot/recipe, the player can know current details for this recipe\n";

	FGameHelperStruct cur3;
	cur3.title = "Ingredient Card";
	cur3.path = "Ingredient";
	cur3.text = "* Ingredient card is used to increase Point of the pot\n* Each Ingredient has its size limit and can't over the pot limit\n* The player's turn will END by adding a ingredient card to pot";

	FGameHelperStruct cur4;
	cur4.title = "Cooking Card";
	cur4.path = "Cooking";
	cur4.text = "* Cooking card is used to add flavor or heat of the pot\n* If the flavor or heat range is not in the range, Point of the pot won't count\n* The current flavor and heat can be seen in the pot's bar or panel\n* The player's turn will END by adding a cooking card to pot";

	FGameHelperStruct cur5;
	cur5.title = "Invisible Status";
	cur5.path = "InvisibleStatus";
	cur5.text = "* The player can't see the cooking cards that own by self";

	FGameHelperStruct cur6;
	cur6.title = "Give Others A Hint";
	cur6.path = "GiveOthersAHint";
	cur6.text = "* Player can choose to tell other players information about their cards";

	FGameHelperStruct cur7;
	cur7.title = "The Type of the Hint";
	cur7.path = "TheKindoftheHint";
	cur7.text = "* Hint have two types, type hint and degree hint\n* The hint will mark ALL cards that have the same information as the card that the player is choosing\n* The player's turn will END after this action\n";

	FGameHelperStruct cur8;
	cur8.title = "Added Item In Pot";
	cur8.path = "AddedItemInRecipe";
	cur8.text = "* The player can see added items by clicking the pot\n* The green number shows the current point for this pot\n";

	FGameHelperStruct cur9;
	cur9.title = "Finish Dish";
	cur9.path = "FinishDish";
	cur9.text = "* The player can choose to finish a pot* This action will use 1 action point (Max: 3) of the player\n* By completing finishing a pot will recover that action point\n* The player's turn will END after this action\n";

	FGameHelperStruct cur10;
	cur10.title = "Remove A Item In Recipe";
	cur10.path = "RemoveAItemInRecipe";
	cur10.text = "* The player can choose to remove an item that has already added in a pot\n* This action will use 1 action point (Max: 3) of the player\n* The player's turn will END after this action\n";

	FGameHelperStruct cur11;
	cur11.title = "Encounter an Action";
	cur11.path = "EncounterAnAction";
	cur11.text = "* If the other player is trying to finish the dish or remove a item, the player can use 1 action point (Max: 3) to reject this action\n* Every player can only encounter an action for a specific item at once\n* This encounter process can happen until no one is able to encounter the action\n";

	FGameHelperStruct cur12;
	cur12.title = "End Turn";
	cur12.path = "EndTurn";
	cur12.text = "* The player can choose to finish the turn with no action\n";

	FGameHelperStruct cur13;
	cur13.title = "Game Panel";
	cur13.path = "GamePanel";
	cur13.text = "* The player can see the monster preference and finished dish or choosing to end game\n* The ending game option is only processable when the monster's requirement fulfilled\n";

	FGameHelperStruct cur14;
	cur14.title = "Monster Preference";
	cur14.path = "MonsterPreference";
	cur14.text = "* Each player can only see 1 monster preference\n* If any monster preference was not fulfilled at the ending of the game, the players will lose the game\n* Note: Ending game option will automatically ignore the monster preference\n";

	gameHelpers.Add(cur0);
	gameHelpers.Add(cur1);
	gameHelpers.Add(cur2);
	gameHelpers.Add(cur3);
	gameHelpers.Add(cur4);
	gameHelpers.Add(cur5);
	gameHelpers.Add(cur6);
	gameHelpers.Add(cur7);
	gameHelpers.Add(cur8);
	gameHelpers.Add(cur9);
	gameHelpers.Add(cur10);
	gameHelpers.Add(cur11);
	gameHelpers.Add(cur12);
	gameHelpers.Add(cur13);
	gameHelpers.Add(cur14);
}