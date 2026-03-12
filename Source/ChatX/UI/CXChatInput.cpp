// Fill out your copyright notice in the Description page of Project Settings.

//TODO: 메세지큐 유니크 포인터로
//ui 동작에서 사용


#include "UI/CXChatInput.h"

#include "Components/EditableText.h"
#include "Components/EditableTextBox.h"
#include "Player/CXPlayerController.h"

void UCXChatInput::NativeConstruct()
{
	Super::NativeConstruct();

	if (!EditableTextBox_ChatInput)
	{
		return;
	}

	if (EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted) == false)
	{
		EditableTextBox_ChatInput->OnTextCommitted.AddDynamic(this, &ThisClass::OnChatInputTextCommitted);
	}
}

void UCXChatInput::NativeDestruct()
{
	Super::NativeDestruct();

	if (!EditableTextBox_ChatInput)
	{
		return;
	}

	if (EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted) == true)
	{
		EditableTextBox_ChatInput->OnTextCommitted.RemoveDynamic(this, &ThisClass::OnChatInputTextCommitted);
	}
}

void UCXChatInput::OnChatInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		APlayerController* OwningPlayerController = GetOwningPlayer();
		if (!IsValid(OwningPlayerController))
		{
			return;
		}
		ACXPlayerController* OwningCxPlayerController = Cast<ACXPlayerController>(OwningPlayerController);

		if (!IsValid(OwningCxPlayerController))
		{
			return;
		}
		OwningCxPlayerController->SetChatMessageString(Text.ToString());

		EditableTextBox_ChatInput->SetText(FText());
	}
}
