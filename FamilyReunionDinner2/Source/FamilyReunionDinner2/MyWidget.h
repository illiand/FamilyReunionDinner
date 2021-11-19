// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class FAMILYREUNIONDINNER2_API UMyWidget : public UUserWidget
{
public:
	SLATE_BEGIN_ARGS(SMyWidget)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
};
