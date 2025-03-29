#pragma once

#include <d3d9types.h>

#define PI 3.14159265358979323846f

D3DMATRIX MatrixMultiplication(D3DMATRIX pm1, D3DMATRIX pm2)
{
	D3DMATRIX pout{};
	pout._11 = pm1._11 * pm2._11 + pm1._12 * pm2._21 + pm1._13 * pm2._31 + pm1._14 * pm2._41;
	pout._12 = pm1._11 * pm2._12 + pm1._12 * pm2._22 + pm1._13 * pm2._32 + pm1._14 * pm2._42;
	pout._13 = pm1._11 * pm2._13 + pm1._12 * pm2._23 + pm1._13 * pm2._33 + pm1._14 * pm2._43;
	pout._14 = pm1._11 * pm2._14 + pm1._12 * pm2._24 + pm1._13 * pm2._34 + pm1._14 * pm2._44;
	pout._21 = pm1._21 * pm2._11 + pm1._22 * pm2._21 + pm1._23 * pm2._31 + pm1._24 * pm2._41;
	pout._22 = pm1._21 * pm2._12 + pm1._22 * pm2._22 + pm1._23 * pm2._32 + pm1._24 * pm2._42;
	pout._23 = pm1._21 * pm2._13 + pm1._22 * pm2._23 + pm1._23 * pm2._33 + pm1._24 * pm2._43;
	pout._24 = pm1._21 * pm2._14 + pm1._22 * pm2._24 + pm1._23 * pm2._34 + pm1._24 * pm2._44;
	pout._31 = pm1._31 * pm2._11 + pm1._32 * pm2._21 + pm1._33 * pm2._31 + pm1._34 * pm2._41;
	pout._32 = pm1._31 * pm2._12 + pm1._32 * pm2._22 + pm1._33 * pm2._32 + pm1._34 * pm2._42;
	pout._33 = pm1._31 * pm2._13 + pm1._32 * pm2._23 + pm1._33 * pm2._33 + pm1._34 * pm2._43;
	pout._34 = pm1._31 * pm2._14 + pm1._32 * pm2._24 + pm1._33 * pm2._34 + pm1._34 * pm2._44;
	pout._41 = pm1._41 * pm2._11 + pm1._42 * pm2._21 + pm1._43 * pm2._31 + pm1._44 * pm2._41;
	pout._42 = pm1._41 * pm2._12 + pm1._42 * pm2._22 + pm1._43 * pm2._32 + pm1._44 * pm2._42;
	pout._43 = pm1._41 * pm2._13 + pm1._42 * pm2._23 + pm1._43 * pm2._33 + pm1._44 * pm2._43;
	pout._44 = pm1._41 * pm2._14 + pm1._42 * pm2._24 + pm1._43 * pm2._34 + pm1._44 * pm2._44;
	return pout;
}

struct FQuat
{
	double x;
	double y;
	double z;
	double w;
};

struct FTransform
{
	FQuat rotation;
	FVector translation;
	uint8_t pad1c[0x8];
	FVector scale3d;
	uint8_t pad2c[0x8];

	D3DMATRIX ToMatrixWithScale()
	{
		D3DMATRIX m{};

		const FVector Scale
		(
			(scale3d.x == 0.0) ? 1.0 : scale3d.x,
			(scale3d.y == 0.0) ? 1.0 : scale3d.y,
			(scale3d.z == 0.0) ? 1.0 : scale3d.z
		);

		const double x2 = rotation.x + rotation.x;
		const double y2 = rotation.y + rotation.y;
		const double z2 = rotation.z + rotation.z;
		const double xx2 = rotation.x * x2;
		const double yy2 = rotation.y * y2;
		const double zz2 = rotation.z * z2;
		const double yz2 = rotation.y * z2;
		const double wx2 = rotation.w * x2;
		const double xy2 = rotation.x * y2;
		const double wz2 = rotation.w * z2;
		const double xz2 = rotation.x * z2;
		const double wy2 = rotation.w * y2;

		m._41 = translation.x;
		m._42 = translation.y;
		m._43 = translation.z;
		m._11 = (1.0f - (yy2 + zz2)) * Scale.x;
		m._22 = (1.0f - (xx2 + zz2)) * Scale.y;
		m._33 = (1.0f - (xx2 + yy2)) * Scale.z;
		m._32 = (yz2 - wx2) * Scale.z;
		m._23 = (yz2 + wx2) * Scale.y;
		m._21 = (xy2 - wz2) * Scale.y;
		m._12 = (xy2 + wz2) * Scale.x;
		m._31 = (xz2 + wy2) * Scale.z;
		m._13 = (xz2 - wy2) * Scale.x;
		m._14 = 0.0f;
		m._24 = 0.0f;
		m._34 = 0.0f;
		m._44 = 1.0f;

		return m;
	}
};


struct _MATRIX
{
	union
	{
		struct
		{
			float        _11, _12, _13, _14;
			float        _21, _22, _23, _24;
			float        _31, _32, _33, _34;
			float        _41, _42, _43, _44;

		};
		float m[4][4];
	};
};

inline _MATRIX Matrix(FVector Vec4, FVector origin = FVector(0, 0, 0))
{
	double radPitch = (Vec4.x * double(PI) / 180.f);
	double radYaw = (Vec4.y * double(PI) / 180.f);
	double radRoll = (Vec4.z * double(PI) / 180.f);

	double SP = sinf(radPitch);
	double CP = cosf(radPitch);
	double SY = sinf(radYaw);
	double CY = cosf(radYaw);
	double SR = sinf(radRoll);
	double CR = cosf(radRoll);

	_MATRIX matrix;
	matrix.m[0][0] = CP * CY;
	matrix.m[0][1] = CP * SY;
	matrix.m[0][2] = SP;
	matrix.m[0][3] = 0.f;

	matrix.m[1][0] = SR * SP * CY - CR * SY;
	matrix.m[1][1] = SR * SP * SY + CR * CY;
	matrix.m[1][2] = -SR * CP;
	matrix.m[1][3] = 0.f;

	matrix.m[2][0] = -(CR * SP * CY + SR * SY);
	matrix.m[2][1] = CY * SR - CR * SP * SY;
	matrix.m[2][2] = CR * CP;
	matrix.m[2][3] = 0.f;

	matrix.m[3][0] = origin.x;
	matrix.m[3][1] = origin.y;
	matrix.m[3][2] = origin.z;
	matrix.m[3][3] = 1.f;

	return matrix;
}