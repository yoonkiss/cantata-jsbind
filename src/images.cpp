#include <v8.h>
#include <node.h>
#include <stdlib.h>

#include <FSystem.h>
#include <iostream>
#include <FContent.h>
#include <FIo.h>
#include <FBaseSysLog.h>
#include <FBase.h>
#include <FIo.h>


#include "contents.h"
#include "images.h"
#include "tizenair-util.h"

using namespace Tizen::Base::Collection;
using namespace Tizen::Content;
using namespace Tizen::Locations;
using namespace Tizen::System;

/**
 * js bind for Tizen::Content API, Image Contents
 *
 * @author Gyeongseok Seo {@literal <gseok.seo@gmail.com>}
 *
 */
void NODE_EXTERN Images::Init(v8::Handle<v8::Object> target) {
   AppLog("entered Images::Init");
   v8::Local<v8::FunctionTemplate> funcTemplate = v8::Local<v8::FunctionTemplate>::New(v8::FunctionTemplate::New(Images::New));
   funcTemplate->SetClassName(v8::String::NewSymbol("Images"));
   funcTemplate->Set(v8::String::NewSymbol("getAllImageInfoToPath"), v8::FunctionTemplate::New(getAllImageInfoToPath)->GetFunction());
   funcTemplate->Set(v8::String::NewSymbol("getAllImageInfo"), v8::FunctionTemplate::New(getAllImageInfo)->GetFunction());
   funcTemplate->Set(v8::String::NewSymbol("getAllImageIDInfo"), v8::FunctionTemplate::New(getAllImageIDInfo)->GetFunction());
   funcTemplate->Set(v8::String::NewSymbol("getAllImagePathInfo"), v8::FunctionTemplate::New(getAllImagePathInfo)->GetFunction());
   funcTemplate->Set(v8::String::NewSymbol("getImageMetaInfo"), v8::FunctionTemplate::New(getImageMetaInfo)->GetFunction());
   funcTemplate->Set(v8::String::NewSymbol("getImageInfoForId"), v8::FunctionTemplate::New(getImageInfoForId)->GetFunction());
   funcTemplate->Set(v8::String::NewSymbol("deleteImageForId"), v8::FunctionTemplate::New(deleteImageForId)->GetFunction());
   funcTemplate->Set(v8::String::NewSymbol("createImage"), v8::FunctionTemplate::New(createImage)->GetFunction());
   funcTemplate->Set(v8::String::NewSymbol("moveImageForId"), v8::FunctionTemplate::New(moveImageForId)->GetFunction());
   funcTemplate->Set(v8::String::NewSymbol("moveImageForPath"), v8::FunctionTemplate::New(moveImageForPath)->GetFunction());
   funcTemplate->Set(v8::String::NewSymbol("getAlbumlistNames"), v8::FunctionTemplate::New(getAlbumlistNames)->GetFunction());

   target->Set(v8::String::NewSymbol("Images"), funcTemplate->GetFunction());
}

v8::Handle<v8::Value> Images::New(const v8::Arguments& args) {
    v8::HandleScope scope;
    // nothing to do new()
    return args.This();
}

v8::Handle<v8::Value> Images::getAllImageInfoToPath(const v8::Arguments& args) {
    AppLog("Entered Images::getAllImageInfoToPath");

    v8::HandleScope scope;
    v8::Local<v8::Object> infoSet = v8::Object::New(); // { 'pathInfoLength' : xxxx, 'infoToPath' : [] }
    v8::Local<v8::Array> infoToPath = v8::Array::New(); // [ ..., ... ]
    char *pResult = null;

    // set info to path length
    ContentDirectory dir;
    dir.Construct(CONTENT_TYPE_IMAGE);
    if ( IsFailed(GetLastResult()) ) {
        AppLog("Failed to get ContentDirectory: %s", GetErrorMessage( GetLastResult() ) );
        return scope.Close( v8::Undefined() );
    }

    int infoLength = dir.GetContentDirectoryCount(); // get length
    pResult = Util::toAnsi( infoLength );
    infoSet->Set( v8::String::New( "infoToPathLength" ), v8::String::New( pResult ) );
    delete pResult;

    // get info to path - ( path, imageInfo-list )
    IList *pImageDirPathList = dir.GetContentDirectoryPathListN(SORT_ORDER_ASCENDING); // get 'image' dir path
    if ( IsFailed( GetLastResult() ) ) {
        AppLog("Failed to get Content Directory Path List: %s", GetErrorMessage( GetLastResult() ) );
        return scope.Close( v8::Undefined() );
    }
    IEnumerator *pEnum = pImageDirPathList->GetEnumeratorN();
    int count = 0;
    while ( pEnum->MoveNext() == E_SUCCESS ) {
        v8::Local<v8::Object> infoToPathElement = v8::Object::New();
        v8::Local<v8::Array> imageInfoArray = v8::Array::New(); // [ ..., ... ]

        // set Path Info
        String *pPath = static_cast<String*>(pEnum->GetCurrent());
        pResult = Util::toAnsi( *pPath );
        infoToPathElement->Set( v8::String::New( "path" ), v8::String::New( pResult ) );
        delete pResult;

        // set image info list length
        int itemCnt = dir.GetContentDirectoryItemCount( *pPath );
        pResult = Util::toAnsi( itemCnt );
        infoToPathElement->Set( v8::String::New( "imageInfo-list-length" ), v8::String::New( pResult ) );
        delete pResult;

        // each Item Info
        IList *pItemInfo = dir.GetContentDirectoryItemListN( *pPath, 1, itemCnt, L"ContentType", SORT_ORDER_ASCENDING );
        IEnumerator *pInEnum = pItemInfo->GetEnumeratorN();
        int incount = 0;
        while ( pInEnum->MoveNext() == E_SUCCESS ) {
            ContentInfo *pInfo = static_cast<ContentInfo *>(pInEnum->GetCurrent());
            ImageContentInfo *pImgInfo = null;
            if ( pInfo->GetContentType() == CONTENT_TYPE_IMAGE ) {
                pImgInfo = static_cast<ImageContentInfo*>(pInfo);
            }

            // get each item info
            if ( pImgInfo != null ) { // 'image' contents
                v8::Local<v8::Object> imageInfo = v8::Object::New();

                String id = pImgInfo->GetContentId().ToString();
                String name = pImgInfo->GetContentName();
                String path = pImgInfo->GetContentPath();
                String category = pImgInfo->GetCategory();
                String keyword = pImgInfo->GetKeyword();
                String author = pImgInfo->GetAuthor();
                String format = pImgInfo->GetMediaFormat();
                String rating = pImgInfo->GetRating();
                String desc = pImgInfo->GetDescription();
                String loctag = pImgInfo->GetLocationTag();
                unsigned long size = pImgInfo->GetContentSize();

                Coordinates coordinates = pImgInfo->GetCoordinates();
                double lati = coordinates.GetLatitude();
                double longi = coordinates.GetLongitude();

                DateTime dt = pImgInfo->GetDateTime();
                int year, month, day, hour, min, sec;
                year = dt.GetYear();
                month = dt.GetMonth();
                day = dt.GetDay();
                hour = dt.GetHour();
                min = dt.GetMinute();
                sec = dt.GetSecond();

                // set Image id
                pResult = Util::toAnsi( id );
                imageInfo->Set( v8::String::New( "id" ), v8::String::New( pResult ) );
                delete pResult;

                // set Image name
                pResult = Util::toAnsi( name );
                imageInfo->Set( v8::String::New( "name" ), v8::String::New( pResult ) );
                delete pResult;

                // set Image path
                pResult = Util::toAnsi( path );
                imageInfo->Set( v8::String::New( "path" ), v8::String::New( pResult ) );
                delete pResult;

                // set Image size
                pResult = Util::toAnsi( size );
                imageInfo->Set( v8::String::New( "size" ), v8::String::New( pResult ) );
                delete pResult;

                // set Image category
                pResult = Util::toAnsi( category );
                imageInfo->Set( v8::String::New( "category" ), v8::String::New( pResult ) );
                delete pResult;

                // set Image keyword
                pResult = Util::toAnsi( keyword );
                imageInfo->Set( v8::String::New( "keyword" ), v8::String::New( pResult ) );
                delete pResult;

                // set Image author
                pResult = Util::toAnsi( author );
                imageInfo->Set( v8::String::New( "author" ), v8::String::New( pResult ) );
                delete pResult;

                // set Image format
                pResult = Util::toAnsi( format );
                imageInfo->Set( v8::String::New( "format" ), v8::String::New( pResult ) );
                delete pResult;

                // set Image rating
                pResult = Util::toAnsi( rating );
                imageInfo->Set( v8::String::New( "rating" ), v8::String::New( pResult ) );
                delete pResult;

                // set Image description
                pResult = Util::toAnsi( desc );
                imageInfo->Set( v8::String::New( "desc" ), v8::String::New( pResult ) );
                delete pResult;

                // set Image location tag
                pResult = Util::toAnsi( loctag );
                imageInfo->Set( v8::String::New( "loctag" ), v8::String::New( pResult ) );
                delete pResult;

                // set Image location coordinates
                v8::Local<v8::Object> coordinatesInfo = v8::Object::New();
                pResult = Util::toAnsi( lati ); // latitude
                coordinatesInfo->Set( v8::String::New( "lati" ), v8::String::New( pResult ) );
                delete pResult;

                pResult = Util::toAnsi( longi ); // longitude
                coordinatesInfo->Set( v8::String::New( "long" ), v8::String::New( pResult ) );
                delete pResult;
                imageInfo->Set( v8::String::New( "loc-coordinates" ), coordinatesInfo );

                // set Image Date
                v8::Local<v8::Object> dateInfo = v8::Object::New();
                dateInfo->Set( v8::String::New( "year" ), v8::String::New( Util::toAnsi(year) ) );
                dateInfo->Set( v8::String::New( "month" ), v8::String::New( Util::toAnsi(month) ) );
                dateInfo->Set( v8::String::New( "day" ), v8::String::New( Util::toAnsi(day) ) );
                dateInfo->Set( v8::String::New( "hour" ), v8::String::New( Util::toAnsi(hour) ) );
                dateInfo->Set( v8::String::New( "min" ), v8::String::New( Util::toAnsi(min) ) );
                dateInfo->Set( v8::String::New( "sec" ), v8::String::New( Util::toAnsi(sec) ) );
                imageInfo->Set( v8::String::New( "date" ), dateInfo );

                // set Array
                imageInfoArray->Set( incount++, imageInfo );
            }
        }
        // free
        if ( pItemInfo != null ) {
            delete pItemInfo;
            pItemInfo = null;
        }
        if ( pInEnum != null ) {
            delete pInEnum;
            pInEnum = null;
        }

        // assemble infoToPath element
        infoToPathElement->Set( v8::String::New( "imageInfo-list" ), imageInfoArray );

        // set array
        infoToPath->Set( count++, infoToPathElement );
    }

    // set info to path
    infoSet->Set( v8::String::New( "infoToPath" ), infoToPath );

    // free
    if( pImageDirPathList != null ) {
        delete pImageDirPathList;
        pImageDirPathList = null;
    }
    if( pEnum != null ) {
        delete pEnum;
        pEnum = null;
    }

    // info return
    if ( !infoSet.IsEmpty() ) {
        return scope.Close( infoSet );
    }

    return scope.Close( v8::Undefined() );
}

