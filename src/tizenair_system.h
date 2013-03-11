#ifndef __TIZENAIR_SYSTEM__
#define __TIZENAIR_SYSTEM__

#include <v8.h>
//#include <node.h>

#include "tizenair-common.h"

class System : node::ObjectWrap
{
	public:
		static void NODE_EXTERN Init( v8::Handle<v8::Object> target );
		static v8::Handle<v8::Value> New( const v8::Arguments& args );
		static v8::Handle<v8::Value> getStorage( const v8::Arguments& args );
		static v8::Handle<v8::Value> getMemoryInfo( const v8::Arguments& args );
		static v8::Handle<v8::Value> getPhoneNumber( const v8::Arguments& args );
		static v8::Handle<v8::Value> getCPUUsages( const v8::Arguments& args );
};

#endif
