#pragma once
#include "pch.h"
#include "CPendulum.h" 
#include "FlickerFreeDC.h"
void CPendulum::CalculateKineticEnergy()
{
    double velocity = m_angularVelocity * m_length;
    m_kineticEnergy = 0.5 * (m_mass * velocity * velocity);
}

void CPendulum::CalculatePotentialEnergy()
{
    double height = m_length * (1 - cos(m_angle * M_PI / 180.0));
    m_potentialEnergy = m_mass * m_gravity * height;
}

void CPendulum::setAcceleration(double value)
{
    m_angularVelocity = value;
}

void CPendulum::addAcceleration(double value)
{
    m_angularVelocity += value;
}

void CPendulum::Init(double length, double angle, double gravity,
    double mass, double resistance)
{
    m_length = length;
    m_angle = angle;
    m_gravity = gravity;
    m_mass = mass;
    m_resistance = resistance;
}

void CPendulum::SetLength(double length) { m_length = length; }
void CPendulum::SetAngle(double angle) { m_angle = angle; }
void CPendulum::SetGravity(double gravity) { m_gravity = gravity; }
void CPendulum::SetMass(double mass) { m_mass = mass; }
void CPendulum::SetResistance(double resistance) { m_resistance = resistance; }

double CPendulum::GetMass() const { return m_mass; }
double CPendulum::GetLength() const { return m_length; }
double CPendulum::GetAngle() const { return m_angle; }
double CPendulum::GetGravity() const { return m_gravity; }
double CPendulum::GetResistance() const { return m_resistance; }

void CPendulum::Update()
{
    double angularAcceleration = CalculateAngularAcceleration();
    m_angularVelocity += angularAcceleration;
    m_angle += m_angularVelocity;

    while (m_angle > M_PI)
        m_angle -= 2 * M_PI;
    while (m_angle < -M_PI)
        m_angle += 2 * M_PI;

    CalculateKineticEnergy();
    CalculatePotentialEnergy();
}


void CPendulum::DrawPendulum(CDC& dc, const CRect& rect)
{
    // Use CMemDC to create a memory device context
    FFDC::CMemDC mDC(&dc, &rect);

    // Rest of your drawing code remains unchanged
    mDC.FillSolidRect(rect, RGB(255, 255, 255));
    double x = CalculateXCoordinate(rect);
    double y = CalculateYCoordinate(rect);

    int centerX = rect.Width() / 2;
    int centerY = rect.Height() / 2;

    mDC.MoveTo(centerX, centerY);
    mDC.LineTo(static_cast<int>(x), static_cast<int>(y));

    int basementLength = 40;
    mDC.MoveTo(centerX, centerY);
    mDC.LineTo(centerX, rect.bottom);
    mDC.LineTo(centerX + basementLength, rect.bottom);
    mDC.LineTo(centerX - basementLength, rect.bottom);

    COLORREF ballColor = RGB(255, 0, 0);
    CBrush ballBrush(ballColor);
    CBrush* pOldBrush = mDC.SelectObject(&ballBrush);

    int radius = 8 + m_mass * 1.25;
    CRect ballRect(static_cast<int>(x) - radius, static_cast<int>(y) - radius,
        static_cast<int>(x) + radius, static_cast<int>(y) + radius);
    mDC.Ellipse(ballRect);
  
    mDC.SelectObject(pOldBrush);
}



double CPendulum::CalculateAngularAcceleration()
{
    double airResistance = -m_resistance * m_angularVelocity;
    double gravitationalForce = -m_gravity / (m_length * scale) * sin(m_angle);
    double massFactor = m_mass * (m_length * scale);
    return (gravitationalForce + airResistance) / massFactor;
}

double CPendulum::CalculateXCoordinate(const CRect& rect)
{
    int centerX = rect.Width() / 2;
    x = centerX + m_length * scale * sin(m_angle);
    m_x = x;
    return x;
}

double CPendulum::CalculateYCoordinate(const CRect& rect)
{
    int centerY = rect.Height() / 2;
    y = centerY + m_length * scale * cos(m_angle);
    m_y = y;
    return y;
}
