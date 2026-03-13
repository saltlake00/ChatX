// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CXPlayerController.h"

#include "ChatX.h"
#include "UI/CXChatInput.h"
#include "Kismet/KismetSystemLibrary.h"

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

	PrintChatMessageString(ChatMessageString);
}

void ACXPlayerController::PrintChatMessageString(const FString& InChatMessageString)
{
	//UKismetSystemLibrary::PrintString(this, ChatMessageString, true, true, FLinearColor::Red, 5.0f);

	/*
	FString NetModeString = ChatXFunctionLibrary::GetNetModeString(this);
	FString CombinedMessageString = FString::Printf(TEXT("%s, %s"), *NetModeString, *InChatMessageString);
	ChatXFunctionLibrary::MyPrintString(this, CombinedMessageString, 10.f);
	*/
	
	ChatXFunctionLibrary::MyPrintString(this, InChatMessageString, 15.f);
}
