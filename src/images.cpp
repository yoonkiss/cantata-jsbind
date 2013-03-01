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

#define PATH_MAX 4096
#define STRING_MAX 128

void NODE_EXTERN Images::Init(v8::Handle<v8::Object> target) {
    AppLog("Entered Images::Init");

    v8::Local<v8::FunctionTemplate> funcTemplate = v8::Local<v8::FunctionTemplate>::New(v8::FunctionTemplate::New(Images::New));

    funcTemplate->SetClassName(v8::String::NewSymbol("Images"));
    funcTemplate->Set(v8::String::NewSymbol("list"), v8::Local<v8::FunctionTemplate>::New(v8::FunctionTemplate::New(list))->GetFunction());
    funcTemplate->Set(v8::String::NewSymbol("viewMetaInfo"), v8::Local<v8::FunctionTemplate>::New(v8::FunctionTemplate::New(viewMetaInfo))->GetFunction());
    funcTemplate->Set(v8::String::NewSymbol("remove"), v8::Local<v8::FunctionTemplate>::New(v8::FunctionTemplate::New(remove))->GetFunction());
    funcTemplate->Set(v8::String::NewSymbol("rename"), v8::Local<v8::FunctionTemplate>::New(v8::FunctionTemplate::New(rename))->GetFunction());
    funcTemplate->Set(v8::String::NewSymbol("moveTo"), v8::Local<v8::FunctionTemplate>::New(v8::FunctionTemplate::New(moveTo))->GetFunction());
    funcTemplate->Set(v8::String::NewSymbol("createAlbum"), v8::Local<v8::FunctionTemplate>::New(v8::FunctionTemplate::New(createAlbum))->GetFunction());
    funcTemplate->Set(v8::String::NewSymbol("listAlbum"), v8::Local<v8::FunctionTemplate>::New(v8::FunctionTemplate::New(listAlbum))->GetFunction());

    target->Set(v8::String::NewSymbol("Images"), funcTemplate->GetFunction());
}

v8::Handle<v8::Value> Images::New(const v8::Arguments& args) {
    v8::HandleScope scope;
    // nothing to do new()
    return args.This();
}

v8::Handle<v8::Value> Images::list(const v8::Arguments& args) {
    AppLog("Entered Images::list (args length:%d)", args.Length());

    if ( args.Length() < 3 ) {
        AppLog("Bad parameters");
        return v8::ThrowException(v8::String::New("Bad parameters"));
    }
    v8::HandleScope scope;
    result r = E_SUCCESS;
    String whereExpr = L"";
    int cnt = 0;
    int pageNo = 1;
    int countPerPage = TizenContents::MAX_CONTENTSEARCH_COUNTPERPAGE;

    whereExpr.SetCapacity(TizenContents::STRING_CAPACITY);

    if(!Util::isArgumentNull(args[0])) {
        whereExpr.Format(TizenContents::STRING_CAPACITY, L"Category LIKE '%%%s%%'", UNWRAP_STRING(args[0]).c_str());
    } else {
        whereExpr = L"";
    }

    if(!Util::isArgumentNull(args[1])) {
        pageNo = args[1]->NumberValue();
    }
    if(!Util::isArgumentNull(args[2])) {
        countPerPage = args[2]->NumberValue();
    }
    AppLog("###: %d, %d, %ls", pageNo, countPerPage, whereExpr.GetPointer());
    TizenContents* pContents = new TizenContents(CONTENT_TYPE_IMAGE, pageNo, countPerPage);

    IList* pImageContents = pContents->getContentSearchList(whereExpr);
    if (pImageContents == null) {
        delete pContents;
        return scope.Close(v8::Undefined());
    }

    v8::Local<v8::Object> fileObject = v8::Object::New();
    v8::Local<v8::Array> fileLists = v8::Array::New();

    fileObject->Set(v8::String::New("total"), v8::Number::New(pContents->getTotalCount()));
    fileObject->Set(v8::String::New("category"), Util::isArgumentNull(args[0]) ? v8::Null() : args[0]->ToString());
    fileObject->Set(v8::String::New("page"), v8::Number::New(pageNo));
    fileObject->Set(v8::String::New("countperpage"), v8::Number::New(countPerPage));

    IEnumerator* pImageEnum = pImageContents->GetEnumeratorN();
    while (pImageEnum->MoveNext() == E_SUCCESS) {
        ContentSearchResult* pSearchInfo = (ContentSearchResult*)pImageEnum->GetCurrent();
        ContentInfo* pContentInfo = pSearchInfo->GetContentInfo();

        String path = pContentInfo->GetContentPath();
        String name = Tizen::Io::File::GetFileName(path);

        if (Tizen::Io::File::IsFileExist(path)) {
            char cname[STRING_MAX];
            char cpath[PATH_MAX];

            v8::Local<v8::Array> aFile = v8::Array::New(2);
            aFile->Set(0, v8::String::New(Util::toAnsi(cname, name, STRING_MAX)));
            aFile->Set(1, v8::String::New(Util::toAnsi(cpath, path, PATH_MAX)));
            fileLists->Set(cnt++, aFile);
        }
        delete pSearchInfo;
    }

    fileObject->Set(v8::String::New("name"), fileLists);

    delete pContents;
    delete pImageContents;
    return scope.Close(fileObject);
}

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
    Tizen::Base::String sourcePath(UNWRAP_STRING(args[0]).c_str());

    ContentManager contentManager;
    r = contentManager.Construct();
    if (IsFailed(r)) {
        AppLog("Failed to create content manager");
        return scope.Close(v8::Boolean::New(false));
    }

    ImageContentInfo sourceContentInfo;
    ContentId sourceContentId;
    r = sourceContentInfo.Construct(&sourcePath);
    if (IsFailed(r))
    {
        AppLog("Failed to get source contentInfo");
        return scope.Close(v8::Boolean::New(false));
    }
    sourceContentId = contentManager.CreateContent(sourceContentInfo);
    if (Tizen::Base::UuId::GetInvalidUuId() != sourceContentId)
    {
        // TODO: how to check?
        /*AppLog("Failed to get contentInfo: %s", GetLastResult());
        return scope.Close(v8::Boolean::New(false));*/
    }
    r = contentManager.DeleteContent(sourceContentId);
    if (IsFailed(r))
    {
       AppLog("Failed to delete the content: %s", GetErrorMessage(GetLastResult()));
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