v8::Handle<v8::Value> Images::getAllImageInfo(const v8::Arguments& args) {
    AppLog("Entered Images::getAllImageInfo");

    v8::HandleScope scope;
    v8::Local<v8::Object> imageInfoList = v8::Object::New(); // { 'imageInfo-list': [ ..., ... ] }
    v8::Local<v8::Array> imageInfoArray = v8::Array::New(); // [ ..., ... ]

    ContentSearch search;
    search.Construct( CONTENT_TYPE_IMAGE );
    if ( IsFailed( GetLastResult() ) ) {
        AppLog("Failed to get ContentSearch: %s", GetErrorMessage( GetLastResult() ) );
        return scope.Close( v8::Undefined() );
    }

    int totalPageCount = 0;
    int totalCount = 0;
    IList *pContentInfoList = search.SearchN(1,1,totalPageCount,totalCount,L"",L"",SORT_ORDER_NONE); // all img
    if ( pContentInfoList == null || totalCount == 0 ) {
        AppLog("Failed to get image content list");
        return scope.Close( v8::Undefined() );
    }

    pContentInfoList = search.SearchN(1,totalCount,totalPageCount,totalCount,L"",L"",SORT_ORDER_NONE); // all img, must free
    int count = pContentInfoList->GetCount();
    if ( pContentInfoList != null && count ) {
        int count = 0;
        IEnumerator *pEnum = pContentInfoList->GetEnumeratorN(); // must free
        while ( pEnum->MoveNext() == E_SUCCESS ) {
            ContentSearchResult *pSearchResult = static_cast<ContentSearchResult*>(pEnum->GetCurrent());
            ContentInfo *pInfo = pSearchResult->GetContentInfo();
            ImageContentInfo *pImgInfo = null;
            if ( pInfo->GetContentType() == CONTENT_TYPE_IMAGE ) {
                pImgInfo = static_cast<ImageContentInfo*>(pInfo);
            }

            if ( pImgInfo != null ) { // 'image' contents
                v8::Local<v8::Object> imageInfo = v8::Object::New();

                String id = pImgInfo->GetContentId().ToString();
                String name = pImgInfo->GetContentName();
                String path = pImgInfo->GetContentPath();
                String category = pImgInfo->GetCategory();
                String keyword = pImgInfo->GetKeyword();
                String author = pImgInfo->GetAuthor();
                String format = pImgInfo->GetMediaFormat();
                String rating = pImgInfo->GetRating();
                String desc = pImgInfo->GetDescription();
                String loctag = pImgInfo->GetLocationTag();
                unsigned long size = pImgInfo->GetContentSize();

                Coordinates coordinates = pImgInfo->GetCoordinates();
                double lati = coordinates.GetLatitude();
                double longi = coordinates.GetLongitude();

                DateTime dt = pImgInfo->GetDateTime();
                int year, month, day, hour, min, sec;
                year = dt.GetYear();
                month = dt.GetMonth();
                day = dt.GetDay();
                hour = dt.GetHour();
                min = dt.GetMinute();
                sec = dt.GetSecond();

                char *pResult = null;
                // set Image id
                pResult = Util::toAnsi( id );
                imageInfo->Set( v8::String::New( "id" ), v8::String::New( pResult ) );
                delete pResult;

                // set Image name
                pResult = Util::toAnsi( name );
                imageInfo->Set( v8::String::New( "name" ), v8::String::New( pResult ) );
                delete pResult;

                // set Image path
                pResult = Util::toAnsi( path );
                imageInfo->Set( v8::String::New( "path" ), v8::String::New( pResult ) );
                delete pResult;

                // set Image size
                pResult = Util::toAnsi( size );
                imageInfo->Set( v8::String::New( "size" ), v8::String::New( pResult ) );
                delete pResult;

                // set Image category
                pResult = Util::toAnsi( category );
                imageInfo->Set( v8::String::New( "category" ), v8::String::New( pResult ) );
                delete pResult;

                // set Image keyword
                pResult = Util::toAnsi( keyword );
                imageInfo->Set( v8::String::New( "keyword" ), v8::String::New( pResult ) );
                delete pResult;

                // set Image author
                pResult = Util::toAnsi( author );
                imageInfo->Set( v8::String::New( "author" ), v8::String::New( pResult ) );
                delete pResult;

                // set Image format
                pResult = Util::toAnsi( format );
                imageInfo->Set( v8::String::New( "format" ), v8::String::New( pResult ) );
                delete pResult;

                // set Image rating
                pResult = Util::toAnsi( rating );
                imageInfo->Set( v8::String::New( "rating" ), v8::String::New( pResult ) );
                delete pResult;

                // set Image description
                pResult = Util::toAnsi( desc );
                imageInfo->Set( v8::String::New( "desc" ), v8::String::New( pResult ) );
                delete pResult;

                // set Image location tag
                pResult = Util::toAnsi( loctag );
                imageInfo->Set( v8::String::New( "loctag" ), v8::String::New( pResult ) );
                delete pResult;

                // set Image location coordinates
                v8::Local<v8::Object> coordinatesInfo = v8::Object::New();
                pResult = Util::toAnsi( lati ); // latitude
                coordinatesInfo->Set( v8::String::New( "lati" ), v8::String::New( pResult ) );
                delete pResult;

                pResult = Util::toAnsi( longi ); // longitude
                coordinatesInfo->Set( v8::String::New( "long" ), v8::String::New( pResult ) );
                delete pResult;
                imageInfo->Set( v8::String::New( "loc-coordinates" ), coordinatesInfo );

                // set Image Date
                v8::Local<v8::Object> dateInfo = v8::Object::New();
                dateInfo->Set( v8::String::New( "year" ), v8::String::New( Util::toAnsi(year) ) );
                dateInfo->Set( v8::String::New( "month" ), v8::String::New( Util::toAnsi(month) ) );
                dateInfo->Set( v8::String::New( "day" ), v8::String::New( Util::toAnsi(day) ) );
                dateInfo->Set( v8::String::New( "hour" ), v8::String::New( Util::toAnsi(hour) ) );
                dateInfo->Set( v8::String::New( "min" ), v8::String::New( Util::toAnsi(min) ) );
                dateInfo->Set( v8::String::New( "sec" ), v8::String::New( Util::toAnsi(sec) ) );
                imageInfo->Set( v8::String::New( "date" ), dateInfo );

                // set array
                imageInfoArray->Set( count++, imageInfo );
            }
        }

        // free
        if ( pEnum != null ) {
            delete pEnum;
            pEnum = null;
        }
    }

    // set all info
    imageInfoList->Set( v8::String::New( "imageInfo-list" ), imageInfoArray );

    // free
    if ( pContentInfoList->GetCount() > 0 ) {
        pContentInfoList->RemoveAll(true);
    }
    delete pContentInfoList;
    pContentInfoList = null;

    // all image info return
    if ( !imageInfoList.IsEmpty() ) {
        return scope.Close( imageInfoList );
    }

    return scope.Close( v8::Undefined() );
}

