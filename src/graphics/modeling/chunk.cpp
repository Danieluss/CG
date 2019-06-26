#include"chunk.hpp"

#include<iostream>
#include"good_random.hpp"

namespace pr {
    struct chunkEntity {
        std::string name;
        glm::vec3 translation;
        chunkEntity(std::string _name, glm::vec3 _translation) : name(_name), translation(_translation) {};
    };
}

pr::Chunk::Chunk() {

}

pr::Chunk::Chunk(std::unordered_map< std::string, Model > &models, int _type, int x, int y) : type(_type) {
    std::vector<glm::vec3> positions = {
        glm::vec3(20,15,0),
        glm::vec3(chunkSize-20-12.6, 15, 0),
        glm::vec3(20, chunkSize-15-23.1, 0),
        glm::vec3(chunkSize-20-12.6, chunkSize-15-23.1, 0)
    };
    std::vector<std::string> data;
    if(type == 0) {
        data = {
            "building1", "building2", "building3", "building4"
        };
    } else {
        for(int i=0; i < 4; i++) {
            data.push_back("building" + std::to_string(rand()%4+1));
        }
    }
    for(int i=0; i < (int)positions.size(); i++) {
        Entity e = Entity(models[data[i]]);
        e.translate(positions[i]+glm::vec3(10.0, 10.0, 0.0));
        e.rotateD(90, X);
        entities.push_back(e);
    }
    std::vector<chunkEntity> v = {
        chunkEntity("grass", {9.0, 9.0, -0.5}),
        chunkEntity("road1", {0.0, 0.0, -0.5}),
        chunkEntity("road1", {0.0, 84.0, -0.5}),
        chunkEntity("road2", {0.0, 0.0, -0.5}),
        chunkEntity("road2", {84.0, 0.0, -0.5}),
        chunkEntity("pavement1", {6.0, 6.0, -0.1}),
        chunkEntity("pavement1", {6.0, 81.0, -0.1}),
        chunkEntity("pavement2", {6.0, 6.0, -0.1}),
        chunkEntity("pavement2", {81.0, 6.0, -0.1})
    };
    for(auto &ce : v) {
        Entity e = Entity(models[ce.name]);
        e.translate(ce.translation);
        basicEntities.push_back(e);
    }
    translate(glm::vec3(x*chunkSize,y*chunkSize,0));
}

void pr::Chunk::setParents() {
    for(auto &e : entities) {
        e.setParent(*this);
    }
    for(auto &e : basicEntities) {
        e.setParent(*this);
    }
}

void pr::Chunk::draw(Shader &shader) {
    for(auto &e : entities) {
        e.draw(shader);
    }
    drawBasic(shader);
}

void pr::Chunk::drawBasic(Shader &shader) {
    for(auto &e : basicEntities) {
        e.draw(shader);
    }
}