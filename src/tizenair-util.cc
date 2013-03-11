#include <string>
#include <locale>
#include <stdio.h>
#include <stdlib.h>
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

bool Util::isArgumentNull(const v8::Handle<v8::Value> value) {
    if (value->IsString() && value->ToString()->Length() > 0) {
        return false;
    } else {
        return true;
    }
}

char* Util::toAnsi(Tizen::Base::String str) {
    //size_t wcstombs(char *dest, const wchar_t *src, size_t n);
    char *cstr = (char *) malloc(str.GetLength()+1);
    wcstombs(cstr, str.GetPointer(), str.GetLength()+1);

    return cstr;
}

char* Util::toAnsi(char *cstr, Tizen::Base::String str, size_t n) {
    wcstombs(cstr, str.GetPointer(), n);
    return cstr;
}

char* Util::toAnsi(const int num) {
    char *cstr = (char *) malloc( 128 );
    sprintf(cstr, "%d", num);
    return cstr;
}

char* Util::toAnsi(const unsigned long num) {
    char *cstr = (char *) malloc( 128 );
    sprintf(cstr, "%lu", num);
    return cstr;
}

char* Util::toAnsi(const double num) {
    char *cstr = (char *) malloc( 128 );
    sprintf(cstr, "%f", num);
    return cstr;
}

/*
 * convert C style string form a V8 Utf8Value<br>
 * if failed then return null.
 */
const char* Util::toCString(const v8::String::Utf8Value& value) {
  return *value ? *value : null;
}

/*
 * convert C style string form a V8 AsciiValue<br>
 * if failed then return null.
 */
const char* Util::toCString(const v8::String::AsciiValue& value) {
  return *value ? *value : null;
}

/*
 * convert Tizen style string form a V8 String<br>
 * if failed then return null.<br>
 *
 * caller must free
 */
Tizen::Base::String* Util::toTizenStringN(const v8::Local<v8::String>& value) {
    if ( value->Length() == 0 ) {
        return new Tizen::Base::String();
    }

    v8::String::Utf8Value v8utfstr( value->ToObject() );
    const char *pValue = toCString( v8utfstr );
    return new Tizen::Base::String( pValue );
}
