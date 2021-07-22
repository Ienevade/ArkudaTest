// Fill out your copyright notice in the Description page of Project Settings.


#include "MainChar.h"

#include <iso646.h>

#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "Item.h"
#include "Blueprint/UserWidget.h"
#include "InventoryComponent.h"

// Sets default values
AMainChar::AMainChar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Добавление СпрингАрма для камеры, установка элемента для присоединения, установка смещения
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SocketOffset = FVector(0.0f, 80.0f,80.f);
	//Добавление компонента камеры, прикрепление к СпрингАрму
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	PhysicsHandleComponent = CreateDefaultSubobject<UPhysicsHandleComponent>("PhysicHandleComponent");

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");
	

}



// Called to bind functionality to input
void AMainChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Bind Func to Axis ans Actions 
	PlayerInputComponent->BindAxis("MoveForward", this, &AMainChar::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainChar::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &AMainChar::LookUp);
	PlayerInputComponent->BindAxis("TurnAround", this, &AMainChar::TurnAround);
	
	PlayerInputComponent->BindAction("Grab",IE_Pressed, this,&AMainChar::Grab);
	//Commented out as used in BP
	// PlayerInputComponent->BindAction("PickUp",IE_Pressed, this,&AMainChar::PickUp);
	// PlayerInputComponent->BindAction("Drop",IE_Pressed, this,&AMainChar::Drop);

	PlayerInputComponent->BindAction("IterSlot",IE_Pressed, this, &AMainChar::IterSlots);
	PlayerInputComponent->BindAction("DeIterslot",IE_Pressed, this,&AMainChar::DeIterslots);
	
}

void AMainChar::MoveForward(float Amount)
{
	AddMovementInput(GetActorForwardVector(), Amount);
}

void AMainChar::MoveRight(float Amount)
{

	AddMovementInput(GetActorRightVector(), Amount);
}

void AMainChar::LookUp(float Amount)
{
	AddControllerPitchInput(Amount*(-1));
}

void AMainChar::TurnAround(float Amount)
{
	AddControllerYawInput(Amount);
}

void AMainChar::Grab()
{
	//Init variables Start and End of Trace
	FVector StartLoc = CameraComponent->GetComponentLocation();
	FVector EndLoc = StartLoc + (CameraComponent->GetForwardVector() * 700);
	//Trace Result
	FHitResult HitResult;
	
	FCollisionQueryParams Params;

	//Make setting struct for Attach\Detach 
	FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true );
	FDetachmentTransformRules DetachmentTransformRules = FDetachmentTransformRules(EDetachmentRule::KeepWorld, true);
	//Trace
	GetWorld()->LineTraceSingleByChannel(HitResult, StartLoc, EndLoc, ECollisionChannel::ECC_Visibility, Params, FCollisionResponseParams()) ;
	
	if (ItemInHand!=nullptr)
		{	//Detach if item in hand
			ItemInHand->DetachFromActor(DetachmentTransformRules);
			ItemInHand->SetActorEnableCollision(true);
			//Detach on land
			FVector DropLocation = this->GetMesh()->GetSocketLocation("hand_rSocket");
			DropLocation.Z = 0.0f;
			ItemInHand->SetActorLocation(DropLocation);
			//Clear variable
			ItemInHand=nullptr;
			return;
			
		}
		
	
	
	ItemInHand = Cast<AItem>(HitResult.GetActor());

	if (ItemInHand == nullptr)
	{	//if actor not Item exit
		return;
	}
	
	//Disable collision and attach to hand socket
	
	HitResult.GetActor()->SetActorEnableCollision(false);
	HitResult.GetActor()->AttachToComponent(this->GetMesh() ,AttachmentTransformRules , "hand_rSocket");
	

}

void AMainChar::PickUp()
{	//If inventory slot empty, add item
	if(InventoryComponent->GetItem(ActiveSlot) == nullptr and ItemInHand!=nullptr)
	{
		InventoryComponent->SetItem(ActiveSlot,ItemInHand);
		ItemInHand->Destroy();
		ItemInHand=nullptr;
	}
}

void AMainChar::Drop()
{
	if(InventoryComponent->GetItem(ActiveSlot)!=nullptr)
	{
		AItem* SpawnActor = InventoryComponent->GetItem(ActiveSlot);
		FVector SpawnLocation = this->GetMesh()->GetSocketLocation("hand_rSocket");
		SpawnLocation.Z = 0.0f;
		GetWorld()->SpawnActor(SpawnActor->GetClass(), &SpawnLocation );
		InventoryComponent->SetItem(ActiveSlot, nullptr);
	}
}

void AMainChar::IterSlots()
{
	if (ActiveSlot==InventoryComponent->MaxItems-1)
	{
		ActiveSlot=0;
		return;
	}
	ActiveSlot++;
	
}

void AMainChar::DeIterslots()
{
	if (ActiveSlot==0)
	{
		ActiveSlot=2;
		return;
	}
	ActiveSlot--;
}



