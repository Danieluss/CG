#ifndef CG_LOOPER_HPP
#define CG_LOOPER_HPP

#include <vector>
#include "window.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "directionalLight.hpp"

namespace pr {

    class Looper {

        Window& window;
        Shader& shader;
        std::vector<Texture> textures;
        std::vector<DirectionalLight> directionalLights;

        void processInput();

        void render();

        void swap();

        void drawCube(glm::mat4);

    public:

        Looper( Window& window, Shader &shader);

        ~Looper();

        void loop();

    };

}

#endif //CG_LOOPER_HPP
