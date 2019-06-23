#ifndef CG_BASIC_ENTITIES_HPP
#define CG_BASIC_ENTITIES_HPP

#include <functional>
#include "graphics/modeling/model.hpp"
#include "object_properties.hpp"

namespace pr {

    bool approx( double a, double b );

    class Entity : public Inertiable, public Rotatable, public Scalable, public Parentable {

        Model *model;

    public:

        glm::mat4 M = glm::mat4( 1 );

        Entity();

        Entity( Model &model );


        glm::mat4 modelMatrix() const;

        Model *getModel() const;

        void draw( Shader &shader ) const;

        bool operator==( const Entity & ) const;

    };

    std::function< float( float ) > neverFade();

    std::function< float( float ) > cosTransition( float expiration, float power );

    std::function< float( float ) > sinTransition( float expiration, float power );

    std::function< float( float ) > linearTransition( float expiration, float a = 0, float b = 1 );

    std::function< float( float ) > negSquareTransition( float a, float expiration );

    std::function< float( float ) > posSquareTransition( float a, float expiration );

    std::function< float( float ) > constTransition( float a, float expiration );

    std::function< float( float ) > randomTransition();

    std::function< glm::vec3( float ) > constTranslation( glm::vec3 vector );

    std::function< glm::vec3( float ) > linearTranslation( glm::vec3 vector, glm::vec3 velocity );

    std::function< glm::vec3( float ) >
    acceleratedTranslation( glm::vec3 vector, glm::vec3 velocity0, glm::vec3 acceleration );

    struct Particle {

        float timeAcc = 0;
        glm::vec3 scale = {1, 1, 1};
        float rot2d = 0;
        float alpha = 1;
        glm::vec3 position;
        std::function< float( float ) > fade = linearTransition( 10, 1, 0 );
        std::function< glm::vec3( float ) > translation = constTranslation( {0, 0, 0} );
        std::function< float( float ) > rotation = constTransition( 0, 0 );
        Texture *texture;

        bool update( float time );

    };

}

namespace std {
    template<>
    struct hash< pr::Entity > {
        size_t operator()( const pr::Entity &obj ) const;
    };
}
#endif //CG_BASIC_ENTITIES_HPP
