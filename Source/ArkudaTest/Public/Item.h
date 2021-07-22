// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/Image.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class ARKUDATEST_API AItem : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	UStaticMeshComponent* MeshComponent;


	
	
	
	// Sets default values for this actor's properties
	AItem();


};
