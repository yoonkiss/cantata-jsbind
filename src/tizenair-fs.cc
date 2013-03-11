#include <iostream>
#include <fstream>
#include <v8.h>
#include <node.h>
#include <sys/stat.h>
#include "tizenair-util.h"
#include "tizenair-fileattr.h"
#include "tizenair-fs.h"

v8::Persistent<v8::FunctionTemplate> FS::funcTemplate;

void NODE_EXTERN FS::Init( v8::Handle<v8::Object> target )
{
	v8::HandleScope scope;

	FS::funcTemplate = v8::Persistent<v8::FunctionTemplate>::New( v8::FunctionTemplate::New( FS::New ) );
	FS::funcTemplate->InstanceTemplate() -> SetInternalFieldCount( 1 );

	FS::funcTemplate->SetClassName( v8::String::NewSymbol( "FS" ) );

	NODE_SET_PROTOTYPE_METHOD( funcTemplate, "getAttribute", getAttribute );
	NODE_SET_PROTOTYPE_METHOD( funcTemplate, "getPath", getPath );
	/*
	NODE_SET_PROTOTYPE_METHOD( funcTemplate, "list", list );
	NODE_SET_PROTOTYPE_METHOD( funcTemplate, "create", create );
	NODE_SET_PROTOTYPE_METHOD( funcTemplate, "remove", remove );
	NODE_SET_PROTOTYPE_METHOD( funcTemplate, "rename", rename );
	NODE_SET_PROTOTYPE_METHOD( funcTemplate, "stream", stream );
	*/

	target -> Set( v8::String::NewSymbol( "FS" ), FS::funcTemplate -> GetFunction() );
}

v8::Handle<v8::Value> FS::New( const v8::Arguments& args )
{
	v8::HandleScope scope;
	FS* instance = new FS();

	if ( 0 < args.Length() ) {
		instance->_base = UNWRAP_STRING( args[0] );
	}

	instance->Wrap( args.This() );
	return args.This();
}


v8::Handle<v8::Value> FS::getPath( const v8::Arguments& args )
{
	v8::HandleScope scope;
	if ( args.Length() < 1 ) {
		return v8::Undefined();
	}
	FS* instance = FS::Unwrap<FS>( args.This() );
	v8::Handle<v8::Value> attrArgs[2] = { WRAP_STRING( instance->_base ), args[0] };
	return v8::Local<v8::Function>::Cast( Util::constructor->Get( v8::String::NewSymbol( "addPath" ) ) )->Call( v8::Context::GetCurrent()->Global(), 2, attrArgs );
}

v8::Handle<v8::Value> FS::getAttribute( const v8::Arguments& args )
{
	
	v8::HandleScope scope;

	FS* instance = FS::Unwrap<FS>( args.This() );
	
	v8::Local<v8::Object> arg = v8::Object::New();

	struct stat info;

	v8::Handle<v8::Value> path = getPath( args );

	if ( 0 == lstat( UNWRAP_STRING( path ).c_str(),  &info ) ) {
		arg->Set( v8::String::NewSymbol( "type" ), WRAP_STRING( std::string( S_ISDIR(info.st_mode)?"d":"f" ) ) );

		v8::Handle<v8::Value> subArgs[1] = { path };
		v8::Handle<v8::Value> name = v8::Local<v8::Function>::Cast( Util::constructor->Get( v8::String::NewSymbol( "getFilenameFrom" ) ) )->Call( v8::Context::GetCurrent()->Global(), 1, subArgs );
		arg->Set( v8::String::NewSymbol( "name" ), name );
		arg->Set( v8::String::NewSymbol( "path" ), path );
		arg->Set( v8::String::NewSymbol( "size" ), v8::Integer::New( info.st_size ) );
	}

	v8::Handle<v8::Value> attrArgs[1] = { arg };

	v8::Local<v8::Object> ret = FileAttribute::constructor->NewInstance( 1, attrArgs );

	return scope.Close( ret );
}
v8::Handle<v8::Value> FS::list( const v8::Arguments& args )
{
	return v8::Undefined();
}
v8::Handle<v8::Value> FS::create( const v8::Arguments& args )
{
	return v8::Undefined();
}
v8::Handle<v8::Value> FS::remove( const v8::Arguments& args )
{
	return v8::Undefined();
}
v8::Handle<v8::Value> FS::rename( const v8::Arguments& args )
{
	return v8::Undefined();
}
v8::Handle<v8::Value> FS::stream( const v8::Arguments& args )
{
	return v8::Undefined();
}
