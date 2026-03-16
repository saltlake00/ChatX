// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CXPlayerController.h"

#include "ChatX.h"
#include "UI/CXChatInput.h"
#include "Kismet/KismetSystemLibrary.h"
#include "EngineUtils.h"

void ACXPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController() == false)
	{
		return;
	}

	FInputModeUIOnly InputModeUIOnly;
	SetInputMode(InputModeUIOnly);

	if (!IsValid(ChatInputWidgetClass) == true)
	{
		return;
	}

	ChatInputWidgetInstance = CreateWidget<UCXChatInput>(this, ChatInputWidgetClass);

	if (!IsValid(ChatInputWidgetInstance) == true)
	{
		return;
	}

	ChatInputWidgetInstance->AddToViewport();
}

void ACXPlayerController::SetChatMessageString(const FString& InChatMessageString)
{
	ChatMessageString = InChatMessageString;

	if (IsLocalController() == true)
	{
		ServerRPCPrintChatMessageString(InChatMessageString);
	}
}

void ACXPlayerController::PrintChatMessageString(const FString& InChatMessageString)
{

	ChatXFunctionLibrary::MyPrintString(this, InChatMessageString, 15.f);
}

void ACXPlayerController::ClientRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	PrintChatMessageString(InChatMessageString);
}

void ACXPlayerController::ServerRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	for (TActorIterator<ACXPlayerController> It(GetWorld()); It; ++It)
	{
		ACXPlayerController* CXPlayerController = *It;
		if (IsValid(CXPlayerController) == true)
		{
			CXPlayerController->ClientRPCPrintChatMessageString(InChatMessageString);
		}
	}
}