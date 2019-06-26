#ifndef CG_CHUNK_HPP
#define CG_CHUNK_HPP

#include<vector>
#include"entities/basic_entities.hpp"

namespace pr {
    class Chunk : public Entity {
        int type;
        std::vector<Entity> entities;
        std::vector<Entity> basicEntities;
        public:
        static constexpr float chunkSize = 90.f;
        Chunk();
        Chunk(std::unordered_map< std::string, Model > &models, int type = 0, int x = 0, int y = 0);
        void draw(Shader &shader);
        void drawBasic(Shader &shader);
        void setParents();
    };
}

#endif