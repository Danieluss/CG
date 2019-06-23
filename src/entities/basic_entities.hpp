#ifndef CG_BASIC_ENTITIES_HPP
#define CG_BASIC_ENTITIES_HPP

#include <functional>
#include "good_random.hpp"
#include "graphics/modeling/model.hpp"
#include "object_properties.hpp"

namespace pr {

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

    struct Particle : public Inertiable, public Scalable {

        static std::function< float( float ) > cosTransition( float expiration, float power ) {
            return
                [ = ]( float time ) -> float {
                    return time < expiration ? pow( cos( time * M_PI / 2 / expiration ), power ) : 0;
                };
        }

        static std::function< float( float ) > sinTransition( float expiration, float power ) {
            return
                [ = ]( float time ) -> float {
                    return time < expiration ? pow( sin( time * M_PI / expiration ), power ) : 0;
                };
        }

        static std::function< float( float ) > negLinearTransition( float expiration ) {
            return
                [ = ]( float time ) -> float {
                    return time < expiration ? ( expiration - time ) / expiration : 0;
                };
        }

        static std::function< float( float ) > posLinearTransition( float expiration ) {
            return
                    [ = ]( float time ) -> float {
                        return time < expiration ? ( time ) / expiration : 1;
                    };
        }

        static std::function< float( float ) > negSquareTransition( float a, float expiration ) {
            return
                [ = ]( float time ) -> float {
                    return time < expiration ? - a * ( - pow( time - expiration / 2, 2 ) + pow( expiration, 2 ) / 4 ) : 0;
                };
        }

        static std::function< float( float ) > posSquareTransition( float a, float expiration ) {
            return
                [ = ]( float time ) -> float {
                    return time < expiration ? a * ( pow( time - expiration / 2, 2 ) - pow( expiration, 2 ) / 4 ) : 0;
                };
        }

        static std::function< float( float ) > randomTransition( float expiration ) {
            return
                    [ = ]( float time ) -> float {
                        static float acc;
                        acc += RNG.get() * time;
                        return acc;
                    };
        }

        float timeStamp = -1;
        std::function< float( float ) > fade;

    };

}

namespace std {
    template<>
    struct hash< pr::Entity > {
        size_t operator()( const pr::Entity &obj ) const;
    };
}
#endif //CG_BASIC_ENTITIES_HPP
