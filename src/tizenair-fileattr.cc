#include <string>
#include <stdio.h>
#include <v8.h>
#include <node.h>
#include "tizenair-fileattr.h"

v8::Persistent<v8::Function> FileAttribute::constructor;

void NODE_EXTERN FileAttribute::Init( v8::Handle<v8::Object> target )
{
	v8::HandleScope scope;

	v8::Local<v8::FunctionTemplate> tmpl = v8::FunctionTemplate::New( New );
	tmpl->SetClassName( v8::String::NewSymbol( "FileAttribute" ) );
	tmpl->InstanceTemplate() -> SetInternalFieldCount( 1 );

	NODE_SET_PROTOTYPE_METHOD( tmpl, "getPath", getPath );
	NODE_SET_PROTOTYPE_METHOD( tmpl, "getName", getName );
	NODE_SET_PROTOTYPE_METHOD( tmpl, "exists", exists );
	NODE_SET_PROTOTYPE_METHOD( tmpl, "isDirectory", isDirectory );
	NODE_SET_PROTOTYPE_METHOD( tmpl, "isFile", isFile );

	FileAttribute::constructor = v8::Persistent<v8::Function>::New( tmpl->GetFunction() );
	target -> Set( v8::String::NewSymbol( "FileAttribute" ), FileAttribute::constructor );
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

		printf( "Attr %s %s %s\n", instance->_type.c_str(), instance->_name.c_str(), instance->_path.c_str() );
	}

	args.This()->SetAccessor( v8::String::New( "size" ), FileAttribute::getSize, FileAttribute::setSize );
	instance -> Wrap( args.This() );

	return args.This();
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

	return scope.Close( v8::Boolean::New( instance->_type != "" ) );
}

v8::Handle<v8::Value> FileAttribute::isDirectory( const v8::Arguments& args )
{
	v8::HandleScope scope;

	FileAttribute* instance = FileAttribute::Unwrap<FileAttribute>( args.This() );

	return v8::Boolean::New( "d" == instance->_type );
}

v8::Handle<v8::Value> FileAttribute::isFile( const v8::Arguments& args )
{
	v8::HandleScope scope;

	FileAttribute* instance = FileAttribute::Unwrap<FileAttribute>( args.This() );

	return v8::Boolean::New( "f" == instance->_type );
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
