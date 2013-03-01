#ifndef __TIZENAIR_FS__
#define __TIZENAIR_FS__

#include <v8.h>
#include <node.h>

#include "tizenair-common.h"

class FS : node::ObjectWrap
{
	public:
		static v8::Persistent<v8::FunctionTemplate> funcTemplate;

		static void NODE_EXTERN Init( v8::Handle<v8::Object> target );
		static v8::Handle<v8::Value> New( const v8::Arguments& args );

		static v8::Handle<v8::Value> getPath( const v8::Arguments& args );
		static v8::Handle<v8::Value> getAttribute( const v8::Arguments& args );
		static v8::Handle<v8::Value> list( const v8::Arguments& args );
		static v8::Handle<v8::Value> create( const v8::Arguments& args );
		static v8::Handle<v8::Value> remove( const v8::Arguments& args );
		static v8::Handle<v8::Value> rename( const v8::Arguments& args );
		static v8::Handle<v8::Value> stream( const v8::Arguments& args );

	protected:
		std::string _base;
};

#endif
