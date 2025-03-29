#pragma once

#define M_PI 3.14159265358979323846

struct FNRot
{
	double a;
	double b;
	double c;
};

class sdk_class
{
public:
	static camera_position_s GetCamera()
	{
		camera_position_s camera;

		uintptr_t location_pointer = memory.Read<uintptr_t>(cache.UWorld + 0x138); //
		uintptr_t rotation_pointer = memory.Read<uintptr_t>(cache.UWorld + 0x148); //
		FNRot fnrot{};
		fnrot.a = memory.Read<double>(rotation_pointer);
		fnrot.b = memory.Read<double>(rotation_pointer + 0x20);
		fnrot.c = memory.Read<double>(rotation_pointer + 0x1D0);
		camera.location = memory.Read<FVector>(location_pointer);
		camera.rotation.x = asin(fnrot.c) * (180.0 / M_PI);
		camera.rotation.y = ((atan2(fnrot.a * -1, fnrot.b) * (180.0 / M_PI)) * -1) * -1;
		camera.fov = memory.Read<float>(cache.PlayerController + 0x3AC) * 90.0f;

		return camera;
	}

	static FVector2D ProjectWorldToScreen(FVector WorldLocation)
	{
		camera_postion = GetCamera();

		if (WorldLocation.x == 0) return FVector2D(0, 0);

		_MATRIX tempMatrix = Matrix(camera_postion.rotation);
		FVector vAxisX = FVector(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]);
		FVector vAxisY = FVector(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]);
		FVector vAxisZ = FVector(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);
		FVector vDelta = WorldLocation - camera_postion.location;
		FVector vTransformed = FVector(vDelta.Dot(vAxisY), vDelta.Dot(vAxisZ), vDelta.Dot(vAxisX));

		if (vTransformed.z < 1.f) vTransformed.z = 1.f;

		return FVector2D((globals.ScreenWidth / 2.0f) + vTransformed.x * (((globals.ScreenWidth / 2.0f) / tanf(camera_postion.fov * (float)M_PI / 360.f))) / vTransformed.z, (globals.ScreenHeight / 2.0f) - vTransformed.y * (((globals.ScreenWidth / 2.0f) / tanf(camera_postion.fov * (float)M_PI / 360.f))) / vTransformed.z);
	}
}; inline sdk_class sdk;

class bones_class
{
public:
	static FVector GetBoneLocation(uintptr_t mesh, int bone_id)
	{
		uintptr_t bone_array = memory.Read<uintptr_t>(mesh + offsets::BoneArray);
		if (bone_array == 0) bone_array = memory.Read<uintptr_t>(mesh + offsets::BoneCache);
		FTransform bone = memory.Read<FTransform>(bone_array + (bone_id * 0x60));
		FTransform component_to_world = memory.Read<FTransform>(mesh + offsets::ComponentToWorld);
		D3DMATRIX matrix = MatrixMultiplication(bone.ToMatrixWithScale(), component_to_world.ToMatrixWithScale());
		return FVector(matrix._41, matrix._42, matrix._43);
	}
}; inline bones_class bones;

class utility_class
{
public:
	bool IsEnemyVisible(uintptr_t Mesh_MJ)
	{
		auto Seconds = memory.Read<double>(cache.UWorld + 0x158);
		auto LastRenderTime = memory.Read<float>(Mesh_MJ + 0x32C);
		return Seconds - LastRenderTime <= 0.06f;
	}

	bool IsEnemyInScreen(FVector2D screen_location)
	{
		if (screen_location.x > 0 && screen_location.x < globals.ScreenWidth && screen_location.y > 0 && screen_location.y < globals.ScreenHeight) return true;
		else return false;
	}
}; inline utility_class utility;