v8::Handle<v8::Value> Images::getAlbumlistNames(const v8::Arguments& args) {
    AppLog("Entered Images::getAlbumlistNames (args length:%d)", args.Length());

    v8::HandleScope scope;
    int cnt = 0;
    char buf[STRING_MAX];
    v8::Local<v8::Array> albumArray = v8::Array::New();
    albumArray->Set(cnt++, v8::String::New(Util::toAnsi(buf, DEFAULT_PLAYLIST, STRING_MAX)));

    ContentDirectory directory;
    result r = directory.Construct(CONTENT_TYPE_IMAGE);
    if (IsFailed(r)) {
        AppLog("Failed to get content directory: %s", GetErrorMessage(GetLastResult()));
        return scope.Close(v8::Undefined());
    }

    IList* pAlbumList = directory.GetContentDirectoryPathListN(SORT_ORDER_ASCENDING);
    r = GetLastResult();
    if (IsFailed(r)) {
        AppLog("Failed to get album list: %s", GetErrorMessage(r));
        return scope.Close(v8::Undefined());
    }

    if (pAlbumList != null && pAlbumList->GetCount()) {
       IEnumerator* pListEnum = pAlbumList->GetEnumeratorN();
       while (pListEnum->MoveNext() == E_SUCCESS) {
           String* albumPath = static_cast<String*>(pListEnum->GetCurrent());
           String albumName = Tizen::Io::File::GetFileName(*albumPath);
           Util::toAnsi(buf, albumName, STRING_MAX);
           albumArray->Set(cnt++, v8::String::New(buf));
       }
   }

    pAlbumList->RemoveAll(true);
    delete pAlbumList;

    return scope.Close(albumArray);
}

v8::Handle<v8::Value> Images::getAllImageIDInfo(const v8::Arguments& args) {
    AppLog("Entered Images::getAllImageIDInfo");

    v8::HandleScope scope;

    v8::Local<v8::Object> imageIDInfoList = v8::Object::New();
    v8::Local<v8::Array> imageIDInfoArray = v8::Array::New(); // [ ..., ... ]

    char *pResult = null;

    ContentSearch search;
    search.Construct( CONTENT_TYPE_IMAGE );
    if ( IsFailed( GetLastResult() ) ) {
        AppLog("Failed to get ContentSearch: %s", GetErrorMessage( GetLastResult() ) );
        return scope.Close( v8::Undefined() );
    }

    int totalPageCount = 0;
    int totalCount = 0;
    IList *pContentInfoList = search.SearchN(1,1,totalPageCount,totalCount,L"",L"",SORT_ORDER_NONE); // all img
    if ( pContentInfoList == null || totalCount == 0 ) {
        AppLog("Failed to get image content list");
        return scope.Close( v8::Undefined() );
    }
    pContentInfoList = search.SearchN(1,totalCount,totalPageCount,totalCount,L"",L"",SORT_ORDER_NONE); // all img, must free
    int infoLength = pContentInfoList->GetCount();
    // set length
    pResult = Util::toAnsi( infoLength );
    imageIDInfoList->Set( v8::String::New( "infoLength" ), v8::String::New( pResult ) );
    delete pResult;

    int count = 0;
    IEnumerator *pEnum = pContentInfoList->GetEnumeratorN(); // must free
    while ( pEnum->MoveNext() == E_SUCCESS ) {
        ContentSearchResult *pSearchResult = static_cast<ContentSearchResult*>(pEnum->GetCurrent());
        ContentInfo *pInfo = pSearchResult->GetContentInfo();
        ImageContentInfo *pImgInfo = null;
        if ( pInfo->GetContentType() == CONTENT_TYPE_IMAGE ) {
            pImgInfo = static_cast<ImageContentInfo*>(pInfo);
        }

        if ( pImgInfo != null ) { // 'image' contents
            v8::Local<v8::Object> imageIDInfo = v8::Object::New();

            String id = pImgInfo->GetContentId().ToString();

            // set Image id
            pResult = Util::toAnsi( id );
            imageIDInfo->Set( v8::String::New( "id" ), v8::String::New( pResult ) );
            delete pResult;

            // set array
            imageIDInfoArray->Set( count++, imageIDInfo );
        }
    }

    // set all info
    imageIDInfoList->Set( v8::String::New( "imageIDInfo-list" ), imageIDInfoArray );

    // free
    if ( pContentInfoList->GetCount() > 0 ) {
        pContentInfoList->RemoveAll(true);
    }
    delete pContentInfoList;
    pContentInfoList = null;

    if ( pEnum != null ) {
        delete pEnum;
        pEnum = null;
    }

    // info return
    if ( !imageIDInfoList.IsEmpty() ) {
        return scope.Close( imageIDInfoList );
    }

    return scope.Close( v8::Undefined() );
}

