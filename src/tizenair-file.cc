#include <string>
#include <stdio.h>
#include <v8.h>
#include <node.h>
#include "tizenair-file.h"
#include "tizenair-fs.h"
#include "tizenair-util.h"
#include "tizenair-fileattr.h"
#include <FBase.h>

#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>

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
    AppLog("Files: Init");
	v8::HandleScope scope;

	Files::funcTemplate = v8::Persistent<v8::FunctionTemplate>::New( v8::FunctionTemplate::New( Files::New ) );
	Files::funcTemplate->InstanceTemplate() -> SetInternalFieldCount( 1 );

	Files::funcTemplate->SetClassName( v8::String::NewSymbol( "Files" ) );

	funcTemplate -> Set( v8::String::NewSymbol( "getAttribute" ), ( _getAttribute = v8::Persistent<v8::FunctionTemplate>::New( v8::FunctionTemplate::New( getAttribute ) ) ) -> GetFunction() );
	funcTemplate -> Set( v8::String::NewSymbol( "read" ), ( _read = v8::Persistent<v8::FunctionTemplate>::New( v8::FunctionTemplate::New( read ) ) ) -> GetFunction() );
	funcTemplate -> Set( v8::String::NewSymbol( "list" ), ( _list = v8::Persistent<v8::FunctionTemplate>::New( v8::FunctionTemplate::New( list ) ) ) -> GetFunction() );
	funcTemplate -> Set( v8::String::NewSymbol( "createDirectory" ), ( _createDirectory = v8::Persistent<v8::FunctionTemplate>::New( v8::FunctionTemplate::New( createDirectory ) ) ) -> GetFunction() );
	funcTemplate -> Set( v8::String::NewSymbol( "remove" ), ( _remove = v8::Persistent<v8::FunctionTemplate>::New( v8::FunctionTemplate::New( remove ) ) ) -> GetFunction() );
	funcTemplate -> Set( v8::String::NewSymbol( "moveTo" ), ( _moveTo = v8::Persistent<v8::FunctionTemplate>::New( v8::FunctionTemplate::New( moveTo ) ) ) -> GetFunction() );
	funcTemplate -> Set( v8::String::NewSymbol( "rename" ), ( _rename = v8::Persistent<v8::FunctionTemplate>::New( v8::FunctionTemplate::New( rename ) ) ) -> GetFunction() );

    //v8::Handle<v8::Value> attrArgs[1] = { v8::String::NewSymbol( "/" ) };
    //FS::constructor->Call( v8::Context::GetCurrent()->Global(), 1, attrArgs );
    //Files::_fs = v8::Persistent<v8::Function>::New( v8::Handle<v8::Function>::Cast( FS::constructor->Call( v8::Context::GetCurrent()->Global(), 1, attrArgs ) ) );
	target -> Set( v8::String::NewSymbol( "Files" ), funcTemplate -> GetFunction() );
}

v8::Handle<v8::Value> Files::New( const v8::Arguments& args ) {
    AppLog("Files: New");

    v8::HandleScope scope;

    return v8::Object::New();
}

v8::Handle<v8::Value> Files::getAttribute( const v8::Arguments& args ) {
    AppLog("TODO: Files: getAttribute: %s", UNWRAP_STRING( args[0] ).c_str());
    /*
    v8::HandleScope scope;
    v8::Handle<v8::Value> attrArgs[1] = { args[0] };
    return Files::_fs->Call( v8::Context::GetCurrent()->Global(), 1, attrArgs );
    */
    v8::HandleScope scope;
    v8::Local<v8::Object> arg = v8::Object::New();

    struct stat info;

    v8::Handle<v8::Value> path = args[0];

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

v8::Handle<v8::Value> Files::read( const v8::Arguments& args ) {
	return v8::Undefined();
}

v8::Handle<v8::Value> Files::list( const v8::Arguments& args ) {
    v8::HandleScope scope;

    v8::Handle<v8::Value> path = args[0];//getPath( args );

    v8::Local<v8::Array> fileLists = v8::Array::New();
    struct stat info;
    const char *cPath = UNWRAP_STRING( path ).c_str();
    AppLog("entered Files::list(%s)", cPath);

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
                    strncpy(stat_path, cPath, PATH_MAX);
                    strncat(stat_path, de->d_name, PATH_MAX);

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
