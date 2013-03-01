#include <string>
#include <stdio.h>
#include <v8.h>
#include <node.h>
#include "tizenair-util.h"

v8::Persistent<v8::Function> Util::constructor;


void NODE_EXTERN Util::Init( v8::Handle<v8::Object> target )
{
	v8::HandleScope scope;

	v8::Local<v8::FunctionTemplate> tmpl = v8::FunctionTemplate::New( New );
	tmpl->SetClassName( v8::String::NewSymbol( "Util" ) );
	tmpl->InstanceTemplate() -> SetInternalFieldCount( 1 );

	tmpl->Set( v8::String::NewSymbol( "startsWith" ), v8::FunctionTemplate::New( startsWith )->GetFunction() );
	tmpl->Set( v8::String::NewSymbol( "endsWith" ), v8::FunctionTemplate::New( endsWith )->GetFunction() );
	tmpl->Set( v8::String::NewSymbol( "addPath" ), v8::FunctionTemplate::New( addPath )->GetFunction() );
	tmpl->Set( v8::String::NewSymbol( "getFilenameFrom" ), v8::FunctionTemplate::New( getFilenameFrom )->GetFunction() );
	tmpl->Set( v8::String::NewSymbol( "getParentFrom" ), v8::FunctionTemplate::New( getParentFrom )->GetFunction() );

	Util::constructor = v8::Persistent<v8::Function>::New( tmpl->GetFunction() );
	target -> Set( v8::String::NewSymbol( "Util" ), Util::constructor );
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

		char* str2 = *str;
		char* checker2 = *checker;

		if ( 0 == strncmp( str2, checker2, strlen( checker2 ) ) ) {
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

		char* str2 = *str;
		char* checker2 = *checker;

		if ( 0 == strncmp( str2 + ( strlen( str2 ) - strlen( checker2 ) ), checker2, strlen( checker2 ) ) ) {
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

	size_t index = p.rfind( "/" );
	if ( index == std::string::npos ) {
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


