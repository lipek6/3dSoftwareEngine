#include "Mesh.h"

bool Mesh::LoadFromObjectFile(std::string sFilename, bool bHasTexture)
{
    std::ifstream file(sFilename);

    if (!file.is_open())
    {
        std::cout << sFilename <<" - Unable to open it" << std::endl;
        return false;
    }
    else
    {
        std::cout << sFilename << " - File open" << std::endl;
    }

    vTriangle.clear();

    std::vector<Vec3d> vVertex;
    std::vector<Vec2d> vTexture;

    while (!file.eof())
    {
        char line[MAX_INLINE_CHARS_OBJFILE];
        file.getline(line, MAX_INLINE_CHARS_OBJFILE);

        std::stringstream lineStream; 
        lineStream << line;

        char junkData;

        if (line[0] == 'v')
        {
            if (line[1] == 't')                                                 // TEXTURE
            {
                Vec2d texture;
                lineStream >> junkData >> junkData >> texture.u >> texture.v;
                // A little hack for the spyro texture
                texture.u = 1.0f - texture.u;
                texture.v = 1.0f - texture.v;
                vTexture.push_back(texture);
            }
            else if (line[1] == ' ')                                            // VERTEX
            {
                Vec3d vertex;
                lineStream >> junkData >> vertex.x >> vertex.y >> vertex.z;
                vVertex.push_back(vertex);
            }
        }

        else if (line[0] == 'f')                                                
        {
            if (bHasTexture)                                                    // FACE + TEXTURE
            {
                lineStream >> junkData;
                
                std::string tokens[6];
                int nTokenCount = -1;

                while (!lineStream.eof())
                {
                    char c = lineStream.get();
                    if (c == ' ' || c == '/')
                        nTokenCount++;
                    else
                        tokens[nTokenCount].append(1, c);
                }

                tokens[nTokenCount].pop_back();
                vTriangle.push_back({ vVertex[stoi(tokens[0]) - 1], vVertex[stoi(tokens[2]) - 1], vVertex[stoi(tokens[4]) - 1], vTexture[stoi(tokens[1]) - 1], vTexture[stoi(tokens[3]) - 1], vTexture[stoi(tokens[5]) - 1] });
            }
            else                                                                // FACE
            {
                int vertex[3];
                lineStream >> junkData >> vertex[0] >> vertex[1] >> vertex[2];
                vTriangle.push_back({ vVertex[vertex[0] - 1], vVertex[vertex[1] - 1], vVertex[vertex[2] - 1] });
            }
        }
    }

    file.close();
    std::cout << sFilename << " - Loaded Sucessfully" << std::endl;
    return true;

}