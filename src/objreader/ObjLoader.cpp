//
// Created by abacate on 29/10/2025.
//

#include "ObjLoader.h"
#include <limits>
#include <algorithm>

std::vector<Point3D> ObjLoader::points;
std::vector<Polygon3D> ObjLoader::polygons;

void ObjLoader::translateValues(QTextStream* textStream, GraphicsFrame* frame)
{
    // points.clear();
    // polygons.clear();

    std::vector<Point3D> tempPoints;

    auto wholeModel = std::make_unique<Polygon3D>();
    wholeModel->setName("Modelo Importado");

    while (!textStream->atEnd())
    {
        QString currentLine = textStream->readLine().trimmed();

        //pontos
        if (currentLine.startsWith("v "))
        {
            QStringList tokens = currentLine.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);

            if (tokens.size() >= 4)
            {
                const double rawX = tokens[1].toDouble();
                const double rawY = tokens[2].toDouble();
                const double rawZ = tokens[3].toDouble();

                const double x = rawX;
                const double y = rawY;
                const double z = rawZ;

                tempPoints.push_back(Point3D(x, y, z));
            }
        }

        if (currentLine.startsWith("f "))
        {
            QStringList tokens = currentLine.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
            if (tokens.size() < 4) continue;

            std::vector<int> indices;
            for (int i = 1; i < tokens.size(); i++)
                indices.push_back(tokens[i].split('/').first().toInt());

            for (size_t i = 0; i < indices.size(); i++)
            {
                int idx1 = indices[i] - 1;
                int idx2 = indices[(i + 1) % indices.size()] - 1;

                if (idx1 >= 0 && idx1 < tempPoints.size() && idx2 >= 0 && idx2 < tempPoints.size())
                {
                    wholeModel->addLine(Line3D(tempPoints[idx1], tempPoints[idx2], {255, 255, 255}));
                }
            }
        }
    }

    if (!tempPoints.empty())
    {
        double minX = std::numeric_limits<double>::max();
        double maxX = std::numeric_limits<double>::lowest();
        double minY = minX, maxY = maxX;
        double minZ = minX, maxZ = maxX;

        for (const auto& p : tempPoints)
        {
            double px = p.vector.x();
            double py = p.vector.y();
            double pz = p.vector.z();

            if (px < minX) minX = px;
            if (px > maxX) maxX = px;
            if (py < minY) minY = py;
            if (py > maxY) maxY = py;
            if (pz < minZ) minZ = pz;
            if (pz > maxZ) maxZ = pz;
        }

        double centerX = (minX + maxX) / 2.0;
        double centerY = (minY + maxY) / 2.0;
        double centerZ = (minZ + maxZ) / 2.0;

        double sizeX = maxX - minX;
        double sizeY = maxY - minY;
        double sizeZ = maxZ - minZ;

        double maxDimension = std::max({sizeX, sizeY, sizeZ});

        if (maxDimension == 0) maxDimension = 100.0;

        frame->m_camera.m_vrp = Vector3D(centerX, centerY, centerZ);

        frame->m_camera.m_radius = maxDimension * 1.5;

        frame->m_camera.m_orthoWidth = frame->m_camera.m_radius;

        frame->m_camera.updateEyeFromAngles();



        // auto grid = std::make_unique<Polygon3D>();
        // grid->setName("Grid");
        // Color gridColor = {80, 80, 80};
        //
        // double step = maxDimension / 5.0;
        // if (step < 0.001) step = 1.0;
        // double epsilon = 0.001;
        //
        // auto generateSteps = [&](double min, double max, double s) -> std::vector<double> {
        //     std::vector<double> coords;
        //     for (double v = min; v <= max + epsilon; v += s) {
        //         coords.push_back(v);
        //     }
        //     if (std::abs(coords.back() - max) > epsilon) {
        //         coords.push_back(max);
        //     }
        //     return coords;
        // };
        //
        // std::vector<double> xCoords = generateSteps(minX, maxX, step);
        // std::vector<double> yCoords = generateSteps(minY, maxY, step);
        // std::vector<double> zCoords = generateSteps(minZ, maxZ, step);
        //
        // for (double x : xCoords) {
        //     for (double z : zCoords) {
        //         grid->addLine(Line3D(Point3D(x, minY, z), Point3D(x, maxY, z), gridColor));
        //     }
        // }
        //
        // for (double y : yCoords) {
        //     for (double z : zCoords) {
        //         grid->addLine(Line3D(Point3D(minX, y, z), Point3D(maxX, y, z), gridColor));
        //     }
        // }
        //
        // for (double x : xCoords) {
        //     for (double y : yCoords) {
        //         grid->addLine(Line3D(Point3D(x, y, minZ), Point3D(x, y, maxZ), gridColor));
        //     }
        // }
        //
        // frame->addShape(std::move(grid));
    }

    if (!wholeModel->getCenter().x() && tempPoints.empty()) {
        // Verifica se o modelo está vazio antes de adicionar
    } else {
        frame->addShape(std::move(wholeModel));
    }

    frame->update();
};
