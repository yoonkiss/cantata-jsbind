#include <node.h>
#include <FBase.h>
#include <FAppPkg.h>

#include "tizenair-util.h"
#include "tizenair_apps.h"

using namespace Tizen::App::Package;
using namespace Tizen::Base;


/**
 * js bind for Tizen::App API
 *
 * @author Gyeongseok Seo {@literal <gseok.seo@gmail.com>}
 *
 */
void NODE_EXTERN Apps::Init(v8::Handle<v8::Object> target) {
    AppLog( "entered Apps::Init" );

    v8::HandleScope scope;
    v8::Local<v8::FunctionTemplate> funcTemplate = v8::Local<v8::FunctionTemplate>::New(v8::FunctionTemplate::New(Apps::New));

    //funcTemplate->InstanceTemplate() -> SetInternalFieldCount( 1 );
    funcTemplate->SetClassName(v8::String::NewSymbol("Apps"));

    //NODE_SET_PROTOTYPE_METHOD(funcTemplate, "name", function);
    funcTemplate -> Set( v8::String::NewSymbol( "getAppsInfoList" ), v8::FunctionTemplate::New( getAppsInfoList ) -> GetFunction() );

    target->Set( v8::String::NewSymbol( "Apps" ), funcTemplate -> GetFunction() );
}

v8::Handle<v8::Value> Apps::New(const v8::Arguments& args) {
    v8::HandleScope scope;
    // nothing to do new()
    return args.This();
}

v8::Handle<v8::Value> Apps::getAppsInfoList( const v8::Arguments& args ) {
    AppLog( "entered Apps::getInstalledAppInfoList" );

    v8::HandleScope scope;
    v8::Local<v8::Object> appInfoList = v8::Object::New(); // { 'appInfo-list': [ ..., ... ] }
    v8::Local<v8::Array> appInfoArray = v8::Array::New(); // [ ..., ... ]

    PackageManager *pPkgManager = PackageManager::GetInstance();
    if ( pPkgManager == null ) {
        return scope.Close( v8::Undefined() );
    }
    Collection::IList *pPkgInfoList = pPkgManager->GetPackageInfoListN(); // must free
    if ( pPkgInfoList == null || pPkgInfoList->GetCount() < 1 ) {
        pPkgManager = null;
        return scope.Close( v8::Undefined() );
    }

    int count = 0;
    Collection::IEnumerator *pEnum = pPkgInfoList->GetEnumeratorN(); // must free
    while( pEnum->MoveNext() == E_SUCCESS ) {
        PackageInfo *pPkgInfo = (PackageInfo *)pEnum->GetCurrent();
        Tizen::App::AppId appId = pPkgInfo->GetMainAppId();

        PackageAppInfo *pAppInfo = pPkgInfo->GetPackageAppInfoN( appId ); // must free
        if ( GetLastResult() == E_SUCCESS && pAppInfo->IsMenuIconVisible() ) {
            // make installed application info
            v8::Local<v8::Object> appInfo = v8::Object::New();

            // set App id
            char cappid[128];
            appInfo->Set( v8::String::New( "app-id" ), v8::String::New( Util::toAnsi(cappid, appId, 128) ) );

            // set App name
            char cname[128];
            String name = pAppInfo->GetAppName();
            appInfo->Set( v8::String::New( "app-name" ), v8::String::New( Util::toAnsi(cname, name, 128) ) );
            AppLog( cname );

            // set App display name
            char cdisname[128];
            String dname = pAppInfo->GetAppDisplayName();
            appInfo->Set( v8::String::New( "app-displayname" ), v8::String::New( Util::toAnsi(cdisname, dname, 128) ) );

            // set App package id
            char cpkgid[128];
            Tizen::App::PackageId pkgId = pPkgInfo->GetId();
            appInfo->Set( v8::String::New( "pkg-id" ), v8::String::New( Util::toAnsi(cpkgid, pkgId, 128) ) );

            // set App package size
            char cpkgsize[128];
            long long pkgsize = pPkgInfo->GetSize();
            sprintf(cpkgsize, "%lld", pkgsize);
            appInfo->Set( v8::String::New( "pkg-size" ), v8::String::New(cpkgsize) );

            // set App package data size
            char cpkgdsize[128];
            long long pkgdsize = pPkgInfo->GetDataSize();
            sprintf(cpkgdsize, "%lld", pkgdsize);
            appInfo->Set( v8::String::New( "pkg-data-size" ), v8::String::New(cpkgdsize) );

            // set App package author
            char cauthor[128];
            String author = pPkgInfo->GetAuthor();
            appInfo->Set( v8::String::New( "pkg-author" ), v8::String::New( Util::toAnsi(cauthor, author, 128) ) );

            // set App package version
            char cpkgversion[128];
            String version = pPkgInfo->GetVersion();
            appInfo->Set( v8::String::New( "pkg-version" ), v8::String::New( Util::toAnsi(cpkgversion, version, 128) ) );

            // set App url
            char cpkgurl[128];
            String url = pPkgInfo->GetUrl();
            appInfo->Set( v8::String::New( "pkg-url" ), v8::String::New( Util::toAnsi(cpkgurl, url, 128) ) );

            // set App package install time
            v8::Local<v8::Object> pkgTimeInfo = v8::Object::New();
            DateTime dt = pPkgInfo->GetInstallationTime();
            int year, month, day, hour, min, sec;
            year = dt.GetYear();
            month = dt.GetMonth();
            day = dt.GetDay();
            hour = dt.GetHour();
            min = dt.GetMinute();
            sec = dt.GetSecond();
            pkgTimeInfo->Set( v8::String::New( "year" ), v8::String::New( Util::toAnsi(year) ) );
            pkgTimeInfo->Set( v8::String::New( "month" ), v8::String::New( Util::toAnsi(month) ) );
            pkgTimeInfo->Set( v8::String::New( "day" ), v8::String::New( Util::toAnsi(day) ) );
            pkgTimeInfo->Set( v8::String::New( "hour" ), v8::String::New( Util::toAnsi(hour) ) );
            pkgTimeInfo->Set( v8::String::New( "min" ), v8::String::New( Util::toAnsi(min) ) );
            pkgTimeInfo->Set( v8::String::New( "sec" ), v8::String::New( Util::toAnsi(sec) ) );
            appInfo->Set( v8::String::New( "pkg-install-time" ), pkgTimeInfo );

            appInfoArray->Set( count++, appInfo );
        }

        // free
        delete pAppInfo;
        pAppInfo = null;
    }
    appInfoList->Set( v8::String::New( "appInfo-list" ), appInfoArray );

    // free
    if ( pEnum != null ) {
        delete pEnum;
        pEnum = null;
    }

    if ( pPkgInfoList != null ) {
        delete pPkgInfoList;
        pPkgInfoList = null;

    }
    pPkgManager = null;

    // app info list return
    if ( !appInfoList.IsEmpty() ) {
        return scope.Close( appInfoList );
    }

    return scope.Close( v8::Undefined() );
}
