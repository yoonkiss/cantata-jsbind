#include <string>
#include <stdio.h>
#include <v8.h>
#include <node.h>
#include "tizenair-file.h"

v8::Persistent<v8::FunctionTemplate> Files::funcTemplate;
v8::Persistent<v8::FunctionTemplate> Files::_getAttribute;
v8::Persistent<v8::FunctionTemplate> Files::_read;
v8::Persistent<v8::FunctionTemplate> Files::_list;
v8::Persistent<v8::FunctionTemplate> Files::_createDirectory;
v8::Persistent<v8::FunctionTemplate> Files::_remove;
v8::Persistent<v8::FunctionTemplate> Files::_moveTo;
v8::Persistent<v8::FunctionTemplate> Files::_rename;

void NODE_EXTERN Files::Init( v8::Handle<v8::Object> target )
{
	v8::HandleScope scope;

	Files::funcTemplate = v8::Persistent<v8::FunctionTemplate>::New( v8::FunctionTemplate::New( Files::New ) );
	Files::funcTemplate->InstanceTemplate() -> SetInternalFieldCount( 1 );

	Files::funcTemplate->SetClassName( v8::String::NewSymbol( "Files" ) );

	/*
	funcTemplate -> Set( v8::String::NewSymbol( "getAttribute" ), ( _getAttribute = v8::Persistent<v8::FunctionTemplate>::New( v8::FunctionTemplate::New( getAttribute ) ) ) -> GetFunction() );
	funcTemplate -> Set( v8::String::NewSymbol( "read" ), ( _read = v8::Persistent<v8::FunctionTemplate>::New( v8::FunctionTemplate::New( read ) ) ) -> GetFunction() );
	funcTemplate -> Set( v8::String::NewSymbol( "list" ), ( _list = v8::Persistent<v8::FunctionTemplate>::New( v8::FunctionTemplate::New( list ) ) ) -> GetFunction() );
	funcTemplate -> Set( v8::String::NewSymbol( "createDirectory" ), ( _createDirectory = v8::Persistent<v8::FunctionTemplate>::New( v8::FunctionTemplate::New( createDirectory ) ) ) -> GetFunction() );
	funcTemplate -> Set( v8::String::NewSymbol( "remove" ), ( _remove = v8::Persistent<v8::FunctionTemplate>::New( v8::FunctionTemplate::New( remove ) ) ) -> GetFunction() );
	funcTemplate -> Set( v8::String::NewSymbol( "moveTo" ), ( _moveTo = v8::Persistent<v8::FunctionTemplate>::New( v8::FunctionTemplate::New( moveTo ) ) ) -> GetFunction() );
	funcTemplate -> Set( v8::String::NewSymbol( "rename" ), ( _rename = v8::Persistent<v8::FunctionTemplate>::New( v8::FunctionTemplate::New( rename ) ) ) -> GetFunction() );
	*/

	target -> Set( v8::String::NewSymbol( "Files" ), funcTemplate -> GetFunction() );
}

v8::Handle<v8::Value> Files::New( const v8::Arguments& args ) {
	return args.This();
}

v8::Handle<v8::Value> Files::getAttribute( const v8::Arguments& args ) {
	return v8::Undefined();
}

v8::Handle<v8::Value> Files::read( const v8::Arguments& args ) {
	return v8::Undefined();
}

v8::Handle<v8::Value> Files::list( const v8::Arguments& args ) {
	return v8::Undefined();
}

v8::Handle<v8::Value> Files::createDirectory( const v8::Arguments& args ) {
	return v8::Undefined();
}

v8::Handle<v8::Value> Files::remove( const v8::Arguments& args ) {
	return v8::Undefined();
}

v8::Handle<v8::Value> Files::moveTo( const v8::Arguments& args ) {
	return v8::Undefined();
}

v8::Handle<v8::Value> Files::rename( const v8::Arguments& args ) {
	return v8::Undefined();
}
