#include <string>
#include <stdio.h>
#include <v8.h>
#include <node.h>
#include "tizenair-util.h"

/*
#include <FSclAddressbookManager.h>
#include <FSclAddressbook.h>
#include <FSclCategory.h>
#include <FSclContact.h>
*/

v8::Persistent<v8::FunctionTemplate> Util::funcTemplate;
v8::Persistent<v8::FunctionTemplate> Util::_startsWith;
v8::Persistent<v8::FunctionTemplate> Util::_endsWith;
v8::Persistent<v8::FunctionTemplate> Util::_addPath;
v8::Persistent<v8::FunctionTemplate> Util::_getFilenameFrom;
v8::Persistent<v8::FunctionTemplate> Util::_getParentFrom;

void NODE_EXTERN Util::Init( v8::Handle<v8::Object> target )
{
	v8::HandleScope scope;

	funcTemplate = v8::Persistent<v8::FunctionTemplate>::New( v8::FunctionTemplate::New( New ) );
	funcTemplate->InstanceTemplate() -> SetInternalFieldCount( 1 );

	// 클래스 명 할당( Contacts = function() {}; )
	funcTemplate->SetClassName( v8::String::NewSymbol( "Util" ) );

	funcTemplate -> Set( v8::String::NewSymbol( "startsWith" ), ( _startsWith = v8::Persistent<v8::FunctionTemplate>::New( v8::FunctionTemplate::New( startsWith ) ) ) -> GetFunction() );
	funcTemplate -> Set( v8::String::NewSymbol( "endsWith" ), ( _startsWith = v8::Persistent<v8::FunctionTemplate>::New( v8::FunctionTemplate::New( endsWith ) ) ) -> GetFunction() );
	funcTemplate -> Set( v8::String::NewSymbol( "addPath" ), ( _startsWith = v8::Persistent<v8::FunctionTemplate>::New( v8::FunctionTemplate::New( addPath ) ) ) -> GetFunction() );
	funcTemplate -> Set( v8::String::NewSymbol( "getFilenameFrom" ), ( _startsWith = v8::Persistent<v8::FunctionTemplate>::New( v8::FunctionTemplate::New( getFilenameFrom ) ) ) -> GetFunction() );
	funcTemplate -> Set( v8::String::NewSymbol( "getParentFrom" ), ( _startsWith = v8::Persistent<v8::FunctionTemplate>::New( v8::FunctionTemplate::New( getParentFrom ) ) ) -> GetFunction() );

	// Contacts.prototype.Contacts 에 생성자 할당
	target -> Set( v8::String::NewSymbol( "Util" ), funcTemplate -> GetFunction() );
}

v8::Handle<v8::Value> Util::New( const v8::Arguments& args ) {
	return v8::Object::New();
}

v8::Handle<v8::Value> Util::startsWith( const v8::Arguments& args ) {
	if ( args.Length() < 2 ) {
		return v8::Boolean::New( false );
	} else {
		v8::String::Utf8Value str( args[0] );
		v8::String::Utf8Value checker( args[1] );

		std::string str2 = *str;
		std::string checker2 = *checker;

		if ( checker2 == str2.substr( 0, checker2.size() ) ) {
			return v8::Boolean::New( true );
		}
	}
	return v8::Boolean::New( false );
}

v8::Handle<v8::Value> Util::endsWith( const v8::Arguments& args ) {
	if ( args.Length() < 2 ) {
		return v8::Boolean::New( false );
	} else {
		v8::String::Utf8Value str( args[0] );
		v8::String::Utf8Value checker( args[1] );

		std::string str2 = *str;
		std::string checker2 = *checker;

		if ( checker2 == str2.substr( str2.size() - checker2.size(), checker2.size() ) ) {
			return v8::Boolean::New( true );
		}
	}
	return v8::Boolean::New( false );
}

std::string Util::addPath( std::string path1, std::string path2 ) {
	if ( path1.substr( path1.size() - 1, 1 ) == "/" ) {
		return addPath( path1.substr( 1, path1.size() - 1 ), path2 );
	} else if ( path2.substr( 0, 1 ) == "/" ) {
		return addPath( path1, path2.substr( 1, path2.size() - 1 ) );
	} else {
		return path1 + "/" + path2;
	}
}

v8::Handle<v8::Value> Util::addPath( const v8::Arguments& args ) {
	if ( args.Length() < 1 ) {
		return v8::Undefined();
	} else if ( args.Length() < 2 ) {
		return args[0];
	} else {
		v8::String::Utf8Value path1( args[0] );
		v8::String::Utf8Value path2( args[1] );

		return v8::String::NewSymbol( addPath( *path1, *path2 ).c_str() );
	}
}
v8::Handle<v8::Value> Util::getFilenameFrom( const v8::Arguments& args ) {
	if ( args.Length() < 1 ) {
		return v8::Undefined();
	}

	v8::String::Utf8Value path( args[0] );
	std::string p = *path;

	size_t index = p.rfind( '/' );
	if ( std::string::npos == index ) {
		return args[0];
	}


	return v8::String::NewSymbol( p.substr( index + 1, p.size() - index - 1 ).c_str() );
}
v8::Handle<v8::Value> Util::getParentFrom( const v8::Arguments& args ) {
	if ( args.Length() < 1 ) {
		return v8::Undefined();
	}

	v8::String::Utf8Value path( args[0] );
	std::string p = *path;

	size_t index = p.rfind( '/' );
	if ( std::string::npos == index ) {
		return args[0];
	}

	return v8::String::NewSymbol( p.substr( 0, index ).c_str() );
}


