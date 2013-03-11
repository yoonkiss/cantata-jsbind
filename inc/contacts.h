#ifndef __CONTACTS__
#define __CONTACTS__

#include <v8.h>
#include <FBase.h>
#include <FContent.h>

#include "tizenair-common.h"

using namespace Tizen::Base;
using namespace Tizen::Content;

class Contacts : node::ObjectWrap
{
    public:
        static void NODE_EXTERN Init( v8::Handle<v8::Object> target );
        static v8::Handle<v8::Value> New(const v8::Arguments& args);
        static v8::Handle<v8::Value> list(const v8::Arguments& args);
        static v8::Handle<v8::Value> addCategory(const v8::Arguments& args);
        static v8::Handle<v8::Value> removeCategory(const v8::Arguments& args);

    private:
};

#endif
