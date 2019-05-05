#ifndef CG_MIN_MAX_PROPERTY_HPP
#define CG_MIN_MAX_PROPERTY_HPP

template< class T >
struct MinMaxProperty{

    T min, max;
    T value;

    MinMaxProperty( T min, T max ) {
        this->min = min;
        this->max = max;
    }

    MinMaxProperty( T min, T max, T def ) {
        this->min = min;
        this->max = max;
        this->operator=( def );
    }

    void operator=( const T& value ) {
        if( value <= min ) {
            this->value = min;
        } else if( min < value && value < max ) {
            this->value = value;
        } else {
            this->value = max;
        }
    }

    operator T&() {
        return value;
    }

};

#endif //CG_MIN_MAX_PROPERTY_HPP
