// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/CXGameModeBase.h"
#include "Player/CXPlayerController.h"
#include "EngineUtils.h"

FString ACXGameModeBase::GenerateSecretNumber()
{
	TArray<int32> Numbers;
	for (int32 i = 1; i <= 9; i++)
	{
		Numbers.Add(1);
	}
	FMath::RandInit(FDateTime::Now().GetTicks());
	Numbers = Numbers.FilterByPredicate([](int32 Num) { return Num>0;});

	FString Result;
	for (int32 i = 0; i<3; ++i)
	{
		int32 Index = FMath::RandRange(0,Numbers.Num() -1);
		Result.Append(FString::FromInt(Numbers[Index]));
		Numbers.RemoveAt(Index);
	}
	
	return Result;
}

bool ACXGameModeBase::IsGuessNumberString(const FString& InNumberString)
{
	bool bCanPlay = false;
	do
	{
		if (InNumberString.Len() != 3)
		{
			break;
		}
		
		bool bIsUnique = true;
		TSet<TCHAR> UniqueDigits;
		
		for (TCHAR C : InNumberString)
		{
			if (FChar::IsDigit(C) == false || C == '0')
			{
				bIsUnique = false;
				break;
			}
			
			UniqueDigits.Add(C);
		}
		
		if (bIsUnique == false)
		{
			break;
		}
		
		bCanPlay = true;
	}
	while (false);
	
	return bCanPlay;
}

FString ACXGameModeBase::JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString)
{
	int32 StrikeCount = 0, BallCount = 0;
	
	for(int32 i = 0; i<3; ++i)
	{
		if (In`[i] == InGuessNumberString[i])
		{
			StrikeCount++;
		}
		else
		{
			{
				FString PlayerGuessChar = FString::Printf(TEXT("%C"), InGuessNumberString[i]);
				if (InSecretNumberString.Contains(PlayerGuessChar))
				{
					BallCount++;
				}
			}
		}
	}
	
	if (StrikeCount == 0 && BallCount == 0)
	{
		return TEXT("OUT");
	}
	
	return FString::Printf(TEXT("%dS%dB"), StrikeCount, BallCount);
}

void ACXGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	SecretNumberString = GenerateSecretNumber();
}

void ACXGameModeBase::PrintChatMessageString(
	ACXPlayerController* InChattingPlayerController,
	const FString& InChatMessageString
	)
{
	int Index = InChatMessageString.Len() -3;
	FString GuessNumberString = InChatMessageString.RightChop(Index);
	if (IsGuessNumberString(GuessNumberString) == true)
	{
		FString JudgeResultString = JudgeResult(SecretNumberString, GuessNumberString);
		for (TActorIterator<ACXPlayerController> It(GetWorld()); It; ++It)
		{
			ACXPlayerController* CXPlayerContoller = *It;
			if (IsValid(CXPlayerContoller) == true)
			{
				FString CombinedMessageString = InChatMessageString + TEXT(" -> ") + JudgeResultString;
				CXPlayerContoller->ClientRPCPrintChatMessageString(CombinedMessageString);
			}
		}
	}
	else
	{
		for (TActorIterator<ACXPlayerController> It(GetWorld()); It; ++It)
		{
			ACXPlayerController* CXPlayerController = *It;
			if (IsValid(CXPlayerController)==true)
			{
				CXPlayerController->ClientRPCPrintChatMessageString(InChatMessageString);
			}
		}
	}
	
	
}
