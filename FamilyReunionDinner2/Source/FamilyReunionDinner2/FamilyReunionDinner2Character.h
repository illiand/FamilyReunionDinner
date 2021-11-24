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

	UFUNCTION(Reliable, Server)
	void finishRecipeCard(int index);
	void finishRecipeCard_Implementation(int index);

public:
	UPROPERTY(BlueprintReadWrite)
	UFamilyReunionDinner2UserWidget* MainUI;

private:
	AActor* holdingItem;

private:
	AActor* pickFromEye();
	void releaseItem();
	void pickingItem();
	void puttingItem();

	void drawItemHint();
};