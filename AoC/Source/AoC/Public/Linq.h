// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <type_traits>
#include "CoreMinimal.h"
#include <functional>
#include <numeric>

namespace UnrealLinq {

	template <
		class _TIn, 
		class _Fn,
		class _TOut = typename std::decay<decltype(std::declval<_Fn>()(std::declval<_TIn>()))>::type
	>
	inline TArray<_TOut> Select(const TArray<_TIn>& From, _Fn _Func) {
		TArray<_TOut> Out;
		Out.SetNum(From.Num());
		std::transform(From.GetData(), From.GetData() + From.Num(), Out.GetData(), _Func);
		return Out;
	}

	template <
		class _TIn,
		class _Fn,
		class _TOut
	>
	inline _TOut Aggregate(const TArray<_TIn>& From, _TOut start, _Fn _Func) {
		return std::accumulate(From.GetData(), From.GetData() + From.Num(), start, _Func);
	}

	template <class _TIn>
	inline TArray<_TIn> Skip(const TArray<_TIn>& From, size_t SkipNum) {
		TArray<_TIn> Out;
		size_t NumElementsToCopy = From.Num() > SkipNum ? From.Num() - SkipNum : 0;
		Out.SetNum(NumElementsToCopy);
		for (int32 i = 0; i < NumElementsToCopy; ++i)
		{
			Out[i] = From[SkipNum + i];
		}
		return Out;
	}

	template <class _TIn>
	inline TArray<_TIn> Take(const TArray<_TIn>& From, size_t TakeNum) {
		TArray<_TIn> Out;
		size_t NumElementsToCopy = From.Num() >= TakeNum ? TakeNum : From.Num();
		Out.SetNum(NumElementsToCopy);
		for (int32 i = 0; i < NumElementsToCopy; ++i)
		{
			Out[i] = From[i];
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

	template <class _TIn, class _Fn>
	inline _TIn MaxBy(const TArray<_TIn>& From, _Fn _Func) {
		if (From.Num() == 0) {
			return _TIn();
		}

		auto comp = [&_Func](const _TIn& a, const _TIn& b) {
			// std::max_element expects comp(a,b) to be true if a < b
			return _Func(a) < _Func(b);
		};

		auto it = std::max_element(From.GetData(), From.GetData() + From.Num(), comp);
		return *it;
	}


	template <class _TIn, class _Fn>
	inline _TIn MinBy(const TArray<_TIn>& From, _Fn _Func) {
		if (From.Num() == 0) {
			return _TIn();
		}

		auto comp = [&_Func](const _TIn& a, const _TIn& b) {
			// std::max_element expects comp(a,b) to be true if a < b
			return _Func(a) < _Func(b);
			};

		auto it = std::min_element(From.GetData(), From.GetData() + From.Num(), comp);
		return *it;
	}

	template <class _TIn>
	inline _TIn Sum(const TArray<_TIn>& From) {
		return std::accumulate(From.GetData(), From.GetData() + From.Num(), _TIn(), [&](const _TIn& a, const _TIn& b) {
			return a + b;
		});
	}


}