// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CXPawn.h"

#include "ChatX.h"

// Sets default values
ACXPawn::ACXPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACXPawn::BeginPlay()
{
	Super::BeginPlay();

	FString NetRoleString = ChatXFunctionLibrary::GetRoleString(this);
	FString CombinedString = FString::Printf(
		TEXT("CXPawn::BeginPlay() %s [%s]"),
		*ChatXFunctionLibrary::GetNetModeString(
			this),
		*NetRoleString);
	ChatXFunctionLibrary::MyPrintString(
		this,
		CombinedString,
		10.f);
}

void ACXPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	FString NetRoleString = ChatXFunctionLibrary::GetRoleString(this);
	FString CombinedString = FString::Printf(
		TEXT("CXPawn::PossessedBy() %s [%s]"),
		*ChatXFunctionLibrary::GetNetModeString(
			this),
		*NetRoleString);
	ChatXFunctionLibrary::MyPrintString(
		this,
		CombinedString,
		10.f);
}
