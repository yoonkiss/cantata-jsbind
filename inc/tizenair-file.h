#ifndef __TIZENAIR_FILE__
#define __TIZENAIR_FILE__

#include <string>
#include <v8.h>
//#include <node.h>

#include "tizenair-common.h"

class Files : node::ObjectWrap
{
	public:
		static v8::Persistent<v8::FunctionTemplate> funcTemplate;
		static v8::Persistent<v8::FunctionTemplate> _getAttribute;
		static v8::Persistent<v8::FunctionTemplate> _read;
		static v8::Persistent<v8::FunctionTemplate> _list;
		static v8::Persistent<v8::FunctionTemplate> _createDirectory;
		static v8::Persistent<v8::FunctionTemplate> _remove;
		static v8::Persistent<v8::FunctionTemplate> _moveTo;
		static v8::Persistent<v8::FunctionTemplate> _rename;

		static void NODE_EXTERN Init( v8::Handle<v8::Object> target );
		static v8::Handle<v8::Value> New( const v8::Arguments& args );

		static v8::Handle<v8::Value> getAttribute( const v8::Arguments& args );
		static v8::Handle<v8::Value> read( const v8::Arguments& args );
		static v8::Handle<v8::Value> list( const v8::Arguments& args );
		static v8::Handle<v8::Value> createDirectory( const v8::Arguments& args );
		static v8::Handle<v8::Value> remove( const v8::Arguments& args );
		static v8::Handle<v8::Value> moveTo( const v8::Arguments& args );
		static v8::Handle<v8::Value> rename( const v8::Arguments& args );
		static v8::Persistent<v8::Function> _fs;

	protected:
	        std::string _base;

};

#endif