v8::Handle<v8::Value> Images::getAllImagePathInfo(const v8::Arguments& args) {
    AppLog("Entered Images::getAllImagePathInfo");

    v8::HandleScope scope;

    v8::Local<v8::Object> imagePathInfoList = v8::Object::New();
    v8::Local<v8::Array> imagePathInfoArray = v8::Array::New(); // [ ..., ... ]

    char *pResult = null;

    ContentDirectory dir;
    dir.Construct(CONTENT_TYPE_IMAGE);
    if ( IsFailed(GetLastResult()) ) {
        AppLog("Failed to get ContentDirectory: %s", GetErrorMessage( GetLastResult() ) );
        return scope.Close( v8::Undefined() );
    }

    // set length
    int infoLength = dir.GetContentDirectoryCount(); // get length
    pResult = Util::toAnsi( infoLength );
    imagePathInfoList->Set( v8::String::New( "infoLength" ), v8::String::New( pResult ) );
    delete pResult;

    IList *pImageDirPathList = dir.GetContentDirectoryPathListN(SORT_ORDER_ASCENDING); // get 'image' dir path
    if ( IsFailed( GetLastResult() ) ) {
        AppLog("Failed to get Content Directory Path List: %s", GetErrorMessage( GetLastResult() ) );
        return scope.Close( v8::Undefined() );
    }
    IEnumerator *pEnum = pImageDirPathList->GetEnumeratorN();
    int count = 0;
    while ( pEnum->MoveNext() == E_SUCCESS ) {
        String *pPath = static_cast<String*>(pEnum->GetCurrent());

        // set Path Info
        v8::Local<v8::Object> pathInfo = v8::Object::New();
        pResult = Util::toAnsi( *pPath );
        pathInfo->Set( v8::String::New( "path" ), v8::String::New( pResult ) );
        delete pResult;

        // set array
        imagePathInfoArray->Set( count++, pathInfo );
    }

    // set all info
    imagePathInfoList->Set( v8::String::New( "imagePathInfo-list" ), imagePathInfoArray );

    // free
    if( pImageDirPathList != null ) {
        delete pImageDirPathList;
        pImageDirPathList = null;
    }
    if( pEnum != null ) {
        delete pEnum;
        pEnum = null;
    }

    // info return
    if ( !imagePathInfoList.IsEmpty() ) {
        return scope.Close( imagePathInfoList );
    }

    return scope.Close( v8::Undefined() );
}

String Images::getTypeDescription(ImageOrientationType type) {
    Tizen::Base::String desc;
    switch ( type ) {
        case IMAGE_ORIENTATION_TYPE_UNKNOWN:
            desc = L"Unknown";
            break;
        case IMAGE_ORIENTATION_TYPE_NORMAL:
            desc = L"Normal";
            break;
        case IMAGE_ORIENTATION_TYPE_FLIP_HORIZONTAL:
            desc = L"Flip horizontal";
            break;
        case IMAGE_ORIENTATION_TYPE_ROTATE_180:
            desc = L"Rotate 180 degree";
            break;
        case IMAGE_ORIENTATION_TYPE_FLIP_VERTICAL:
            desc = L"Flip vertical";
            break;
        case IMAGE_ORIENTATION_TYPE_TRANSPOSE:
            desc = L"Transpose";
            break;
        case IMAGE_ORIENTATION_TYPE_ROTATE_90:
            desc = L"Rotate 90 degree";
            break;
        case IMAGE_ORIENTATION_TYPE_TRANSVERSE:
            desc = L"Transverse";
            break;
        case IMAGE_ORIENTATION_TYPE_ROTATE_270:
            desc = L"Rotate 270 degree";
            break;
        default:
            desc = L"Unknown";
            break;
    }
    return desc;
}

