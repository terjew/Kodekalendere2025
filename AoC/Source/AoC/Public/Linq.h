// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <type_traits>
#include "CoreMinimal.h"

namespace UnrealLinq {

	template <class _TIn, 
		class _Fn,
		class _TOut = typename std::decay<decltype(std::declval<_Fn>()(std::declval<_TIn>()))>::type
	>
	inline TArray<_TOut> Select(const TArray<_TIn>& From, _Fn _Func) {
		TArray<_TOut> Out;
		Out.SetNum(From.Num());
		std::transform(From.GetData(), From.GetData() + From.Num(), Out.GetData(), _Func);
		return Out;
	}

	template <class _Fn,
		class _TOut = typename std::decay<decltype(std::declval<_Fn>()(std::declval<TCHAR>()))>::type
	>
	inline TArray<_TOut> SelectString(const FString& From, _Fn _Func) {
		TArray<_TOut> Out;
		Out.SetNum(From.Len());
		auto Data = From.GetCharArray().GetData();
		std::transform(Data, Data + From.Len(), Out.GetData(), _Func);
		return Out;
	}

	template <class _TIn,
		class _Fn,
		class _TOut = typename std::decay<decltype(std::declval<_Fn>()(std::declval<_TIn>()))>::type
	>
	inline TArray<_TIn> Skip(const TArray<_TIn>& From, size_t SkipNum) {
		TArray<_TOut> Out;
		size_t NumElementsToCopy = From.Num() > SkipNum ? From.Num() - SkipNum : 0;
		Out.SetNum(NumElementsToCopy);
		for (int32 i = 0; i < NumElementsToCopy; ++i)
		{
			Out[i] = From[SkipNum + i];
		}
		return Out;
	}

	template <class _TIn>
	inline size_t MaxIndex(const TArray<_TIn> From) {
		return std::max_element(From.GetData(), From.GetData() + From.Num()) - From.GetData();
	}

	template <class _TIn>
	inline size_t MaxIndex(const TArray<_TIn> From, size_t startIndex, size_t endIndex) {
		return std::max_element(From.GetData() + startIndex, From.GetData() + endIndex) - From.GetData();
	}

}