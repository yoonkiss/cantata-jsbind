#include <string>
#include <stdio.h>
#include <v8.h>
#include <node.h>
#include "tizenair-fileattr.h"
#include "tizenair-file.h"

/*
#include <FSclAddressbookManager.h>
#include <FSclAddressbook.h>
#include <FSclCategory.h>
#include <FSclContact.h>
*/


v8::Persistent<v8::FunctionTemplate> FS::funcTemplate;

void NODE_EXTERN FS::Init( v8::Handle<v8::Object> target )
{
	v8::HandleScope scope;

	funcTemplate = v8::Persistent<v8::FunctionTemplate>::New( v8::FunctionTemplate::New( New ) );
	funcTemplate->InstanceTemplate() -> SetInternalFieldCount( 1 );

	// 클래스 명 할당( Contacts = function() {}; )
	funcTemplate->SetClassName( v8::String::NewSymbol( "FS" ) );
	target -> Set( v8::String::NewSymbol( "FS" ), funcTemplate -> GetFunction() );

}

v8::Handle<v8::Value> FS::New( const v8::Arguments& args )
{
	return v8::Object::New();
}


