#ifndef __TIZENAIR_FILEATTR__
#define __TIZENAIR_FILEATTR__

#include <string>
#include <v8.h>
#include <node.h>


#include "tizenair-common.h"

class FileAttribute : node::ObjectWrap
{
	public:
		static v8::Persistent<v8::Function> constructor;

		static void NODE_EXTERN Init( v8::Handle<v8::Object> target );
		static v8::Handle<v8::Value> New( const v8::Arguments& args );

		static v8::Handle<v8::Value> getPath( const v8::Arguments& args );
		static v8::Handle<v8::Value> exists( const v8::Arguments& args );
		static v8::Handle<v8::Value> isDirectory( const v8::Arguments& args );
		static v8::Handle<v8::Value> isFile( const v8::Arguments& args );
		static v8::Handle<v8::Value> getName( const v8::Arguments& args );

		static v8::Handle<v8::Value> getSize( v8::Local<v8::String> property, const v8::AccessorInfo& info );
		static void setSize( v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::AccessorInfo& info );
		
	protected:
		std::string _type;
		std::string _name;
		std::string _path;
		size_t _size;

};

#endif
