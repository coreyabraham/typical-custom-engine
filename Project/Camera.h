#pragma once

#include "Maths.h"
#include "Graphics.h"

#include "GameObject.h"

enum CameraType
{
    Free = 0,
    Orbit
};

enum CameraDirection
{
    Up = 0,
    Down,
    Left,
    Right,

    Front,
    Back,
};

class Camera
{
private:
    vec3 UP = { 0.0f, 1.0f, 0.0f };
    vec3 DOWN = { 0.0f, -1.0f, 0.0f };
    vec3 LEFT = { 1.0f, 0.0f, 0.0f };
    vec3 RIGHT = { -1.0f, 0.0f, 0.0f };

    vec3 FRONT = { 0.0f, 0.0f, -1.0f };
    vec3 BACK = { 0.0f, 0.0f, 1.0f };

public:
    float Speed = 5.0f;

    float HalfSpeed = Speed / 2.0f;
    float FullSpeed = Speed;

    float Yaw = -90.0f;
    float Pitch = 0.0f;

    vec2 Last = vec2();

    float FOV = 45.0f;
    vec2 ClampFOV = vec2(1.0f, FOV);

    vec3 Position;
    vec3 Direction;

    vec3 ORBIT_viewportAngle = { 0, 1, 0 };
    vec3 ORBIT_viewportZoom = { 0, 1, 0 };

    CameraType CameraType = CameraType::Free;

    vec3 GetLookVector(CameraDirection Direction) const;
    mat4 Draw(const float aspectRatio);
};