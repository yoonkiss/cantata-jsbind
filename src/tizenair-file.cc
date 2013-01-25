#include <string>
#include <stdio.h>
#include <v8.h>
#include <node.h>
#include "tizenair-file.h"

/*
#include <FSclAddressbookManager.h>
#include <FSclAddressbook.h>
#include <FSclCategory.h>
#include <FSclContact.h>
*/

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

	funcTemplate = v8::Persistent<v8::FunctionTemplate>::New( v8::FunctionTemplate::New( New ) );
	funcTemplate->InstanceTemplate() -> SetInternalFieldCount( 1 );

	// 클래스 명 할당( Contacts = function() {}; )
	funcTemplate->SetClassName( v8::String::NewSymbol( "Files" ) );

	funcTemplate -> Set( v8::String::NewSymbol( "getAttribute" ), ( _getAttribute = v8::Persistent<v8::FunctionTemplate>::New( v8::FunctionTemplate::New( getAttribute ) ) ) -> GetFunction() );
	funcTemplate -> Set( v8::String::NewSymbol( "read" ), ( _read = v8::Persistent<v8::FunctionTemplate>::New( v8::FunctionTemplate::New( read ) ) ) -> GetFunction() );
	funcTemplate -> Set( v8::String::NewSymbol( "list" ), ( _list = v8::Persistent<v8::FunctionTemplate>::New( v8::FunctionTemplate::New( list ) ) ) -> GetFunction() );
	funcTemplate -> Set( v8::String::NewSymbol( "createDirectory" ), ( _createDirectory = v8::Persistent<v8::FunctionTemplate>::New( v8::FunctionTemplate::New( createDirectory ) ) ) -> GetFunction() );
	funcTemplate -> Set( v8::String::NewSymbol( "remove" ), ( _remove = v8::Persistent<v8::FunctionTemplate>::New( v8::FunctionTemplate::New( remove ) ) ) -> GetFunction() );
	funcTemplate -> Set( v8::String::NewSymbol( "moveTo" ), ( _moveTo = v8::Persistent<v8::FunctionTemplate>::New( v8::FunctionTemplate::New( moveTo ) ) ) -> GetFunction() );
	funcTemplate -> Set( v8::String::NewSymbol( "rename" ), ( _rename = v8::Persistent<v8::FunctionTemplate>::New( v8::FunctionTemplate::New( rename ) ) ) -> GetFunction() );

	// Contacts.prototype.Contacts 에 생성자 할당
	target -> Set( v8::String::NewSymbol( "Files" ), funcTemplate -> GetFunction() );
}

v8::Handle<v8::Value> Files::New( const v8::Arguments& args ) {
	return v8::Object::New();
}

v8::Handle<v8::Value> Files::getAttribute( const v8::Arguments& args ) {
	return v8::Undefined( false );
}

v8::Handle<v8::Value> Files::read( const v8::Arguments& args ) {
	return v8::Undefined( false );
}

v8::Handle<v8::Value> Files::list( const v8::Arguments& args ) {
	return v8::Boolean::New( false );
}

v8::Handle<v8::Value> Files::createDirectory( const v8::Arguments& args ) {
	return v8::Boolean::New( false );
}

v8::Handle<v8::Value> Files::remove( const v8::Arguments& args ) {
	return v8::Boolean::New( false );
}

v8::Handle<v8::Value> Files::moveTo( const v8::Arguments& args ) {
	return v8::Boolean::New( false );
}

v8::Handle<v8::Value> Files::rename( const v8::Arguments& args ) {
	return v8::Boolean::New( false );
}
