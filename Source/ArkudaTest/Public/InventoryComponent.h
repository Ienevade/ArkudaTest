// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Item.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARKUDATEST_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	int32 MaxItems = 3;
	
	void SetItem(int i, AItem* Item)
	{
		ItemTable[i]=Item;
	}

	AItem* GetItem(int i)
	{
		return ItemTable[i];
	}
	
	
	UInventoryComponent();

protected:
	// Called when the game starts
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray <AItem*> ItemTable;


	virtual void BeginPlay() override;

};
