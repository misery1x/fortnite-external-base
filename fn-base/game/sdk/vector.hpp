#pragma once

// Windows
#include <corecrt_math.h>

// Custom

class FVector2D
{
public:
	FVector2D() : x(0.f), y(0.f)
	{

	}

	FVector2D(double _x, double _y) : x(_x), y(_y)
	{

	}
	~FVector2D()
	{

	}

	FVector2D operator-(FVector2D v)
	{
		return FVector2D(x - v.x, y - v.y);
	}

	double x;
	double y;
};

class FVector
{
public:
	FVector() : x(0.f), y(0.f), z(0.f)
	{

	}

	FVector(double _x, double _y, double _z) : x(_x), y(_y), z(_z)
	{

	}
	~FVector()
	{

	}

	double x;
	double y;
	double z;

	inline double Dot(FVector v)
	{
		return x * v.x + y * v.y + z * v.z;
	}

	inline double Distance(FVector v)
	{
		return double(sqrtf(powf(v.x - x, 2.0) + powf(v.y - y, 2.0) + powf(v.z - z, 2.0)));
	}

	inline double Length()
	{
		return sqrt(x * x + y * y + z * z);
	}

	FVector operator+(FVector v)
	{
		return FVector(x + v.x, y + v.y, z + v.z);
	}

	FVector operator-(FVector v)
	{
		return FVector(x - v.x, y - v.y, z - v.z);
	}

	FVector operator/(double flNum)
	{
		return FVector(x / flNum, y / flNum, z / flNum);
	}

	FVector operator*(double flNum) { return FVector(x * flNum, y * flNum, z * flNum); }
};