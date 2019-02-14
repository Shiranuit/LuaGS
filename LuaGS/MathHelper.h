#pragma once
class MathHelper
{
public:
	template <typename T>
	static T min(T a, T b)
	{
		return a < b ? a : b;
	}

	template <typename T>
	static T max(T a, T b)
	{
		return a > b ? a : b;
	}

	template <typename T>
	static T clamp(T val, T minval, T maxval)
	{
		return MathHelper::max(minval, MathHelper::min(val, maxval));
	}
};


