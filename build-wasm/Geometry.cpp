#include "Geometry.hpp"

#include <AIS_ColoredShape.hxx>


int Geometry::s_LastID = 0;

Geometry::Geometry(const char* name)
    : m_ID(s_LastID++), m_Name(name), m_AISShape(nullptr)
{
}

Geometry::Geometry(const char* name, Handle(AIS_ColoredShape) shape)
    : m_ID(s_LastID++), m_Name(name), m_AISShape(shape)
{
}

Geometry::Geometry(const Geometry& other)
    : m_ID(other.m_ID), m_Name(other.m_Name), m_AISShape(other.m_AISShape)
{
}

Geometry::Geometry(Geometry&& other)
    : m_ID(other.m_ID), m_Name(std::move(other.m_Name)), m_AISShape(other.m_AISShape)
{
    other.m_ID = -1;
    other.m_AISShape = nullptr;
}

Geometry& Geometry::operator=(const Geometry& other)
{
    m_ID = other.m_ID;
    m_Name = other.m_Name;
    m_AISShape = other.m_AISShape;
}

Geometry& Geometry::operator=(Geometry&& other)
{
    m_ID = other.m_ID;
    m_Name = std::move(other.m_Name);
    m_AISShape = other.m_AISShape;

    other.m_ID = -1;
    other.m_Name.clear();
    other.m_AISShape = nullptr;
}

void Geometry::SetColor(Quantity_Color color)
{
    m_Color = color;
}

void Geometry::SetShape(Handle(AIS_ColoredShape) shape)
{
    m_AISShape = shape;
}