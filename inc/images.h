#ifndef __IMAGES__
#define __IMAGES__

#include <v8.h>
//#include <node.h>

#include "tizenair-common.h"

class Images : node::ObjectWrap
{
	public:
		static void NODE_EXTERN Init( v8::Handle<v8::Object> target );
		static v8::Handle<v8::Value> New( const v8::Arguments& args );
		static v8::Handle<v8::Value> list( const v8::Arguments& args );
		static v8::Handle<v8::Value> getPath( const v8::Arguments& args );
};

#endif
