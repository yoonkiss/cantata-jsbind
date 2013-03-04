#include <v8.h>
#include <node.h>
#include <FBase.h>
#include <FSystem.h>
#include <iostream>
#include <FContent.h>
#include <FIo.h>

#include "contents.h"

using namespace Tizen::Base;
using namespace Tizen::Base::Collection;
using namespace Tizen::Content;

TizenContents::TizenContents(Tizen::Content::ContentType type)
: __pageNo(1)
, __countPerPage(1)
, __sortOrder(SORT_ORDER_NONE)
{
    __contentType = type;

}

TizenContents::TizenContents(Tizen::Content::ContentType type, int pageNo, int countPerPage)
: __sortOrder(SORT_ORDER_NONE)
{
    __contentType = type;
    __pageNo = pageNo;
    __countPerPage = countPerPage;
}

TizenContents::TizenContents(Tizen::Content::ContentType type, Tizen::Base::SortOrder sortOrder)
: __pageNo(1)
, __countPerPage(1)
{
    __contentType = type;
    __sortOrder = sortOrder;
}

TizenContents::~TizenContents(void)
{
    AppLog("called TizenContents disstructor");
}

IList* TizenContents::getContentSearchList(Tizen::Base::String whereExpr) {
    Tizen::Content::ContentSearch search;
    result r = search.Construct(__contentType);

    if (IsFailed(r))
    {
        AppLog("Failed to create search instance");
        return null;
    }

    IList* pContentInfoList = search.SearchN(__pageNo, __countPerPage, __totalPageCount, __totalCount, whereExpr, L"", __sortOrder);
    r = GetLastResult();
    if (IsFailed(r)) {
        AppLog("%s: pageno: %d, perpage: %d, totalpage: %d, totalcount: %d, where: %s",
                    GetErrorMessage(r)
                    ,__pageNo, __countPerPage, __totalPageCount, __totalCount, whereExpr.GetPointer());
    }
    SetLastResult(r);
    return pContentInfoList;
}

IList* TizenContents::getContentDirectoryList() {
    ContentDirectory directory;
    result r = directory.Construct(__contentType);
    if (IsFailed(r))
    {
      AppLog("Failed to get content directory: %s", GetErrorMessage(GetLastResult()));
      return null;
    }

    IList* pContentInfoList = directory.GetContentDirectoryPathListN(__sortOrder);

    r = GetLastResult();
    if (IsFailed(r)) {
        AppLog("Failed to get pathlist: %s", GetErrorMessage(r));
    }
    SetLastResult(r);

    return pContentInfoList;
}

IList* TizenContents::getContentDirectoryItemList(String contentDirectoryPath) {
    ContentDirectory directory;
    result r = directory.Construct(__contentType);
    if (IsFailed(r))
    {
      AppLog("Failed to get content directory: %s", GetErrorMessage(GetLastResult()));
      return null;
    }

    IList* pContentInfoList = directory.GetContentDirectoryItemListN(contentDirectoryPath, __pageNo, __countPerPage, L"", __sortOrder);
    r = GetLastResult();
    if (IsFailed(r)) {
        AppLog("Failed to get directory item: %s", GetErrorMessage(r));
    }
    SetLastResult(r);
    return pContentInfoList;
}
