#include <string>
#include <stdio.h>
#include <v8.h>
#include <node.h>
#include "tizenair-fileattr.h"

v8::Persistent<v8::FunctionTemplate> FileAttribute::funcTemplate;

void NODE_EXTERN FileAttribute::Init( v8::Handle<v8::Object> target )
{
	v8::HandleScope scope;

	funcTemplate = v8::Persistent<v8::FunctionTemplate>::New( v8::FunctionTemplate::New( New ) );
	funcTemplate->InstanceTemplate() -> SetInternalFieldCount( 1 );

	// 클래스 명 할당( Contacts = function() {}; )
	funcTemplate->SetClassName( v8::String::NewSymbol( "FileAttribute" ) );

	NODE_SET_PROTOTYPE_METHOD( funcTemplate, "getPath", getPath );
	NODE_SET_PROTOTYPE_METHOD( funcTemplate, "getName", getName );
	NODE_SET_PROTOTYPE_METHOD( funcTemplate, "exists", exists );
	NODE_SET_PROTOTYPE_METHOD( funcTemplate, "isDirectory", isDirectory );
	NODE_SET_PROTOTYPE_METHOD( funcTemplate, "isFile", isFile );


	target -> Set( v8::String::NewSymbol( "FileAttribute" ), funcTemplate -> GetFunction() );
}

v8::Handle<v8::Value> FileAttribute::New( const v8::Arguments& args )
{
	v8::HandleScope scope;
	FileAttribute* instance = new FileAttribute();

	if ( 0 < args.Length() ) {
		v8::Local<v8::Object> attr = v8::Local<v8::Object>::Cast( args[0] );
		instance->_type = UNWRAP_STRING( attr->Get( v8::String::NewSymbol( "type" ) ) );
		instance->_name = UNWRAP_STRING( attr->Get( v8::String::NewSymbol( "name" ) ) );
		instance->_path = UNWRAP_STRING( attr->Get( v8::String::NewSymbol( "path" ) ) );
		instance->_size = v8::Local<v8::Integer>::Cast( attr->Get( v8::String::NewSymbol( "size" ) ) )->Value();
	}

	args.This()->SetAccessor( v8::String::New( "size" ), FileAttribute::getSize, FileAttribute::setSize );
	instance -> Wrap( args.This() );

	return args.This();
}

FileAttribute::~FileAttribute() {
	TRY_DELETE( this->_type );
	TRY_DELETE( this->_name );
	TRY_DELETE( this->_path );
}

v8::Handle<v8::Value> FileAttribute::getPath( const v8::Arguments& args )
{
	v8::HandleScope scope;

	FileAttribute* instance = FileAttribute::Unwrap<FileAttribute>( args.This() );

	return scope.Close( WRAP_STRING( instance->_path ) );
}

v8::Handle<v8::Value> FileAttribute::exists( const v8::Arguments& args )
{
	v8::HandleScope scope;

	FileAttribute* instance = FileAttribute::Unwrap<FileAttribute>( args.This() );

	return scope.Close( v8::Boolean::New( instance->_type != NULL ) );
}

v8::Handle<v8::Value> FileAttribute::isDirectory( const v8::Arguments& args )
{
	v8::HandleScope scope;

	FileAttribute* instance = FileAttribute::Unwrap<FileAttribute>( args.This() );

	return v8::Boolean::New( NULL!= instance->_type && "d" == *( instance->_type ) );
}

v8::Handle<v8::Value> FileAttribute::isFile( const v8::Arguments& args )
{
	v8::HandleScope scope;

	FileAttribute* instance = FileAttribute::Unwrap<FileAttribute>( args.This() );

	return v8::Boolean::New( NULL!= instance->_type && "f" == *( instance->_type ) );
}

v8::Handle<v8::Value> FileAttribute::getName( const v8::Arguments& args )
{
	v8::HandleScope scope;

	FileAttribute* instance = FileAttribute::Unwrap<FileAttribute>( args.This() );

	return scope.Close( WRAP_STRING( instance->_name ) );
}

v8::Handle<v8::Value> FileAttribute::getSize( v8::Local<v8::String> property, const v8::AccessorInfo& info )
{
	v8::HandleScope scope;

	FileAttribute* instance = FileAttribute::Unwrap<FileAttribute>( info.This() );

	return v8::Integer::New( instance->_size );
}
void FileAttribute::setSize( v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::AccessorInfo& info )
{
	v8::HandleScope scope;

	FileAttribute* instance = FileAttribute::Unwrap<FileAttribute>( info.This() );

	instance->_size = value->Int32Value();

}
