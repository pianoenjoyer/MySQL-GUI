#pragma once
#include "pch.h"
#include "CPendulum.h" 

void CPendulum::CalculateKineticEnergy()
{
    double velocity = m_angularVelocity * m_length * 200;
    m_kineticEnergy = 0.5 * (m_mass * velocity * velocity);
}

void CPendulum::CalculatePotentialEnergy()
{
    double height = m_length * (1 - cos(m_angle * M_PI / 180.0)) * 10000;
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

double CPendulum::GetMass() { return m_mass; }
double CPendulum::GetLength() { return m_length; }
double CPendulum::GetAngle() { return m_angle; }
double CPendulum::GetGravity() { return m_gravity; }
double CPendulum::GetResistance() { return m_resistance; }

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


void CPendulum::DrawGraph(CDC& dc, const CRect& rect)
{
    dc.FillSolidRect(rect, RGB(255, 255, 255));

    // Определить границы графика
    int graphWidth = rect.Width();
    int graphHeight = rect.Height();
    int offsetX = rect.left;
    int offsetY = rect.top + graphHeight / 2;
    int maxX = graphWidth - 1;
    int maxY = graphHeight / 2;

    // Рисование осей графика
    dc.MoveTo(offsetX, offsetY);
    dc.LineTo(offsetX + maxX, offsetY);
    dc.MoveTo(offsetX, offsetY + maxY);
    dc.LineTo(offsetX, offsetY - maxY);

    // Построение графика синусоиды координаты маятника
    double amplitude = maxY; // Амплитуда синусоиды, используем максимальную высоту графика
    double period = maxX; // Период синусоиды, используем максимальную ширину графика
    double step = period / maxX; // Шаг изменения времени на графике

    // Получаем текущие координаты маятника
    double currentX = 0.0; // Предположим, что у вас есть переменная, содержащая текущее время
    double currentY = offsetY - amplitude * sin(currentX / period);

    // Сохраняем предыдущие координаты
    static double prevX = 0.0;
    static double prevY = currentY;

    // Рисуем линию от предыдущих координат к текущим
    dc.MoveTo(offsetX + prevX, prevY);
    dc.LineTo(offsetX + currentX, currentY);

    // Обновляем предыдущие координаты
    prevX = currentX;
    prevY = currentY;
}


void CPendulum::DrawPendulum(CDC& dc, const CRect& rect)
{
    dc.FillSolidRect(rect, RGB(255, 255, 255));
    double x = CalculateXCoordinate(rect);
    double y = CalculateYCoordinate(rect);

    int centerX = rect.Width() / 2;
    int centerY = rect.Height() / 2;
    // Рисование нити
    dc.MoveTo(centerX, centerY);
    dc.LineTo(static_cast<int>(x), static_cast<int>(y));

    // Рисование опоры маятника
    int basementLenght = 40;
    dc.MoveTo(centerX, centerY);
    dc.LineTo(centerX, rect.bottom);
    dc.LineTo(centerX + basementLenght, rect.bottom);
    dc.LineTo(centerX - basementLenght, rect.bottom);

    COLORREF ballColor = RGB(255, 0, 0);
    CBrush ballBrush(ballColor);
    CBrush* pOldBrush = dc.SelectObject(&ballBrush);

    // Рисование шарика на конце нитки
    int radius = 8 + m_mass * 1.25;
    CRect ballRect(static_cast<int>(x) - radius, static_cast<int>(y) - radius,
        static_cast<int>(x) +
        radius, static_cast<int>(y) + radius);
    dc.Ellipse(ballRect);
    dc.SelectObject(pOldBrush);
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
    return x;
}

double CPendulum::CalculateYCoordinate(const CRect& rect)
{
    int centerY = rect.Height() / 2;
    y = centerY + m_length * scale * cos(m_angle);
    return y;
}
