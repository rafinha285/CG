//
// Created by abacate on 29/10/2025.
//

#ifndef CG_OBJLOADER_H
#define CG_OBJLOADER_H
#include <string>
#include <vector>

#include "../../GraphicsFrame.h"
#include "../3d/shapes/Line3D.h"

#include <QRegularExpression>

#include "../3d/shapes/Polygon3D.h"

class ObjLoader
{
private:
    std::string currentLine;
    static std::vector<Point3D> points;
    static std::vector<Polygon3D> polygons;
public:
    static void translateValues(QTextStream* textStream, GraphicsFrame* frame);
};


#endif //CG_OBJLOADER_H