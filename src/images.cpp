#include <v8.h>
#include <node.h>
#include <FBase.h>
#include <FSystem.h>
#include <iostream>
#include <FContent.h>
#include <FIo.h>

#include "images.h"

using namespace Tizen::Base;
using namespace Tizen::Base::Collection;
using namespace Tizen::Content;

#define PATH_MAX 4096

static const int STRING_CAPACITY = 1024;
static const int MAX_CONTENTSEARCH_COUNTPERPAGE = 5;

void NODE_EXTERN Images::Init(v8::Handle<v8::Object> target) {
    AppLog("entered Images::Init");

    v8::HandleScope scope;
    v8::Local<v8::FunctionTemplate> funcTemplate = v8::Local<v8::FunctionTemplate>::New(v8::FunctionTemplate::New(Images::New));

    funcTemplate->SetClassName(v8::String::NewSymbol("Images"));
    funcTemplate->Set(v8::String::NewSymbol("list"), v8::Local<v8::FunctionTemplate>::New(v8::FunctionTemplate::New(list))->GetFunction());
    funcTemplate->Set(v8::String::NewSymbol("getPath"), v8::Local<v8::FunctionTemplate>::New(v8::FunctionTemplate::New(getPath))->GetFunction());

    target->Set(v8::String::NewSymbol("Images"), funcTemplate->GetFunction());
}


v8::Handle<v8::Value> Images::New(const v8::Arguments& args) {
    v8::HandleScope scope;
    // nothing to do new()
    return args.This();
}

v8::Handle<v8::Value> Images::list(const v8::Arguments& args) {
    AppLog("entered Images::list (args length:%d)", args.Length());

    if ( args.Length() < 3 ) {
        AppLog("missing arguments");
        return v8::Undefined();
    }
    v8::HandleScope scope;

    String strQuery = L"";
    int pageNo = 1;
    int totalPage = 0;
    int totalCount = 0;

    strQuery.SetCapacity(STRING_CAPACITY);

#if 0
    // TODO: album, page, page_view
    if (!args[0].IsEmpty()) {
        //strQuery: Category LIKE '%Image9%'
    }
    for (int j=0; j < args.Length(); j++) {
        AppLog("%dth: %s", j, UNWRAP_STRING( args[j] ).c_str());
    }
#endif

    Tizen::Content::ContentSearch search;
    result r = search.Construct(CONTENT_TYPE_IMAGE);

    if (IsFailed(r))
    {
        AppLog("Failed to create search instance");
    }

    Tizen::Base::Collection::IList*  pSearchList = search.SearchN(pageNo, MAX_CONTENTSEARCH_COUNTPERPAGE, totalPage, totalCount, strQuery, L"", SORT_ORDER_NONE);
    r = GetLastResult();
    if (IsFailed(r))
    {
        AppLog("ContentSearch.SearchN() failed by %s.", GetErrorMessage(r));
        return scope.Close(v8::Undefined());
    }

    v8::Local<v8::Array> fileLists = v8::Array::New();

    if (pSearchList != null)
    {
        int cnt = 0;
        Tizen::Io::File file;
        for (int i = 0; i < pSearchList->GetCount(); i++) {
            ContentSearchResult* pSearchInfo = (ContentSearchResult*) pSearchList->GetAt(i);
            ContentInfo* pContentInfo = pSearchInfo->GetContentInfo();
            String path = pContentInfo->GetContentPath();

            if (Tizen::Io::File::IsFileExist(path)) {
                String title = pContentInfo->GetContentName();
                String category = pContentInfo->GetCategory();
                char c_path[PATH_MAX];
                char c_category[PATH_MAX];
                snprintf(c_path, PATH_MAX, "%ls", Tizen::Io::File::GetFileName(path).GetPointer());
//                snprintf(c_category, PATH_MAX, "%ls", Tizen::Io::File::GetFileName(path).GetPointer());
                AppLog("title: %ls, path: %s", title.GetPointer(), c_path);

                v8::Local<v8::Object> fileObject = v8::Object::New();
                fileObject->Set(v8::String::New ("type"), v8::String::New ("f")); // Do we need??
                fileObject->Set(v8::String::New("name"), v8::String::New(c_path));
                fileLists->Set(cnt++, fileObject);
            }
        }
    }

    if (!fileLists.IsEmpty()) {
        return scope.Close(fileLists);
    }

    return scope.Close(v8::Undefined());
}

/**
 * Returns a full path with given image name
 */
v8::Handle<v8::Value> Images::getPath(const v8::Arguments& args) {
    AppLog("entered Images::getPath (args length:%d)", args.Length());

    if ( args.Length() < 1 ) {
        AppLog("missing arguments");
        return v8::Undefined();
    }
    v8::HandleScope scope;

    String strQuery = L"";
    int pageNo = 1;
    int totalPage = 0;
    int totalCount = 0;

    strQuery.SetCapacity(STRING_CAPACITY);

    // TODO: album, page, page_view
    if (!args[0].IsEmpty()) {
        // FIXME: make query more specific!
        // Tizen Native App Programming > Programming Guide > Content: Managing Content
        strQuery.Append("ContentFileName LIKE '%");
        strQuery.Append(UNWRAP_STRING( args[0] ).c_str());
        strQuery.Append("%'");
    }

    Tizen::Content::ContentSearch search;
    result r = search.Construct(CONTENT_TYPE_IMAGE);

    if (IsFailed(r))
    {
        AppLog("Failed to create search instance");
    }

    Tizen::Base::Collection::IList*  pSearchList = search.SearchN(pageNo, MAX_CONTENTSEARCH_COUNTPERPAGE, totalPage, totalCount, strQuery, L"", SORT_ORDER_NONE);
    r = GetLastResult();
    if (IsFailed(r))
    {
        AppLog("ContentSearch.SearchN() failed by %s.", GetErrorMessage(r));
        return scope.Close(v8::Undefined());
    }

    if (pSearchList != null)
    {
        AppLog("query: %ls, the cnt: %d", strQuery.GetPointer(), pSearchList->GetCount());
        for (int i = 0; i < pSearchList->GetCount(); i++) {
            ContentSearchResult* pSearchInfo = (ContentSearchResult*) pSearchList->GetAt(i);
            ContentInfo* pContentInfo = pSearchInfo->GetContentInfo();
            String path = pContentInfo->GetContentPath();
            char c_path[PATH_MAX];
            snprintf(c_path, PATH_MAX, "%ls", path.GetPointer());
            AppLog("path: %ls", path.GetPointer());
            v8::Local<v8::Object> fileObject = v8::Object::New();
            fileObject->Set(v8::String::New("path"), v8::String::New(c_path));
            return scope.Close(fileObject);
        }
    }

    return scope.Close(v8::Undefined());
}
