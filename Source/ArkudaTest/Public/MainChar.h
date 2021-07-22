// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "InventoryComponent.h"
#include "Item.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"

#include "GameFramework/SpringArmComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

#include "MainChar.generated.h"
//Предварительно объявление класса UCameraComponent


UCLASS()
class ARKUDATEST_API AMainChar : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainChar();

protected:
	//Add Char components
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	UCameraComponent * CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	AItem* ItemInHand;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	int32 ActiveSlot = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	UPhysicsHandleComponent* PhysicsHandleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	UInventoryComponent* InventoryComponent;
	// Called when the game starts or when spawned
	
	

public:	
	// Called every frame
	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Define func for move
private:
	void MoveForward(float Amount);
	void MoveRight(float Amount);

	//Define func for look
	void LookUp(float Amount);
	void TurnAround(float Amount);

	//Define func for Grab, Drop, PickUp
	
	void Grab();
	UFUNCTION(BlueprintCallable)
	void PickUp();
	UFUNCTION(BlueprintCallable)
	void Drop();
	//Change active slot func
	void IterSlots();
	void DeIterslots();
	
};
