#pragma once

#include <Standard_Type.hxx>
#include <Quantity_Color.hxx>

#include <string>

class AIS_ColoredShape;


class Geometry
{
public:
    // Constructors
    Geometry(const char* name);
    Geometry(const char* name, Handle(AIS_ColoredShape) shape);
    Geometry(const Geometry& other);  // Copy Constructor
    Geometry(Geometry&& other);  // Move Constructor

    // Operators
    Geometry& operator=(const Geometry& other);  // Copy Operator
    Geometry& operator=(Geometry&& other);  // Move Operator
    
    // Getters
    int GetID() const { return m_ID; }
    std::string GetName() const { return m_Name; }
    Quantity_Color GetColor() const { return m_Color; }
    Handle(AIS_ColoredShape) GetShape() const { return m_AISShape; }

    // Setters
    void SetColor(Quantity_Color color);
    void SetShape(Handle(AIS_ColoredShape) shape);

private:
    int m_ID;
    std::string m_Name;
    Handle(AIS_ColoredShape) m_AISShape;
    Quantity_Color m_Color;

    static int s_LastID;
};