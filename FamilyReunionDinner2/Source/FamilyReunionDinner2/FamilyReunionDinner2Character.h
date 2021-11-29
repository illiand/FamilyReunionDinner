#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RecipeCard.h"
#include "FamilyReunionDinner2UserWidget.h"
#include "FamilyReunionDinner2Character.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UMotionControllerComponent;
class UAnimMontage;
class USoundBase;

UCLASS(config = Game)
class AFamilyReunionDinner2Character : public ACharacter
{
	GENERATED_BODY()

		/** First person camera */
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* FirstPersonCameraComponent;

public:
	AFamilyReunionDinner2Character();

protected:
	virtual void BeginPlay();
	virtual void Tick(float DeltaTime) override;

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

protected:
	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
	void setLocationByIndex(int index);

public:
	UFUNCTION(BlueprintCallable)
	void useSpecialAction();

	UFUNCTION(Reliable, Server)
	void startGame();
	void startGame_Implementation();

	UFUNCTION(Reliable, Server, BlueprintCallable)
	void endTurn();
	void endTurn_Implementation();

	UFUNCTION(Reliable, Server)
	void moveToDeck(AActor* hitActor);
	void moveToDeck_Implementation(AActor* hitActor);

	UFUNCTION(Reliable, Server)
	void addCookingCardToPot(ACookingCard* card, int index);
	void addCookingCardToPot_Implementation(ACookingCard* card, int index);

	UFUNCTION(Reliable, Server)
	void addIngredientCardToPot(int ingredientCardIndex, int potIndex);
	void addIngredientCardToPot_Implementation(int ingredientCardIndex, int potIndex);

	UFUNCTION(Reliable, Server, BlueprintCallable)
	void finishRecipeCardRequest(int index);
	void finishRecipeCardRequest_Implementation(int index);

	UFUNCTION(Reliable, Server, BlueprintCallable)
	void removePotItemRequest(int potIndex, int index);
	void removePotItemRequest_Implementation(int potIndex, int index);

	UFUNCTION(Reliable, NetMulticast, BlueprintCallable)
	void setObservingPotItemIndex(int index);
	void setObservingPotItemIndex_Implementation(int index);

	UFUNCTION(Reliable, Server)
	void requestCertainHandInfo(ACookingCard* card);
	void requestCertainHandInfo_Implementation(ACookingCard* card);

	UFUNCTION(Reliable, Client)
	void sendCertainHandInfo(const TArray<ACookingCard*>& cards, int focusIndex);
	void sendCertainHandInfo_Implementation(const TArray<ACookingCard*>& cards, int focusIndex);

	UFUNCTION(Reliable, Server, BlueprintCallable)
	void giveTypeHint(ACookingCard* card);
	void giveTypeHint_Implementation(ACookingCard* card);

	UFUNCTION(Reliable, Server, BlueprintCallable)
	void giveDegreeHint(ACookingCard* card);
	void giveDegreeHint_Implementation(ACookingCard* card);

	UFUNCTION(Reliable, Client)
	void setHintShowed(bool showed);
	void setHintShowed_Implementation(bool showed);

	UFUNCTION(Reliable, Client)
	void sendReactionRequest(int potIndex, const FString& actionPath, const FString& actionDes, FVector actionColor);
	void sendReactionRequest_Implementation(int potIndex, const FString& actionPath, const FString& actionDes, FVector actionColor);

	UFUNCTION(Reliable, Server, BlueprintCallable)
	void replyRecipeFinishAction();
	void replyRecipeFinishAction_Implementation();

	UFUNCTION(Reliable, Server, BlueprintCallable)
	void replyRemovePotItemAction();
	void replyRemovePotItemAction_Implementation();

	UFUNCTION(Reliable, Server, BlueprintCallable)
	void giveUpReactionInRecipeFinish();
	void giveUpReactionInRecipeFinish_Implementation();

	UFUNCTION(Reliable, Server, BlueprintCallable)
	void giveUpInRemovingItem();
	void giveUpInRemovingItem_Implementation();

	UFUNCTION(Reliable, Client)
	void setWaitingTextUI(const FString& text);
	void setWaitingTextUI_Implementation(const FString& text);

	UFUNCTION(Reliable, Client)
	void clearUI();
	void clearUI_Implementation();

public:
	UPROPERTY(BlueprintReadWrite)
	UFamilyReunionDinner2UserWidget* MainUI;

	UPROPERTY(BlueprintReadOnly)
	TArray<ACookingCard*> observingCards;
	UPROPERTY(BlueprintReadOnly)
	int observingCardIndex = 0;

	UPROPERTY(BlueprintReadOnly)
	int observingPotIndex = 0;

	UPROPERTY(BlueprintReadWrite)
	int observingItemInPotIndex = -1;

private:
	AActor* holdingItem;
	bool UIOn = false;

private:
	AActor* pickFromEye();
	void releaseItem();
	void pickingItem();
	void puttingItem();

	UFUNCTION(BlueprintCallable)
	void showFlavorHintPreview(int index);
	UFUNCTION(BlueprintCallable)
	void showDegreeHintPreview(int index);
};