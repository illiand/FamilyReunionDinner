// Fill out your copyright notice in the Description page of Project Settings.


#include "ToPotArrow.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AToPotArrow::AToPotArrow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AToPotArrow::BeginPlay()
{
	Super::BeginPlay();
	
	head = Cast<UStaticMeshComponent>(GetDefaultSubobjectByName(TEXT("ArrowHead")));
	body = Cast<UStaticMeshComponent>(GetDefaultSubobjectByName(TEXT("ArrowBody")));

	SetActorScale3D(FVector(0.075f, 0.075f, 0.2f));

	material = UMaterialInstanceDynamic::Create(body->GetMaterial(0), NULL);
	body->SetMaterial(0, material);
}

// Called every frame
void AToPotArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(endPos);

	FVector curWorldDir = UKismetMathLibrary::FindLookAtRotation(startPos, endPos).Vector();
	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(startPos, endPos));
	AddActorLocalRotation(FRotator(0, 90, 0));

	float bodyLength = 175.536957f * 0.075f;
	float curWorldDis = FVector::Distance(startPos, endPos);
	body->SetRelativeScale3D(FVector(1.7675, (curWorldDis + 44.670551f * 0.075f) / bodyLength, 0));

	material->SetScalarParameterValue(TEXT("Scale"), curWorldDis / bodyLength);
}

void AToPotArrow::arrowInit(FVector start, FVector end) 
{
	startPos = start;
	endPos = end;
}