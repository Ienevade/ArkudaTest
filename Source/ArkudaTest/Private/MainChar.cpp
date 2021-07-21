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

// Called when the game starts or when spawned
void AMainChar::BeginPlay()
{
	Super::BeginPlay();
	ActiveSlot = 0;
	
}

// Called every frame
void AMainChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMainChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Бинд функций к Axis Mapping из редактора 
	PlayerInputComponent->BindAxis("MoveForward", this, &AMainChar::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainChar::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &AMainChar::LookUp);
	PlayerInputComponent->BindAxis("TurnAround", this, &AMainChar::TurnAround);
	//Бинд Функций к Action Maping
	PlayerInputComponent->BindAction("Grab",IE_Pressed, this,&AMainChar::Grab);
	PlayerInputComponent->BindAction("PickUp",IE_Pressed, this,&AMainChar::PickUp);
	PlayerInputComponent->BindAction("Drop",IE_Pressed, this,&AMainChar::Drop);

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
	//Опеределние переменных начала и конца трейса
	FVector StartLoc = CameraComponent->GetComponentLocation();
	FVector EndLoc = StartLoc + (CameraComponent->GetForwardVector() * 700);
	//Переменная с результатом трейса
	FHitResult HitResult;
	
	FCollisionQueryParams Params;

	//Создание структуры настроек для присоединения
	FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true );
	FDetachmentTransformRules DetachmentTransformRules = FDetachmentTransformRules(EDetachmentRule::KeepWorld, true);
	//Сам трейс
	if (not GetWorld()->LineTraceSingleByChannel(HitResult, StartLoc, EndLoc, ECollisionChannel::ECC_Visibility, Params, FCollisionResponseParams()) )
	{
		if (ItemInHand!=nullptr)
		{
			ItemInHand->DetachFromActor(DetachmentTransformRules);
			ItemInHand->SetActorEnableCollision(true);
			ItemInHand->MeshComponent->SetSimulatePhysics(true);
			ItemInHand=nullptr;
			
		}
		return;
	}
	ItemInHand = Cast<AItem>(HitResult.GetActor());

	if (ItemInHand == nullptr)
	{
		return;
	}
	
	//Отлключение коллизии присоединяемого объекта
	
	HitResult.GetActor()->SetActorEnableCollision(false);
	HitResult.GetActor()->AttachToComponent(this->GetMesh() ,AttachmentTransformRules , "hand_rSocket");
	

}

void AMainChar::PickUp()
{
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
		auto SpawnActor = InventoryComponent->GetItem(ActiveSlot);
		FVector SpawnLocation = this->GetMesh()->GetSocketLocation("hand_rSocket");
		SpawnLocation.Z = 0.0f;
		GetWorld()->SpawnActor(SpawnActor->GetClass(), &SpawnLocation );
		InventoryComponent->SetItem(ActiveSlot, nullptr);
	}
}

void AMainChar::IterSlots()
{
	if (ActiveSlot==2)
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



