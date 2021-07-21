// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameMode.h"
#include "MainChar.h"

ABaseGameMode::ABaseGameMode()
{
	DefaultPawnClass = AMainChar::StaticClass();
}
