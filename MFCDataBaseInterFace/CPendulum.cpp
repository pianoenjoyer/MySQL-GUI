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
    double height = m_length * (1 - cos(m_angle));
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

void CPendulum::Update(double speedFactor)
{
    double angularAcceleration = CalculateAngularAcceleration();
    m_angularVelocity += angularAcceleration;
    m_angle += m_angularVelocity * speedFactor;  // Adjust speed using a factor

    CalculateKineticEnergy();
    CalculatePotentialEnergy();

    while (m_angle > M_PI)
        m_angle -= 2 * M_PI;
    while (m_angle < -M_PI)
        m_angle += 2 * M_PI;
}


void CPendulum::DrawPendulum(CDC& dc, const CRect& rect)
{
    FFDC::CMemDC mDC(&dc, &rect);
    mDC.FillSolidRect(rect, RGB(200, 220, 220));

    int basementLength = 40;

    x = CalculateXCoordinate(rect);
    y = CalculateYCoordinate(rect);

    int centerX = rect.Width() / 2;
    int centerY = rect.Height() / 2;

    mDC.MoveTo(centerX, centerY);
    mDC.LineTo(static_cast<int>(x), static_cast<int>(y));

    mDC.MoveTo(centerX, centerY);
    mDC.LineTo(centerX, rect.bottom);
    mDC.LineTo(centerX + basementLength, rect.bottom);
    mDC.LineTo(centerX - basementLength, rect.bottom);

    mDC.LineTo(centerX, centerX + basementLength);
    mDC.LineTo(centerX, centerX - basementLength);

    COLORREF ballColor = RGB(255, 0, 0);
    CBrush ballBrush(ballColor);
    CBrush* pOldBrush = mDC.SelectObject(&ballBrush);

    int radius = 8 + static_cast<int>(m_mass * 1.25);  // Ensure the radius is an integer
    CRect ballRect(static_cast<int>(x) - radius, static_cast<int>(y) - radius,
        static_cast<int>(x) + radius, static_cast<int>(y) + radius);
    mDC.Ellipse(ballRect);
    mDC.SelectObject(pOldBrush);
}

double CPendulum::CalculateAngularAcceleration()
{
    double airResistance = -m_resistance * m_angularVelocity;
    double gravitationalForce = -m_gravity / m_length * sin(m_angle);
    double massFactor = m_mass * m_length;
    return (gravitationalForce + airResistance) / massFactor;
}

double CPendulum::CalculateXCoordinate(const CRect& rect)
{
    int centerX = rect.Width() / 2;
    x = centerX + m_length * sin(m_angle);
    return x;
}

double CPendulum::CalculateYCoordinate(const CRect& rect)
{
    int centerY = rect.Height() / 2;
    y = centerY + m_length * cos(m_angle);
    return y;
}