v8::Handle<v8::Value> Images::getImageMetaInfo(const v8::Arguments& args) {
    AppLog("Entered Images::getImageMetaInfo");

    v8::HandleScope scope;
    v8::Local<v8::Object> metaInfoSet = v8::Object::New(); // { 'imageMetaInfo-list' : [...] }
    v8::Local<v8::Array> metaInfoArray = v8::Array::New(); // [ ..., ... ]

    result r;
    char *pResult = null;
    String *pstr = null;
    ImageMetadata *pImageMeta = null;
    int count = 0;

    // first args is null or undefined or no args then 'ALL' image meta info return
    if ( args.Length() < 1 || args[0]->IsUndefined() || !args[0]->IsString() ) {
        ContentDirectory dir;
        dir.Construct(CONTENT_TYPE_IMAGE);
        if ( IsFailed(GetLastResult()) ) {
            AppLog("Failed to get ContentDirectory: %s", GetErrorMessage( GetLastResult() ) );
            return scope.Close( v8::Undefined() );
        }

        IList *pImageDirPathList = dir.GetContentDirectoryPathListN(SORT_ORDER_ASCENDING); // must free
        if ( IsFailed( GetLastResult() ) ) {
            AppLog("Failed to get Content Directory Path List: %s", GetErrorMessage( GetLastResult() ) );
            return scope.Close( v8::Undefined() );
        }

        IEnumerator *pEnum = pImageDirPathList->GetEnumeratorN(); // must free
        while ( pEnum->MoveNext() == E_SUCCESS ) {
            String *pPath = static_cast<String*>(pEnum->GetCurrent());
            int itemCnt = dir.GetContentDirectoryItemCount( *pPath );

            // each Item Info
            IList *pItemInfo = dir.GetContentDirectoryItemListN( *pPath, 1, itemCnt, L"ContentType", SORT_ORDER_ASCENDING ); // must free
            IEnumerator *pInEnum = pItemInfo->GetEnumeratorN();
            while ( pInEnum->MoveNext() == E_SUCCESS ) {
                ContentInfo *pInfo = static_cast<ContentInfo *>(pInEnum->GetCurrent());
                ImageContentInfo *pImgInfo = null;
                if ( pInfo->GetContentType() == CONTENT_TYPE_IMAGE ) {
                    pImgInfo = static_cast<ImageContentInfo*>(pInfo);
                }

                // get each item info
                if ( pImgInfo != null ) { // 'image' contents
                    String path = pImgInfo->GetContentPath();

                    pImageMeta  = ContentManagerUtil::GetImageMetaN( path ); // must free
                    TryCatch( pImageMeta != null, r = E_FAILURE, "Can't get ImageMeta");

                    String manufacture = pImageMeta->GetCameraManufacturer();
                    String model = pImageMeta->GetCameraModel();
                    String date = pImageMeta->GetDateTime();
                    String software = pImageMeta->GetSoftware();
                    String wbalance = pImageMeta->GetWhiteBalance();
                    String orientation = getTypeDescription( pImageMeta->GetOrientation() );
                    int heigth = pImageMeta->GetHeight();
                    int width = pImageMeta->GetWidth();
                    double lati = pImageMeta->GetLatitude();
                    double longi = pImageMeta->GetLongitude();

                    v8::Local<v8::Object> pathAndInfo = v8::Object::New();
                    pathAndInfo->Set( v8::String::New( "path" ), v8::String::New( Util::toAnsi( path ) ) );

                    v8::Local<v8::Object> metaInfo = v8::Object::New();

                    // set detail info
                    pResult = Util::toAnsi( manufacture );
                    metaInfo->Set( v8::String::New( "camera-manufacture" ), v8::String::New( pResult ) );
                    delete pResult;

                    pResult = Util::toAnsi( model );
                    metaInfo->Set( v8::String::New( "camera-model" ), v8::String::New( pResult ) );
                    delete pResult;

                    pResult = Util::toAnsi( date );
                    metaInfo->Set( v8::String::New( "date" ), v8::String::New( pResult ) );
                    delete pResult;

                    pResult = Util::toAnsi( software );
                    metaInfo->Set( v8::String::New( "software" ), v8::String::New( pResult ) );
                    delete pResult;

                    pResult = Util::toAnsi( wbalance );
                    metaInfo->Set( v8::String::New( "white-balance" ), v8::String::New( pResult ) );
                    delete pResult;

                    pResult = Util::toAnsi( orientation );
                    metaInfo->Set( v8::String::New( "orientation" ), v8::String::New( pResult ) );
                    delete pResult;

                    pResult = Util::toAnsi( heigth );
                    metaInfo->Set( v8::String::New( "heigth" ), v8::String::New( pResult ) );
                    delete pResult;

                    pResult = Util::toAnsi( width );
                    metaInfo->Set( v8::String::New( "width" ), v8::String::New( pResult ) );
                    delete pResult;

                    pResult = Util::toAnsi( lati );
                    metaInfo->Set( v8::String::New( "lati" ), v8::String::New( pResult ) );
                    delete pResult;

                    pResult = Util::toAnsi( longi );
                    metaInfo->Set( v8::String::New( "long" ), v8::String::New( pResult ) );
                    delete pResult;

                    pathAndInfo->Set( v8::String::New( "metaInfo" ), metaInfo );
                    metaInfoArray->Set( count++, pathAndInfo );
                }
            }

            // free
            if ( pItemInfo != null ) {
                delete pItemInfo;
                pItemInfo = null;
            }
            if ( pInEnum != null ) {
                delete pInEnum;
                pInEnum = null;
            }
        }

        // free
        if ( pImageDirPathList != null ) {
            delete pImageDirPathList;
            pImageDirPathList = null;
        }
        if ( pEnum != null ) {
            delete pEnum;
            pEnum = null;
        }
    } else {
        pstr = Util::toTizenStringN( args[0]->ToString() );  // must free
        pImageMeta  = ContentManagerUtil::GetImageMetaN( *pstr ); // must free
        TryCatch( pImageMeta != null, r = E_FAILURE, "Can't get ImageMeta");

        String manufacture = pImageMeta->GetCameraManufacturer();
        String model = pImageMeta->GetCameraModel();
        String date = pImageMeta->GetDateTime();
        String software = pImageMeta->GetSoftware();
        String wbalance = pImageMeta->GetWhiteBalance();
        String orientation = getTypeDescription( pImageMeta->GetOrientation() );
        int heigth = pImageMeta->GetHeight();
        int width = pImageMeta->GetWidth();
        double lati = pImageMeta->GetLatitude();
        double longi = pImageMeta->GetLongitude();

        v8::Local<v8::Object> pathAndInfo = v8::Object::New();
        pathAndInfo->Set( v8::String::New( "path" ), v8::String::New( Util::toAnsi( *pstr ) ) );

        v8::Local<v8::Object> metaInfo = v8::Object::New();

        // set detail info
        pResult = Util::toAnsi( manufacture );
        metaInfo->Set( v8::String::New( "camera-manufacture" ), v8::String::New( pResult ) );
        delete pResult;

        pResult = Util::toAnsi( model );
        metaInfo->Set( v8::String::New( "camera-model" ), v8::String::New( pResult ) );
        delete pResult;

        pResult = Util::toAnsi( date );
        metaInfo->Set( v8::String::New( "date" ), v8::String::New( pResult ) );
        delete pResult;

        pResult = Util::toAnsi( software );
        metaInfo->Set( v8::String::New( "software" ), v8::String::New( pResult ) );
        delete pResult;

        pResult = Util::toAnsi( wbalance );
        metaInfo->Set( v8::String::New( "white-balance" ), v8::String::New( pResult ) );
        delete pResult;

        pResult = Util::toAnsi( orientation );
        metaInfo->Set( v8::String::New( "orientation" ), v8::String::New( pResult ) );
        delete pResult;

        pResult = Util::toAnsi( heigth );
        metaInfo->Set( v8::String::New( "heigth" ), v8::String::New( pResult ) );
        delete pResult;

        pResult = Util::toAnsi( width );
        metaInfo->Set( v8::String::New( "width" ), v8::String::New( pResult ) );
        delete pResult;

        pResult = Util::toAnsi( lati );
        metaInfo->Set( v8::String::New( "lati" ), v8::String::New( pResult ) );
        delete pResult;

        pResult = Util::toAnsi( longi );
        metaInfo->Set( v8::String::New( "long" ), v8::String::New( pResult ) );
        delete pResult;

        pathAndInfo->Set( v8::String::New( "metaInfo" ), metaInfo );
        metaInfoArray->Set( count++, pathAndInfo );
    }

    // set info
    metaInfoSet->Set( v8::String::New( "imageMetaInfo-list" ), metaInfoArray );


CATCH:
    // free
    if ( pstr != null ) {
        delete pstr;
        pstr = null;
    }
    if ( pImageMeta != null ) {
        delete pImageMeta;
        pImageMeta = null;
    }

    // info return
    if ( !metaInfoSet.IsEmpty() ) {
        return scope.Close( metaInfoSet );
    }

    return scope.Close( v8::Undefined() );
}

