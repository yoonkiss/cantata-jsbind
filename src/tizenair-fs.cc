#include <iostream>
#include <fstream>
#include <v8.h>
#include <node.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include "tizenair-util.h"
#include "tizenair-fileattr.h"
#include "tizenair-fs.h"
#include <FBase.h>

v8::Persistent<v8::FunctionTemplate> FS::funcTemplate;
v8::Persistent<v8::Function> FS::constructor;

void NODE_EXTERN FS::Init( v8::Handle<v8::Object> target )
{
    AppLog("FS: Init");
	v8::HandleScope scope;

	FS::funcTemplate = v8::Persistent<v8::FunctionTemplate>::New( v8::FunctionTemplate::New( FS::New ) );
	FS::funcTemplate->InstanceTemplate() -> SetInternalFieldCount( 2 );

	FS::funcTemplate->SetClassName( v8::String::NewSymbol( "FS" ) );

	NODE_SET_PROTOTYPE_METHOD( funcTemplate, "getAttribute", getAttribute );
	NODE_SET_PROTOTYPE_METHOD( funcTemplate, "getPath", getPath );

	NODE_SET_PROTOTYPE_METHOD( funcTemplate, "list", list );
	NODE_SET_PROTOTYPE_METHOD( funcTemplate, "create", create );
	NODE_SET_PROTOTYPE_METHOD( funcTemplate, "remove", remove );
	NODE_SET_PROTOTYPE_METHOD( funcTemplate, "rename", rename );
	NODE_SET_PROTOTYPE_METHOD( funcTemplate, "stream", stream );

    FS::constructor = v8::Persistent<v8::Function>::New( FS::funcTemplate -> GetFunction() );

	target -> Set( v8::String::NewSymbol( "FS" ), FS::constructor );
}

v8::Handle<v8::Value> FS::New( const v8::Arguments& args )
{
    AppLog("FS: New");
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

	FS* instance = node::ObjectWrap::Unwrap<FS>( args.This() ); //args[0]->ToObject()
	v8::Handle<v8::Value> attrArgs[2] = { WRAP_STRING( instance->_base ), args[0] };
	return v8::Local<v8::Function>::Cast( Util::constructor->Get( v8::String::NewSymbol( "addPath" ) ) )->Call( v8::Context::GetCurrent()->Global(), 2, attrArgs );
}

v8::Handle<v8::Value> FS::getAttribute( const v8::Arguments& args )
{
    AppLog("FS: getAttribute");
	v8::HandleScope scope;

	//FS* instance = FS::Unwrap<FS>( args.This() );
	
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
	AppLog("FS: Finished");
	return scope.Close( ret );
}

#define PATH_MAX        4096

v8::Handle<v8::Value> FS::list( const v8::Arguments& args )
{
    v8::HandleScope scope;
    if ( args.Length() < 1 ) {
        return v8::Null();
    }
    v8::Handle<v8::Value> path = args[0];

    v8::Local<v8::Array> fileLists = v8::Array::New();
    struct stat info;
    const char *cPath = UNWRAP_STRING( path ).c_str();
    AppLog("Files.list: %s", cPath);

    if (lstat( cPath,  &info ) == 0) {
        if (S_ISDIR(info.st_mode)) {
            DIR *d;
            struct dirent *de;
            struct stat st;
            int file_cnt = 0;

            d = opendir(cPath);
            if (d != NULL) {
                while((de = readdir(d))) {
                    char stat_path[PATH_MAX];
                    char *name = de->d_name;

                    if(name[0] == '.') {
                        if(name[1] == 0) continue;
                        if((name[1] == '.') && (name[2] == 0)) continue;
                    }

                    if (strlen(cPath) + strlen(de->d_name) + 1 > sizeof(stat_path)) {
                        continue;
                    }
                    snprintf(stat_path, PATH_MAX, "%s/%s", cPath, de->d_name);
                    if (lstat(stat_path, &st) == 0) {
                        v8::Local<v8::Object> fileObject = v8::Object::New();
                        fileObject->Set(v8::String::New ("type"), S_ISDIR(st.st_mode) ? v8::String::New ("d") :  v8::String::New ("f"));
                        fileObject->Set(v8::String::New("name"), v8::String::New(stat_path));

                        fileLists->Set(file_cnt++, fileObject);
                    }
                }
                if (!fileLists.IsEmpty()) {
                    return scope.Close(fileLists);
                }
            }
        }
    }

    return scope.Close( v8::Null() );
}
v8::Handle<v8::Value> FS::create( const v8::Arguments& args )
{
    AppLog("FS: create");

    v8::HandleScope scope;
    if ( args.Length() < 1 ) {
        return scope.Close( v8::Null() );
    }

    struct stat stat;
    const char *cPath = UNWRAP_STRING( args[0] ).c_str();

    if (access(cPath, F_OK) == 0) {
        return scope.Close(v8::Boolean::New(false));
    }

    if (lstat( cPath,  &stat ) == 0) {

    }
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
