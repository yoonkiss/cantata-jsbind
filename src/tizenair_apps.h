#ifndef __TIZENAIR_APPS__
#define __TIZENAIR_APPS__

#include <v8.h>

#include "tizenair-common.h"

class Apps : node::ObjectWrap
{
    public:
        static void NODE_EXTERN Init( v8::Handle<v8::Object> target );
        static v8::Handle<v8::Value> New( const v8::Arguments& args );
        static v8::Handle<v8::Value> getAppsInfoList( const v8::Arguments& args );
};

#endif