v8::Handle<v8::Value> Images::getImageInfoForId(const v8::Arguments& args) {
    AppLog("Entered Images::getImageInfoForId");

    v8::HandleScope scope;
    v8::Local<v8::Object> infoSet = v8::Object::New();
    v8::Local<v8::Object> imageInfo = v8::Object::New();

    result r;
    String *pstr = null;
    ContentInfo *pImgInfo = null;

    ContentManager contentManager;
    r = contentManager.Construct();
    TryCatch( r == E_SUCCESS, r = GetLastResult(), "ContentManager Construct failed");

    if ( args.Length() < 1 || args[0]->IsUndefined() || !args[0]->IsString() ) {
        return scope.Close( v8::Undefined() );
    } else {
        pstr = Util::toTizenStringN( args[0]->ToString() );  // must free
        pstr->ToUpper();

        ContentId id;
        UuId::Parse( *pstr, id );

        pImgInfo = contentManager.GetContentInfoN( id ); // must free
        TryCatch( pImgInfo != null, r = GetLastResult(), GetErrorMessage( r ) );

        String strid = pImgInfo->GetContentId().ToString();
        String name = pImgInfo->GetContentName();
        String path = pImgInfo->GetContentPath();
        String category = pImgInfo->GetCategory();
        String keyword = pImgInfo->GetKeyword();
        String author = pImgInfo->GetAuthor();
        String format = pImgInfo->GetMediaFormat();
        String rating = pImgInfo->GetRating();
        String desc = pImgInfo->GetDescription();
        String loctag = pImgInfo->GetLocationTag();
        unsigned long size = pImgInfo->GetContentSize();

        Coordinates coordinates = pImgInfo->GetCoordinates();
        double lati = coordinates.GetLatitude();
        double longi = coordinates.GetLongitude();

        DateTime dt = pImgInfo->GetDateTime();
        int year, month, day, hour, min, sec;
        year = dt.GetYear();
        month = dt.GetMonth();
        day = dt.GetDay();
        hour = dt.GetHour();
        min = dt.GetMinute();
        sec = dt.GetSecond();

        char *pResult = null;
        // set Image id
        pResult = Util::toAnsi( strid );
        imageInfo->Set( v8::String::New( "id" ), v8::String::New( pResult ) );
        delete pResult;

        // set Image name
        pResult = Util::toAnsi( name );
        imageInfo->Set( v8::String::New( "name" ), v8::String::New( pResult ) );
        delete pResult;

        // set Image path
        pResult = Util::toAnsi( path );
        imageInfo->Set( v8::String::New( "path" ), v8::String::New( pResult ) );
        delete pResult;

        // set Image size
        pResult = Util::toAnsi( size );
        imageInfo->Set( v8::String::New( "size" ), v8::String::New( pResult ) );
        delete pResult;

        // set Image category
        pResult = Util::toAnsi( category );
        imageInfo->Set( v8::String::New( "category" ), v8::String::New( pResult ) );
        delete pResult;

        // set Image keyword
        pResult = Util::toAnsi( keyword );
        imageInfo->Set( v8::String::New( "keyword" ), v8::String::New( pResult ) );
        delete pResult;

        // set Image author
        pResult = Util::toAnsi( author );
        imageInfo->Set( v8::String::New( "author" ), v8::String::New( pResult ) );
        delete pResult;

        // set Image format
        pResult = Util::toAnsi( format );
        imageInfo->Set( v8::String::New( "format" ), v8::String::New( pResult ) );
        delete pResult;

        // set Image rating
        pResult = Util::toAnsi( rating );
        imageInfo->Set( v8::String::New( "rating" ), v8::String::New( pResult ) );
        delete pResult;

        // set Image description
        pResult = Util::toAnsi( desc );
        imageInfo->Set( v8::String::New( "desc" ), v8::String::New( pResult ) );
        delete pResult;

        // set Image location tag
        pResult = Util::toAnsi( loctag );
        imageInfo->Set( v8::String::New( "loctag" ), v8::String::New( pResult ) );
        delete pResult;

        // set Image location coordinates
        v8::Local<v8::Object> coordinatesInfo = v8::Object::New();
        pResult = Util::toAnsi( lati ); // latitude
        coordinatesInfo->Set( v8::String::New( "lati" ), v8::String::New( pResult ) );
        delete pResult;

        pResult = Util::toAnsi( longi ); // longitude
        coordinatesInfo->Set( v8::String::New( "long" ), v8::String::New( pResult ) );
        delete pResult;
        imageInfo->Set( v8::String::New( "loc-coordinates" ), coordinatesInfo );

        // set Image Date
        v8::Local<v8::Object> dateInfo = v8::Object::New();
        dateInfo->Set( v8::String::New( "year" ), v8::String::New( Util::toAnsi(year) ) );
        dateInfo->Set( v8::String::New( "month" ), v8::String::New( Util::toAnsi(month) ) );
        dateInfo->Set( v8::String::New( "day" ), v8::String::New( Util::toAnsi(day) ) );
        dateInfo->Set( v8::String::New( "hour" ), v8::String::New( Util::toAnsi(hour) ) );
        dateInfo->Set( v8::String::New( "min" ), v8::String::New( Util::toAnsi(min) ) );
        dateInfo->Set( v8::String::New( "sec" ), v8::String::New( Util::toAnsi(sec) ) );
        imageInfo->Set( v8::String::New( "date" ), dateInfo );
    }

    // set info
    infoSet->Set( v8::String::New( "imageInfo" ), imageInfo );

CATCH:
    // free
    if ( pstr != null ) {
        delete pstr;
        pstr = null;
    }
    if ( pImgInfo != null ) {
        delete pImgInfo;
        pImgInfo = null;
    }

    // info return
    if ( !infoSet.IsEmpty() ) {
        return scope.Close( infoSet );
    }

    return scope.Close( v8::Undefined() );
}

v8::Handle<v8::Value> Images::deleteImageForId(const v8::Arguments& args) {
    AppLog("Entered Images::deleteImageForId");

    v8::HandleScope scope;
    v8::Local<v8::Object> infoSet = v8::Object::New();

    result r;
    String *pstr = null;

    ContentManager contentManager;
    r = contentManager.Construct();
    TryCatch( r == E_SUCCESS, r = GetLastResult(), "ContentManager Construct failed");

    if ( args.Length() < 1 || args[0]->IsUndefined() || !args[0]->IsString() ) {
        infoSet->Set( v8::String::New( "id" ), v8::Undefined() );
        infoSet->Set( v8::String::New( "result" ), v8::False() );
        infoSet->Set( v8::String::New( "desc" ), v8::String::New( "wrong id value" ) );

        goto CATCH;
    } else {
        char *pResult = null;
        pstr = Util::toTizenStringN( args[0]->ToString() );  // must free
        pstr->ToUpper();

        ContentId id;
        UuId::Parse( *pstr, id );
        pResult = Util::toAnsi( id.ToString() );
        infoSet->Set( v8::String::New( "id" ),  v8::String::New( pResult ) );
        delete pResult;

        // Deletes
        r = contentManager.DeleteContent( id );
        if ( IsFailed(r) )
        {
            infoSet->Set( v8::String::New( "result" ), v8::False() );
            infoSet->Set( v8::String::New( "desc" ), v8::String::New( GetErrorMessage( r ) ) );
        } else {
            infoSet->Set( v8::String::New( "result" ), v8::True() );
            infoSet->Set( v8::String::New( "desc" ), v8::Undefined() );
        }

    }

CATCH:
    // free
    if ( pstr != null ) {
        delete pstr;
        pstr = null;
    }

    // info return
    if ( !infoSet.IsEmpty() ) {
        return scope.Close( infoSet );
    }

    return scope.Close( v8::Undefined() );
}

v8::Handle<v8::Value> Images::createImage(const v8::Arguments& args) {
    AppLog("Entered Images::createImage");

    v8::HandleScope scope;
    v8::Local<v8::Object> infoSet = v8::Object::New();

    result r;
    String *psourcePath = null; // first arg - image source full address
    String *pdestPath = null; // second arg - image destination full address
    bool delSource = false;  // third arg - image source delete option

    ContentManager contentManager;
    r = contentManager.Construct();
    TryCatch( r == E_SUCCESS, r = GetLastResult(), "ContentManager Construct failed");

    if ( args.Length() < 1 || args[0]->IsUndefined() || !args[0]->IsString() ) {
        infoSet->Set( v8::String::New( "id" ), v8::Undefined() );
        infoSet->Set( v8::String::New( "path" ), v8::Undefined() );
        infoSet->Set( v8::String::New( "result" ), v8::False() );
        infoSet->Set( v8::String::New( "desc" ), v8::String::New( "first argument is full path and must input" ) );
        goto CATCH;
    } else {
        char *pResult = null;

        // first argument
        psourcePath = Util::toTizenStringN( args[0]->ToString() );  // must free

        // second argument
        if ( !args[1]->IsUndefined() && args[1]->IsString() ) {
            pdestPath = Util::toTizenStringN( args[1]->ToString() );  // must free
        } else {
            // default value setting
            pResult = Util::toAnsi( *psourcePath ); // must free
            String *pName = new String( strrchr( pResult, '/' ) ); // must free
            pdestPath = new String( Environment::GetMediaPath() + L"Images" + *pName );

            // free
            delete pResult;
            delete pName;
        }

        // thired argument
        if ( !args[2]->IsUndefined() && args[2]->IsBoolean() ) {
            delSource = args[2]->BooleanValue();
        }

        // create image operation
        ContentId id;
        id = contentManager.CreateContent( *psourcePath, *pdestPath, delSource, null );
        if ( (r = GetLastResult()) != E_SUCCESS || UuId::GetInvalidUuId() == id ) {
            infoSet->Set( v8::String::New( "id" ), v8::Undefined() );
            infoSet->Set( v8::String::New( "path" ), v8::Undefined() );
            infoSet->Set( v8::String::New( "result" ), v8::False() );
            infoSet->Set( v8::String::New( "desc" ), v8::String::New( GetErrorMessage( r ) ) );
        } else {
            pResult = Util::toAnsi( id.ToString() );
            infoSet->Set( v8::String::New( "id" ),  v8::String::New( pResult ) );
            delete pResult;

            pResult = Util::toAnsi( *pdestPath );
            infoSet->Set( v8::String::New( "path" ),  v8::String::New( pResult ) );
            delete pResult;

            infoSet->Set( v8::String::New( "result" ), v8::True() );
        }
    }

CATCH:
    // free
    if ( psourcePath != null ) {
        delete psourcePath;
        psourcePath = null;
    }
    if ( pdestPath != null ) {
        delete pdestPath;
        pdestPath = null;
    }

    // info return
    if ( !infoSet.IsEmpty() ) {
        return scope.Close( infoSet );
    }

    return scope.Close( v8::Undefined() );
}

