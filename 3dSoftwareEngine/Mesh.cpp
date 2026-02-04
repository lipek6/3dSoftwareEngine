#include "Mesh.h"

bool Mesh::LoadFromObjectFile(std::string sFilename)
{
    std::ifstream file(sFilename);

    if (!file.is_open())
    {
        std::cout << "Error loading '.obj file' - Unable to open it" << std::endl;
        return false;
    }

    vTriangle.clear();

    std::vector<Vec3d> vVertex;     

    while (!file.eof())
    {
        char line[MAX_INLINE_CHARS_OBJFILE];
        file.getline(line, MAX_INLINE_CHARS_OBJFILE);

        std::stringstream lineStream; lineStream << line;

        if (line[0] == 'v' && line[1] == ' ')
        {
            char dataType;
            Vec3d vertex;

            lineStream >> dataType >> vertex.x >> vertex.y >> vertex.z;
            vVertex.push_back(vertex);
        }
        else if (line[0] == 'f' && line[1] == ' ')
        {
            char dataType;
            size_t idx0, idx1, idx2;

            lineStream >> dataType >> idx0 >> idx1 >> idx2;
            vTriangle.push_back({ vVertex[idx0 - 1], vVertex[idx1 - 1], vVertex[idx2 - 1] });
        }
    }

    file.close();
    std::cout << "'.obj file' Loaded Sucessfully" << std::endl;
    return true;

}