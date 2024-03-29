#ifndef MODEL_HPP
#define MODEL_HPP

#include <string>
#include <vector>
#include <glm/glm.hpp>

enum FaceIndex
{
    VERTEX = 0,
    UV = 1,
    NORMAL = 2
};

struct Model
{
    Model(const std::string &obj_path);
    void draw() const;

    unsigned int VAO;
    std::vector<glm::vec3> vertices, normals;
    std::vector<glm::vec2> uvs;
    std::vector<std::vector<glm::vec3>> faces;

private:
    void load_model(const std::string &obj_path);
    void set_up_VAO();
};

#endif