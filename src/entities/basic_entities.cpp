#include <limits>
#include <cmath>
#include "basic_entities.hpp"
#include "good_random.hpp"

bool pr::approx( double a, double b ) {
    double c = std::numeric_limits<float>::min() * 5;
    return abs( a - b ) < c;
}

pr::Entity::Entity() {

}

pr::Entity::Entity( pr::Model &model ) : model( &model ) {

}

pr::Model *pr::Entity::getModel() const {
    return model;
}

glm::mat4 pr::Entity::modelMatrix() const {
    glm::mat4 Mtmp = M;
    // std::cout << this << " " << parent << std::endl;
    if( parent != nullptr ) {
        Mtmp = M*parent->modelMatrix();
    }
    return Scalable::transform( Rotatable::transform( Positionable::transform( Mtmp )));
}

void pr::Entity::draw( pr::Shader &shader ) const {
    glm::mat4 M = modelMatrix();
    shader.setUniform( "M", M );
    glm::mat3 normalMatrix = glm::transpose( glm::inverse( glm::mat3( M )));
    shader.setUniform( "normalMatrix", normalMatrix );
    model->draw( shader );
}


bool pr::Entity::operator==( const pr::Entity &entity ) const {
    return &entity == this;
}

namespace std {
    size_t hash< pr::Entity >::operator()( const pr::Entity &obj ) const {
        auto addr = reinterpret_cast<uintptr_t>( &obj );
#if SIZE_MAX < UINTPTR_MAX
        addr %= SIZE_MAX;
#endif
        return addr;
    }
}

std::function< glm::vec3( float ) > pr::constTranslation( glm::vec3 vector ) {
    return
            [ = ]( float time ) -> glm::vec3 {
                return vector;
            };
}

std::function< float( float ) > pr::randomTransition() {
    return
            [ = ]( float time ) -> float {
                static float acc;
                acc += GoodRandom::rng.get()*time;
                return acc;
            };
}

std::function< float( float ) > pr::constTransition( float a, float expiration ) {
    return
            [ = ]( float time ) -> float {
                if( time < expiration )
                    return a;
                return 0;
            };
}

std::function< float( float ) > pr::posSquareTransition( float a, float expiration ) {
    return
            [ = ]( float time ) -> float {
                return time < expiration ? a*( pow( time - expiration/2, 2 ) - pow( expiration, 2 )/4 ) : 0;
            };
}

std::function< float( float ) > pr::negSquareTransition( float a, float expiration ) {
    return
            [ = ]( float time ) -> float {
                return time < expiration ? -a*( -pow( time - expiration/2, 2 ) + pow( expiration, 2 )/4 ) : 0;
            };
}

std::function< float( float ) > pr::linearTransition( float expiration, float a, float b ) {
    return
            [ = ]( float time ) -> float {
                float w = time / expiration;
                w = w < 1 ? w : 1;
                return b * w + ( 1 - w ) * a;
            };
}

std::function< float( float ) > pr::sinTransition( float expiration, float power ) {
    return
            [ = ]( float time ) -> float {
                return time < expiration ? pow( sin( time*M_PI/expiration ), power ) : 0;
            };
}

std::function< float( float ) > pr::cosTransition( float expiration, float power ) {
    return
            [ = ]( float time ) -> float {
                std::cout<<pow( cos( time*M_PI/2/expiration ), power)<<std::endl;
                return time < expiration ? pow( cos( time*M_PI/2/expiration ), power ) : 0;
            };
}

std::function< glm::vec3( float ) > pr::linearTranslation( glm::vec3 vector, glm::vec3 velocity ) {
    return
            [ = ]( float time ) -> glm::vec3 {
                return vector + time * velocity;
            };
}

std::function< glm::vec3( float ) >
pr::acceleratedTranslation( glm::vec3 vector, glm::vec3 velocity0, glm::vec3 acceleration ) {
    return
            [ = ]( float time ) -> glm::vec3 {
                return vector + time * velocity0 + ( static_cast<float>( pow( time, 2 ) ) ) * acceleration * 0.5f;
            };
}

std::function< float( float ) > pr::neverFade() {
    return
            [ = ]( float time ) -> float {
                return 1;
            };
}

bool pr::Particle::update( float time ) {
    timeAcc += time;
    position = translation( timeAcc );
    rot2d = rotation( timeAcc );
    alpha = fade( timeAcc );
    scale = scaling( timeAcc );
    return approx( alpha, 0 );
}