v8::Handle<v8::Value> Images::moveImageForId(const v8::Arguments& args) {
    AppLog("Entered Images::moveImageForId");

    v8::HandleScope scope;
    v8::Local<v8::Object> infoSet = v8::Object::New();

    result r;
    String *pstr = null;
    ContentInfo *pInfo = null;

    ContentManager contentManager;
    r = contentManager.Construct();
    TryCatch( r == E_SUCCESS, r = GetLastResult(), "ContentManager Construct failed");

    if ( args.Length() < 2 || args[0]->IsUndefined() || !args[0]->IsString() ||
            args[1]->IsUndefined() || !args[1]->IsString()) {
        infoSet->Set( v8::String::New( "result" ), v8::False() );
        infoSet->Set( v8::String::New( "desc" ), v8::String::New( "wrong input value" ) );
        goto CATCH;
    } else {
        char *pResult = null;

        // get old id
        pstr = Util::toTizenStringN( args[0]->ToString() );  // must free
        pstr->ToUpper();

        ContentId id;
        UuId::Parse( *pstr, id );

        // get old image info
        pInfo = contentManager.GetContentInfoN( id ); // must free
        if ( pInfo->GetContentType() != CONTENT_TYPE_IMAGE ) {
            infoSet->Set( v8::String::New( "result" ), v8::False() );
            infoSet->Set( v8::String::New( "desc" ), v8::String::New( "Not Image content" ) );
            goto CATCH;
        }

        // get old path
        String oldPath = pInfo->GetContentPath();

        // get image file name and New path setting
        pResult = Util::toAnsi( oldPath ); // must free
        String *pName = new String( strrchr( pResult, '/' ) ); // must free
        String *pInputPath = Util::toTizenStringN( args[1]->ToString() ); // must free
        String newPath = *pInputPath + *pName;
        delete pInputPath;
        delete pName;
        delete pResult;

        // create image operation ( create and old image remove )
        ContentId newId;
        newId = contentManager.CreateContent( oldPath, newPath, true, null );
        if ( (r = GetLastResult()) != E_SUCCESS || UuId::GetInvalidUuId() == newId ) {
            // failed
            infoSet->Set( v8::String::New( "result" ), v8::False() );
            infoSet->Set( v8::String::New( "desc" ), v8::String::New( GetErrorMessage( r ) ) );
            goto CATCH;
        } else {
            // success
            pResult = Util::toAnsi( id.ToString() );
            infoSet->Set( v8::String::New( "old-id" ),  v8::String::New( pResult ) );
            delete pResult;

            pResult = Util::toAnsi( oldPath );
            infoSet->Set( v8::String::New( "old-path" ),  v8::String::New( pResult ) );
            delete pResult;

            pResult = Util::toAnsi( newId.ToString() );
            infoSet->Set( v8::String::New( "new-id" ),  v8::String::New( pResult ) );
            delete pResult;

            pResult = Util::toAnsi( newPath );
            infoSet->Set( v8::String::New( "new-path" ),  v8::String::New( pResult ) );
            delete pResult;

            infoSet->Set( v8::String::New( "result" ), v8::True() );
        }
    }

CATCH:
    // free
    if ( pstr != null ) {
        delete pstr;
        pstr = null;
    }
    if ( pInfo != null ) {
        delete pInfo;
        pInfo = null;
    }

    // info return
    if ( !infoSet.IsEmpty() ) {
        return scope.Close( infoSet );
    }

    return scope.Close( v8::Undefined() );
}

v8::Handle<v8::Value> Images::moveImageForPath(const v8::Arguments& args) {
    AppLog("Entered Images::moveImageForPath");

    v8::HandleScope scope;
    v8::Local<v8::Object> infoSet = v8::Object::New();

    result r;
    String *pstr = null;
    ContentInfo *pInfo = null;

    ContentManager contentManager;
    r = contentManager.Construct();
    TryCatch( r == E_SUCCESS, r = GetLastResult(), "ContentManager Construct failed");

    if ( args.Length() < 2 || args[0]->IsUndefined() || !args[0]->IsString() ||
            args[1]->IsUndefined() || !args[1]->IsString()) {
        infoSet->Set( v8::String::New( "result" ), v8::False() );
        infoSet->Set( v8::String::New( "desc" ), v8::String::New( "wrong input value" ) );
        goto CATCH;
    } else {
        char *pResult = null;

        String *oldPathN = Util::toTizenStringN( args[0]->ToString() );
        String oldPath = *oldPathN;

        // get image file name and New path setting
        pResult = Util::toAnsi( oldPath ); // must free
        String *pName = new String( strrchr( pResult, '/' ) ); // must free
        String *pInputPath = Util::toTizenStringN( args[1]->ToString() ); // must free
        String newPath = *pInputPath + *pName;
        delete pInputPath;
        delete pName;
        delete pResult;
        delete oldPathN;

        // create image operation ( create and old image remove )
        ContentId newId;
        newId = contentManager.CreateContent( oldPath, newPath, true, null );
        if ( (r = GetLastResult()) != E_SUCCESS || UuId::GetInvalidUuId() == newId ) {
            // failed
            infoSet->Set( v8::String::New( "result" ), v8::False() );
            infoSet->Set( v8::String::New( "desc" ), v8::String::New( GetErrorMessage( r ) ) );
            goto CATCH;
        } else {
            // success
            pResult = Util::toAnsi( oldPath );
            infoSet->Set( v8::String::New( "old-path" ),  v8::String::New( pResult ) );
            delete pResult;

            pResult = Util::toAnsi( newId.ToString() );
            infoSet->Set( v8::String::New( "new-id" ),  v8::String::New( pResult ) );
            delete pResult;

            pResult = Util::toAnsi( newPath );
            infoSet->Set( v8::String::New( "new-path" ),  v8::String::New( pResult ) );
            delete pResult;

            infoSet->Set( v8::String::New( "result" ), v8::True() );
        }
    }

CATCH:
    // free
    if ( pstr != null ) {
        delete pstr;
        pstr = null;
    }
    if ( pInfo != null ) {
        delete pInfo;
        pInfo = null;
    }

    // info return
    if ( !infoSet.IsEmpty() ) {
        return scope.Close( infoSet );
    }

    return scope.Close( v8::Undefined() );
}

