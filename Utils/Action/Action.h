// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

template <typename... args>
struct ActionFunction final
{
private:
	typedef void (UObject::*ptr)(args ...);
	UObject* mCurrentClass;
	ptr mFunction;
public:
	ActionFunction(UObject* _currentClass, ptr _function)
	{
		mCurrentClass = _currentClass;
		mFunction = _function;
	}

	ActionFunction()
	{
		mCurrentClass = nullptr;
		mFunction = nullptr;
	}

	UObject* GetClass() const { return mCurrentClass; }
	ptr GetFunction() const { return mFunction; }
	void Invoke(args ... _args) { (mCurrentClass->*mFunction)(_args...); }
};

/**
 * 
 */
template<typename ... args>
class PROJET_K_API Action final
{
	private:
	TArray<ActionFunction<args...>> mMethod = TArray<ActionFunction<args...>>();
	public:
	void Invoke(args ... _args)
	{
		for (ActionFunction<args...> methods : mMethod)
			methods.Invoke(_args...);

	}

	void Clear() { mMethod.Empty(); }

	template <class T>
	void Add(T* _currentClass, void (T::*ptr)(args ...))
	{
		UObject* _classToAdd = dynamic_cast<UObject*>(_currentClass);
		if (!_classToAdd)return;
		ActionFunction<args...> _methodToAdd(_currentClass, static_cast<void(UObject::*)(args ...)>(ptr));
		mMethod.Add(_methodToAdd);
	}

	template <class T = UObject>
	void Remove(T* _currentClass, void (T::*ptr)(args ...))
	{
		for (int i = 0; i < mMethod.Num(); i++)
			if (mMethod[i].GetClass() == _currentClass && mMethod[i].GetFunction() == static_cast<void(UObject::*
			)(args ...)>(ptr))
			{
				mMethod.RemoveAt(i);
				break;
			}
	}
	void operator ()(args... _args)
	{
		Invoke(_args...);
	}
};
