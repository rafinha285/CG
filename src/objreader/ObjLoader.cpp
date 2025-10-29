//
// Created by abacate on 29/10/2025.
//

#include "ObjLoader.h"

std::vector<Point3D> ObjLoader::points;
std::vector<Polygon3D> ObjLoader::polygons;

void ObjLoader::translateValues(QTextStream* textStream, GraphicsFrame* frame)
{
    while (!textStream->atEnd())
    {
        QString currentLine = textStream->readLine().trimmed();

        //pontos
        if (currentLine.startsWith("v "))
        {
            QStringList tokens = currentLine.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);

            if (tokens.size() >= 4)
            {
                const double x = tokens[1].toDouble();
                const double y = tokens[2].toDouble();
                const double z = tokens[3].toDouble();

                points.push_back(Point3D(x, y, z));
            }
        }

        if (currentLine.startsWith("f "))
        {
            QStringList tokens = currentLine.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);

            // --- Correção do Bug 1 ---
            // Pula se for uma linha ou ponto (precisa de pelo menos 3 vértices)
            if (tokens.size() < 4)
            {
                continue;
            }

            // Cria um novo polígono para esta face
            Polygon3D currentFace;
            currentFace.setColor({255,255,255}); // Define a cor do polígono

            std::vector<int> indices;

            // --- Correção do Bug 2 ---
            // Loop começa em 1 para pular a letra "f"
            for (int i = 1; i < tokens.size(); i++)
            {
                // Pega o primeiro número (índice do vértice)
                // (Ex: "1/2/3" -> "1")
                QString vertexIndexStr = tokens[i].split('/').first();
                indices.push_back(vertexIndexStr.toInt());
            }

            // Cria as linhas (arestas) da face
            for (size_t i = 0; i < indices.size(); i++)
            {
                // Pega o índice atual e o próximo (ou o primeiro, para fechar a face)
                int index1 = indices[i];
                int index2 = indices[(i + 1) % indices.size()];

                // Converte de 1-based (OBJ) para 0-based (vector)
                int p1_idx = index1 - 1;
                int p2_idx = index2 - 1;

                // Checagem de segurança
                if (p1_idx < 0 || p1_idx >= points.size() ||
                    p2_idx < 0 || p2_idx >= points.size())
                {
                    qDebug() << "Erro: Índice de face inválido encontrado!";
                    continue;
                }

                // Pega os pontos reais
                Point3D p1 = points[p1_idx];
                Point3D p2 = points[p2_idx];

                // --- 3. FINALIZANDO A LÓGICA ---
                // Adiciona a linha ao polígono da face
                currentFace.addLine(Line3D(p1, p2, {255,255,255}));
            }
            polygons.push_back(currentFace);
        }
    }
    for (Polygon3D polygon : polygons)
    {
        frame->addShape(std::make_unique<Polygon3D>(polygon));
    }
    frame->update();
};
