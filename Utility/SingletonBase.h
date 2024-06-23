#pragma once

template <typename T>
class SingletonBase
{
public:
	static T& GetInstance()
	{
		static T s;
		return s;
	}

protected:
	SingletonBase() = default;
	SingletonBase(const SingletonBase&) = delete;
	SingletonBase& operator=(const SingletonBase&) = delete;
	SingletonBase(SingletonBase&&) = delete;
	SingletonBase& operator=(SingletonBase&&) = delete;
};