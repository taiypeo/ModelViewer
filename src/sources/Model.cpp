#include "../headers/Model.hpp"

#include <glad/glad.h>

#include <cstdlib>
#include <fstream>
#include <sstream>
#include <stdexcept>

void Model::load_model(const std::string &obj_path)
{
    std::ifstream file(obj_path);
    if (!file)
        throw std::runtime_error("Failed to open the model file");

    std::string line;
    while(!file.eof())
    {
        std::getline(file, line);
        std::stringstream ss(line);

        std::string trash;
        ss >> trash; // gets rid of v/vn/vt/f

        if (line.compare(0, 2, "v ") == 0)
        {
            glm::vec3 vec;
            for (size_t i = 0; i < 3; i++)
                ss >> vec[i];

            vertices.push_back(vec);
        }
        else if (line.compare(0, 3, "vn ") == 0)
        {
            glm::vec3 vec;
            for (size_t i = 0; i < 3; i++)
                ss >> vec[i];

            normals.push_back(vec);
        }
        else if (line.compare(0, 3, "vt ") == 0)
        {
            glm::vec2 vec;
            for (size_t i = 0; i < 2; i++)
                ss >> vec[i];

            uvs.push_back(vec);
        }
        else if (line.compare(0, 2, "f ") == 0)
        {
            char c_trash; // gets rid of slashes in f

            std::vector<glm::vec3> f;
            glm::vec3 vec;
            while(ss >> vec[0] >> c_trash >> vec[1] >> c_trash >> vec[2])
            {
                for (size_t i = 0; i < 3; i++)
                    vec[i]--;

                f.push_back(vec);
            }

            faces.push_back(f);
        }
    }
}

void Model::set_up_VAO()
{
    std::vector<float> vertex_data;
    for (size_t iface = 0; iface < faces.size(); iface++)
    {
        for (size_t ivertex = 0; ivertex < 3; ivertex++)
        {
            glm::vec3 vertex = vertices[faces[iface][ivertex][VERTEX]];
            for (size_t i = 0; i < 3; i++)
                vertex_data.push_back(vertex[i]);

            glm::vec2 uv = uvs[faces[iface][ivertex][UV]];
            for (size_t i = 0; i < 2; i++)
                vertex_data.push_back(uv[i]);
        }
    }

    const float *data = vertex_data.data();
    size_t size = vertex_data.size() * sizeof(float);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); // position
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(sizeof(float) * 3)); // uv
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Model::Model(const std::string &obj_path)
{
    load_model(obj_path);
    set_up_VAO();
}

void Model::draw() const
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, faces.size() * 3);
    glBindVertexArray(0);
}