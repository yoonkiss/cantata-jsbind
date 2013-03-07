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

void NODE_EXTERN Images::Init(v8::Handle<v8::Object> target) {
   AppLog("entered Images::Init");
   v8::Local<v8::FunctionTemplate> funcTemplate = v8::Local<v8::FunctionTemplate>::New(v8::FunctionTemplate::New(Images::New));
   funcTemplate->SetClassName(v8::String::NewSymbol("Images"));
   funcTemplate->Set(v8::String::NewSymbol("getAllImageInfo"), v8::FunctionTemplate::New(getAllImageInfo)->GetFunction());
   funcTemplate->Set(v8::String::NewSymbol("getAlbumlistNames"), v8::FunctionTemplate::New(getAlbumlistNames)->GetFunction());

   target->Set(v8::String::NewSymbol("Images"), funcTemplate->GetFunction());
}

v8::Handle<v8::Value> Images::New(const v8::Arguments& args) {
    v8::HandleScope scope;
    // nothing to do new()
    return args.This();
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

                // set Image id
                imageInfo->Set( v8::String::New( "id" ), v8::String::New( Util::toAnsi( id ) ) );

                // set Image name
                imageInfo->Set( v8::String::New( "name" ), v8::String::New( Util::toAnsi( name ) ) );

                // set Image path
                imageInfo->Set( v8::String::New( "path" ), v8::String::New( Util::toAnsi( path ) ) );

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
