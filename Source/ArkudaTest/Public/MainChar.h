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
	//Добавление компонента камеры
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	UCameraComponent * CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	AItem* ItemInHand;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	int ActiveSlot;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	UPhysicsHandleComponent* PhysicsHandleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	UInventoryComponent* InventoryComponent;
	// Called when the game starts or when spawned
	
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Объявление функций, используемых для перемещения персонажа
private:
	void MoveForward(float Amount);
	void MoveRight(float Amount);

	//Объявление функций для обзора персонажа
	void LookUp(float Amount);
	void TurnAround(float Amount);

	//Объявление функций для поднятий, захвата и сброса
	void Grab();
	UFUNCTION(BlueprintCallable)
	void PickUp();
	UFUNCTION(BlueprintCallable)
	void Drop();

	UFUNCTION(BlueprintCallable)
	void IterSlots();
	UFUNCTION(BlueprintCallable)
	void DeIterslots();
	
};
