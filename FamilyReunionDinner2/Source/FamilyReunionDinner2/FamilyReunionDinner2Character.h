#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "APIClass.h"
#include "RecipeCard.h"
#include "Net/UnrealNetwork.h"
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

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	UPROPERTY(EditDefaultsOnly, Category = Card)
		TSubclassOf<class ARecipeCard> recipeCard;

protected:
	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

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

public:
	UPROPERTY(Replicated)
	TArray<FRecipeCardStruct> fileData;


	//UPROPERTY(replicated)
	//	int32 test;

public:
	UFUNCTION(BlueprintCallable)
	void useSpecialAction();

	UFUNCTION(Reliable, Server)
		void moveActor();
	void moveActor_Implementation();
	//void moveActor_Validate();

	UFUNCTION(Reliable, Server)
		void moveActorAnother();
	void moveActorAnother_Implementation();

	UFUNCTION(Reliable, NetMulticast)
		void moveActorRA();
	void moveActorRA_Implementation();

	UFUNCTION(Reliable, Server)
		void startGame();
	void startGame_Implementation();

	UFUNCTION(Reliable, Server)
		void moveToDeck(AActor* hitActor);
	void moveToDeck_Implementation(AActor* hitActor);

private:
	void pickFromEye();

};