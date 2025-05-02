#pragma once

#include <string>
#include <vector>

struct Vec2 {
    float x, y;
};

struct Vec3 {
    float x, y, z;
};

struct ObjVertex {
    Vec3 position;
    Vec3 normal;
    Vec2 texCoord;
};

class ObjectLoader {
public:
    static ObjectLoader& getInstance();
    std::vector<ObjVertex> loadOBJ(const std::string& filename);

private:
    ObjectLoader() {}
    ObjectLoader(const ObjectLoader&) = delete;
    ObjectLoader& operator=(const ObjectLoader&) = delete;
};