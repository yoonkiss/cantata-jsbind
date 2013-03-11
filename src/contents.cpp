#include <v8.h>
#include <node.h>
#include <FBase.h>
#include <FSystem.h>
#include <iostream>
#include <FMedia.h>
#include <FContent.h>
#include <FIo.h>

#include "contents.h"

using namespace Tizen::Base;
using namespace Tizen::Base::Collection;
using namespace Tizen::Content;
using namespace Tizen::Media;
/*

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
    if (IsFailed(r)) {
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
*/

result TizenContents::removeContent(ContentType contentType, String contentUriPath) {
    result r = E_SUCCESS;

    ContentManager contentManager;
    r = contentManager.Construct();
    if (IsFailed(r)) {
        AppLog("Failed to create content manager");
        return r;
    }
    ContentId contentid;
    String contentFullPath = L"";
    r = getContentPath(contentType, contentUriPath, contentFullPath);
    if (!IsFailed(r)) {
        r = getContentId(contentFullPath, contentid);
        if (!IsFailed(r)) {
            r = contentManager.DeleteContent(contentid);
            if (IsFailed(r))
            {
               AppLog("Failed to delete the content: %s", GetErrorMessage(GetLastResult()));
               return r;
            }
        }
    }
    return E_SUCCESS;
}

result TizenContents::moveContent(ContentType contentType, String srcUriPath, String destUriPath) {
    result r = E_SUCCESS;

    String srcFullPath = L"";
    String destFullPath = L"";

    r = getContentPath(contentType, srcUriPath, srcFullPath);
    if (IsFailed(r)) {
        AppLog("Failed to get source content path");
        return r;
    }

    r = getContentPath(contentType, destUriPath, destFullPath);
    if (IsFailed(r)) {
        AppLog("Failed to get destination content path");
        return r;
    }

    if (!Tizen::Io::File::IsFileExist(srcFullPath)) {
        AppLog("File is not existed");
        return GetLastResult();
    }

    if (destFullPath.CompareTo(srcFullPath) == 0) {
        AppLog("Failed to create same file");
        return E_FAILURE;
    }

    if (Tizen::Io::File::IsFileExist(destFullPath)) {
        AppLog("File is already existed");
        SetLastResult(E_FILE_ALREADY_EXIST);
        return GetLastResult();
    }

    ContentManager contentManager;
    r = contentManager.Construct();
    if (IsFailed(r)) {
        AppLog("Failed to create content manager");
        return r;
    }
    ContentId contentId = contentManager.CreateContent(srcFullPath, destFullPath, true, null);
    return GetLastResult();
}

result TizenContents::getContentId(String contentPath, ContentId &contentId) {
    result r = E_SUCCESS;

    ContentManager contentManager;
    r = contentManager.Construct();
    if (IsFailed(r)) {
        AppLog("Failed to create content manager");
        return r;
    }

    ContentType contentType = ContentManagerUtil::CheckContentType(contentPath);
    if (contentType == CONTENT_TYPE_IMAGE) {
        ImageContentInfo sourceContentInfo;
        r = sourceContentInfo.Construct(&contentPath);
        if (IsFailed(r)) {
            AppLog("Failed to get source contentInfo");
            return r;
        }
        contentId = contentManager.CreateContent(sourceContentInfo);
    } else if (contentType == CONTENT_TYPE_AUDIO) {
        AudioContentInfo sourceContentInfo;
        r = sourceContentInfo.Construct(&contentPath);
        if (IsFailed(r)) {
            AppLog("Failed to get source contentInfo");
            return r;
        }
        contentId = contentManager.CreateContent(sourceContentInfo);
    } else if (contentType == CONTENT_TYPE_VIDEO) {
        AudioContentInfo sourceContentInfo;
        r = sourceContentInfo.Construct(&contentPath);
        if (IsFailed(r)) {
            AppLog("Failed to get source contentInfo");
            return r;
        }
        contentId = contentManager.CreateContent(sourceContentInfo);
    } else {
        AppLog("Not supporting type");
        SetLastResult(E_FAILURE);
        return r;
    }
    return E_SUCCESS;
}

result TizenContents::getContentPath(ContentType contentType, String uri, String &path) {
    // TODO: Not implements

    return E_SUCCESS;

#if 0
    int pageNo = 1;
    int countPerPage = TizenContents::MAX_CONTENTSEARCH_COUNTPERPAGE;
    int totalPageCount = 0, totalCount = 0;

    ContentSearch search;
    result r = search.Construct(contentType);

    if (IsFailed(r)) {
        AppLog("Failed to create search instance");
        return r;
    }
    String whereExpr = L"";
    Tizen::Base::Utility::StringTokenizer st(uri, L"/");
    String token;
    while (st.HasMoreTokens())
    {
        st.GetNextToken(token);
    }

    String contentName = Tizen::Io::File::GetFileName(uri);

    whereExpr.Format(TizenContents::STRING_CAPACITY, L"ContentFileName = '%ls'", contentName.GetPointer());
    IList* pContents = search.SearchN(pageNo, countPerPage, totalPageCount, totalCount, whereExpr, L"", SORT_ORDER_NONE);

    if (!IsFailed(GetLastResult()) && pContents != null && pContents->GetCount() > 0) {

        IEnumerator* pContentEnum = pContents->GetEnumeratorN();
        while (pContentEnum->MoveNext() == E_SUCCESS) {
            ContentSearchResult* pSearchInfo = static_cast<ContentSearchResult*>(pContentEnum->GetCurrent());
            ContentInfo* pContentInfo = static_cast<ContentInfo*>(pSearchInfo->GetContentInfo());
            String fileName = Tizen::Io::File::GetFileName(pContentInfo->GetContentPath());

            playArray->Set(cnt++, v8::String::New(Util::toAnsi(buf, fileName, STRING_MAX)));
        }
        pContents->RemoveAll();
        delete pContents;
    }
    return SetLastResult(E_FILE_NOT_FOUND);
#endif
}

