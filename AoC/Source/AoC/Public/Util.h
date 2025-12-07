// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFileManager.h"

namespace Util
{
	inline int64 StringToInt64(const FString& Str)
	{
		return FCString::Atoi64(*Str);
	}

	inline int CharToInt(const TCHAR& Char)
	{
		return Char - _T('0');
	}

	inline FString Int64ToString(int64 Number)
	{
		return FString::Printf(TEXT("%lld"), Number);
	}

	inline std::pair<int64, int64> StringToRange(const FString& Str)
	{
		FString Left, Right;
		Str.Split(TEXT("-"), &Left, &Right);
		return std::make_pair(FCString::Atoi64(*Left), FCString::Atoi64(*Right));
	}

	inline FString FileAsString(const FString& LocalPath)
	{
		const FString FilePath = FPaths::ProjectContentDir() + LocalPath;
		FString FileContent;
		if (!FFileHelper::LoadFileToString(FileContent, *FilePath))
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to load file: %s"), *FilePath);
		}
		return FileContent;
	}

	inline TArray<FString> FileAsLines(const FString& LocalPath, bool bCullEmptyLines = true)
	{
		FString FileContent = FileAsString(LocalPath);
		TArray<FString> Lines;
		FileContent.ParseIntoArrayLines(Lines, bCullEmptyLines);
		return Lines;
	}

}
