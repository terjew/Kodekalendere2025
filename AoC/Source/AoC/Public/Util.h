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

	inline FString Int64ToString(int64 Number)
	{
		return FString::Printf(TEXT("%lld"), Number);
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

	inline TArray<FString> FileAsLines(const FString& LocalPath)
	{
		FString FileContent = FileAsString(LocalPath);
		TArray<FString> Lines;
		FString Delimiter = TEXT("\n");
		FileContent.ParseIntoArray(Lines, *Delimiter, true);
		return Lines;
	}

	template <class _TIn, class _TOut, class _Fn>
	inline void selectInto(const TArray<_TIn> From, TArray<_TOut> & To, _Fn _Func) {
		To.SetNum(From.Num());
		std::transform(From.GetData(), From.GetData() + From.Num(), To.GetData(), _Func);
	}


	template <class _TIn, class _TOut, class _Fn>
	inline void selectInto(const _TIn * FromPtr, size_t FromSize, TArray<_TOut> & To, _Fn _Func) {
		To.SetNum(FromSize);
		std::transform(FromPtr, FromPtr + FromSize, To.GetData(), _Func);
	}

	template <class _TIn, class _TOut, class _Fn>
	inline TArray<_TOut> select(const TArray<_TIn> From, _Fn _Func) {
		TArray<_TOut> To;
		select(From, To, _Func);
		return To;
	}

	template <class _TIn>
	inline size_t maxIndex(const TArray<_TIn> From) {
		return std::max_element(From.GetData(), From.GetData() + From.Num()) - From.GetData();
	}

	template <class _TIn>
	inline size_t maxIndex(const TArray<_TIn> From, size_t startIndex, size_t endIndex) {
		return std::max_element(From.GetData() + startIndex, From.GetData() + endIndex) - From.GetData();
	}


}