/*
v8::Handle<v8::Value> Images::viewMetaInfo( const v8::Arguments& args ) {
    AppLog("Entered Images::viewInfo (args length:%d)", args.Length());

    if ( args.Length() < 1 || Util::isArgumentNull(args[0])) {
        AppLog("Bad parameters");
        return v8::ThrowException(v8::String::New("Bad parameters"));
    }
    v8::HandleScope scope;

    Tizen::Base::String contentPath(UNWRAP_STRING(args[0]).c_str());
    ImageMetadata* pImgMeta = ContentManagerUtil::GetImageMetaN(contentPath);
    if (pImgMeta == null) {
        AppLog("Failed to get metainfo: %s", GetErrorMessage(GetLastResult()));
        return v8::ThrowException(v8::String::New("Failed to get metainfo"));
    }

    v8::Local<v8::Object> fileObject = v8::Object::New();
    char buf[STRING_MAX];
    fileObject->Set(v8::String::New("path"), args[0]->ToString());
    fileObject->Set(v8::String::New("width"), v8::Number::New(pImgMeta->GetWidth()));
    fileObject->Set(v8::String::New("height"), v8::Number::New(pImgMeta->GetHeight()));
    fileObject->Set(v8::String::New("cameramanufacturer"), v8::String::New(Util::toAnsi(buf, pImgMeta->GetCameraManufacturer(), STRING_MAX)));
    fileObject->Set(v8::String::New("camaramodel"), v8::String::New(Util::toAnsi(buf, pImgMeta->GetCameraModel(), STRING_MAX)));
    fileObject->Set(v8::String::New("datetime"), v8::String::New(Util::toAnsi(buf, pImgMeta->GetDateTime(), STRING_MAX)));
    fileObject->Set(v8::String::New("latitude"), v8::Number::New(pImgMeta->GetLatitude()));
    fileObject->Set(v8::String::New("longitude"), v8::Number::New(pImgMeta->GetLongitude()));
    fileObject->Set(v8::String::New("software"), v8::String::New(Util::toAnsi(buf, pImgMeta->GetSoftware(), STRING_MAX)));
    fileObject->Set(v8::String::New("whitebalance"), v8::String::New(Util::toAnsi(buf, pImgMeta->GetWhiteBalance(), STRING_MAX)));

    return scope.Close(fileObject);
}

v8::Handle<v8::Value> Images::remove( const v8::Arguments& args ) {
    AppLog("Entered Images::remove (args length:%d)", args.Length());

    if ( args.Length() < 1 || Util::isArgumentNull(args[0])) {
        AppLog("Bad parameters");
        return v8::ThrowException(v8::String::New("Bad parameters"));
    }
    v8::HandleScope scope;
    result r = E_SUCCESS;
    Tizen::Base::String contentPath(UNWRAP_STRING(args[0]).c_str());

    r = TizenContents::removeContent(contentPath);
    if (IsFailed(r))
    {
       return scope.Close(v8::Boolean::New(false));
    }

    return scope.Close(v8::Boolean::New(true));
}

v8::Handle<v8::Value> Images::rename( const v8::Arguments& args ) {
    AppLog("Entered Images::rename (args length:%d)", args.Length());
    v8::HandleScope scope;
    if ( args.Length() < 2 || Util::isArgumentNull(args[0]) || Util::isArgumentNull(args[1])) {
        AppLog("Bad parameters");
        return v8::ThrowException(v8::String::New("Bad parameters"));
    }

    String sourcePath = UNWRAP_STRING(args[0]).c_str();
    String newName = UNWRAP_STRING(args[1]).c_str();
    if (!Tizen::Io::File::IsFileExist(sourcePath)) {
        AppLog("File is not existed");
        return scope.Close(v8::Boolean::New(false));
    }
    int index = 0;
    String destPath = L"";
    sourcePath.LastIndexOf("/",  sourcePath.GetLength() - 1, index);
    sourcePath.SubString(0, index+1, destPath);
    destPath.Append(newName);

    if (destPath.CompareTo(sourcePath) == 0)
    {
        AppLog("Failed to create same file");
        return scope.Close(v8::Boolean::New(false));
    }

    if (Tizen::Io::File::IsFileExist(destPath))
    {
        AppLog("File is already existed");
        return scope.Close(v8::Boolean::New(false));
    }

    result r = E_SUCCESS;

    ContentManager contentManager;
    r = contentManager.Construct();

    ImageContentInfo sourceContentInfo;
    ContentId sourceContentId;
    r = sourceContentInfo.Construct(&sourcePath);
    if (IsFailed(r))
    {
        AppLog("Failed to get source contentInfo");
        return scope.Close(v8::Boolean::New(false));
    }
    sourceContentId = contentManager.CreateContent(sourceContentInfo);

    r = Tizen::Io::File::Copy(sourcePath, destPath, false);
    if (r == E_SUCCESS)
    {
        ImageContentInfo destContentInfo;
        r = destContentInfo.Construct(&destPath);
        if (r == E_SUCCESS) {
            contentManager.CreateContent(destContentInfo);
            contentManager.DeleteContent(sourceContentId);
            return scope.Close(v8::Boolean::New(true));
        }
    }

    return scope.Close(v8::Boolean::New(false));
}

v8::Handle<v8::Value> Images::moveTo( const v8::Arguments& args ) {
    AppLog("Entered Images::moveTo (args length:%d)", args.Length());

    if ( args.Length() < 2 || Util::isArgumentNull(args[0]) || Util::isArgumentNull(args[1])) {
        AppLog("Bad parameters");
        return v8::ThrowException(v8::String::New("Bad parameters"));
    }
    v8::HandleScope scope;

    return scope.Close(v8::Undefined());
}

v8::Handle<v8::Value> Images::createAlbum( const v8::Arguments& args ) {
    AppLog("Entered Images::createAlbum (args length:%d)", args.Length());

    if ( args.Length() < 1 ) {
        AppLog("missing arguments");
        return v8::Undefined();
    }
    v8::HandleScope scope;

    return scope.Close(v8::Undefined());
}

v8::Handle<v8::Value> Images::listAlbum( const v8::Arguments& args ) {
    AppLog("Entered Images::createAlbum (args length:%d)", args.Length());
    v8::HandleScope scope;
    Tizen::Io::Directory* pDir;
    Tizen::Io::DirEnumerator* pDirEnum;
    result r = E_SUCCESS;
    String mediaPath = Tizen::System::Environment::GetMediaPath();

    pDir = new Tizen::Io::Directory;
    r = pDir->Construct(mediaPath);
    if (IsFailed(r))
    {
        goto CATCH;
    }

    // Reads all the directory entries
    pDirEnum = pDir->ReadN();
    if (!pDirEnum)
    {
        goto CATCH;
    }

    // Loops through all the directory entries
    while (pDirEnum->MoveNext() == E_SUCCESS)
    {
        Tizen::Io::DirEntry entry = pDirEnum->GetCurrentDirEntry();

        // Do something...
    }

    // Releases the %Directory instance.
    // The opened directory is closed automatically when the destructor of the %Directory class is invoked.
    delete pDir;
    pDir = null;
    return scope.Close(v8::Boolean::New(true));

    CATCH:
    if (pDir)
    {
        delete pDir;
    }
    AppLog("Failed...");
    return scope.Close(v8::Undefined());
}

ContentInfo* Images::getContentInfo(String path) {
    result r = E_SUCCESS;
    String whereExpr = L"";
    ContentInfo *pContentInfo = null;

    whereExpr.SetCapacity(TizenContents::STRING_CAPACITY);
    whereExpr.Format(TizenContents::STRING_CAPACITY, L"ContentFileName = '%ls'", Tizen::Io::File::GetFileName(path).GetPointer());

    TizenContents* pContents = new TizenContents(CONTENT_TYPE_IMAGE, 1, 1);
    IList* pImageContents = pContents->getContentSearchList(whereExpr);
    if (pImageContents == null){
        AppLog("Failed to query: %s", GetErrorMessage(GetLastResult()));
        delete pContents;
        return null;
    }

    AppLog("pContents:%d, %d", pContents->getTotalCount(), pContents->getTotalPageCount());

    IEnumerator* pImageEnum = pImageContents->GetEnumeratorN();
    // FIXME: what is the key??
    while (pImageEnum->MoveNext() == E_SUCCESS) {
        ContentSearchResult* pSearchInfo = (ContentSearchResult*)pImageEnum->GetCurrent();
        pContentInfo = pSearchInfo->GetContentInfo();
        if (path.Equals(pContentInfo->GetContentPath(), true)) {
            delete pSearchInfo;
            break;
        }
        delete pSearchInfo;
    }
    delete pContents;
    delete pImageContents;
    return pContentInfo;
}
*/
