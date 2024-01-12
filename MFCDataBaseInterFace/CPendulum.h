#pragma once

#include "pch.h"
#include <math.h>
#define M_PI 3.1415926535

class CPendulum
{
public:
    CPendulum(double length, double angle, double gravity, double mass, double resistance)
        : m_length(length), m_angle(angle), m_gravity(gravity), m_mass(mass),
        m_resistance(resistance)
    {
        m_angularVelocity = 0.0;
        m_kineticEnergy = 0.0;
        m_potentialEnergy = 0.0;
        x = 0.0;
        y = 0.0;
        scale = 100;
        scale = 100;
    }
    virtual ~CPendulum() {}
    void Init(double length, double angle, double gravity, double mass, double resistance);
    void Update();
    void DrawGraph(CDC& dc, const CRect& rect);
    void DrawPendulum(CDC& dc, const CRect& rect);
    void setAcceleration(double value);
    void addAcceleration(double value);
    double GetKinetic() { return m_kineticEnergy; }
    double GetPotentional() { return m_potentialEnergy; }
    void SetLength(double length);
    void SetAngle(double angle);
    void SetGravity(double gravity);
    void SetMass(double mass);
    void SetResistance(double resistance);
    double GetLength();
    double GetAngle();
    double GetGravity();
    double GetMass();
    double GetResistance();

private:
    double m_length;
    double m_angle;
    double m_gravity;
    double m_mass;
    double m_angularVelocity;
    double m_resistance;
    double m_kineticEnergy;
    double m_potentialEnergy;
    double scale;
    double x;
    double y;
    void CalculateKineticEnergy();
    void CalculatePotentialEnergy();
    double CalculateAngularAcceleration();
    double CalculateXCoordinate(const CRect& rect);
    double CalculateYCoordinate(const CRect& rect);


};
