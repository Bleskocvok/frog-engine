#pragma once


struct Paddle
{
    static constexpr float Speed = 100;  // in degrees per second
    static constexpr float AngularSize = 30;   // in degrees
    static constexpr int Count = 3;
    static constexpr float Radius = 6;
    static constexpr float Thickness = 0.75;
    static constexpr float Height = 0.75;
};


struct Wall
{
    static constexpr int NumberPerLevel = 12;
    static constexpr int Levels = 5;
    static constexpr float Radius = 2;
    static constexpr float Thickness = 0.5;
    static constexpr float Height = 0.5;
    inline static const float AngularSize = 360.f / NumberPerLevel;
};


struct Ball
{
    static constexpr float Radius = 0.3;
    static constexpr float DefaultSpeed = 2;

    static constexpr float IncrementSpeed = 1.25;
    static constexpr float IncrementPeriod = 60;    // in sec

    static constexpr float SpeedupValue = 1.5;
    static constexpr float SpeedupArea = 2.5;
    static constexpr float SpeedupDuration = 0.5;     // in sec
};


static constexpr float GroundSize = 8;

static constexpr int Sides = 30;    // generated meshes sides

static constexpr int StartingLives = 3;

static constexpr float PowerupSize = 0.3;
