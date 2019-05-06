#ifndef CG_ENUM_TO_STRING_HPP
#define CG_ENUM_TO_STRING_HPP

#ifndef ENUM_STRINGS
    #define Elem( element ) element
    #define BegEnum( enumName ) typedef enum tag##enumName
    #define EndEnum( enumName ) enumName; \
            char* str##enumName(enum tag##enumName index);
#else
    #define Elem( element ) #element
    #define BegEnum( enumName ) char* gs_##enumName [] =
    #define EndEnum( enumName ) ; char* str##enumName(enum \
            tag##enumName index){ return gs_##enumName [index]; }
#endif

#endif //CG_ENUM_TO_STRING_HPP
