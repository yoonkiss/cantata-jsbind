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

result TizenContents::createContent(String srcPath, String destPath, bool deleteSource, ContentId &newContentId) {
    result r = E_SUCCESS;

    ContentManager contentManager;
    r = contentManager.Construct();
    if (IsFailed(r)) {
        AppLog("Failed to create content manager");
        return r;
    }

    newContentId = contentManager.CreateContent( srcPath, destPath, deleteSource, null );
    r = GetLastResult();
    if ( IsFailed( r ) ) {
        AppLog("Failed to create content");
        return r;
    }

    return E_SUCCESS;
}

result TizenContents::removeContent(Tizen::Content::ContentId id) {
    result r = E_SUCCESS;

    ContentManager contentManager;
    r = contentManager.Construct();
    if ( IsFailed( r ) ) {
        AppLog("Failed to create content manager");
        return r;
    }

    r = contentManager.DeleteContent( id );
    if ( IsFailed( r ) )
    {
        AppLog("Failed to delete the content: %s", GetErrorMessage( GetLastResult() ) );
        return r;
    }

    return E_SUCCESS;
}

result TizenContents::removeContent(Tizen::Content::ContentType type, Tizen::Base::String path) {
    result r = E_SUCCESS;

    ContentManager contentManager;
    r = contentManager.Construct();
    if (IsFailed(r)) {
        AppLog("Failed to create content manager");
        return r;
    }

    ContentId contentid;
    r = getContentId( type, path, contentid );
    if (IsFailed(r)) {
        AppLog("Failed to get content id");
        return r;
    } else {
        r = contentManager.DeleteContent( contentid );
        if ( IsFailed( r ) )
        {
           AppLog( "Failed to delete the content: %s", GetErrorMessage( GetLastResult() ) );
           return r;
        }
    }

    return E_SUCCESS;
}

result TizenContents::moveContent(String srcPath, String destPath, ContentId &newContentId) {
    result r = E_SUCCESS;

    if( !Tizen::Io::File::IsFileExist( srcPath ) ) {
        AppLog( "File is not existed" );
        SetLastResult( E_FILE_NOT_FOUND );
        return E_FILE_NOT_FOUND;
    }

    if( destPath.CompareTo( srcPath ) == 0 ) {
        AppLog( "Failed to create same file" );
        SetLastResult( E_FILE_ALREADY_EXIST );;
        return E_FILE_ALREADY_EXIST;
    }

    if( Tizen::Io::File::IsFileExist( destPath ) ) {
        AppLog( "File is already existed" );
        SetLastResult( E_FILE_ALREADY_EXIST );
        return E_FILE_ALREADY_EXIST;
    }

    r = createContent( srcPath, destPath, true, newContentId );

    return r;
}

result TizenContents::moveContent(String srcPath, String destPath) {
    result r = E_SUCCESS;

    if( !Tizen::Io::File::IsFileExist( srcPath ) ) {
        AppLog( "File is not existed" );
        SetLastResult( E_FILE_NOT_FOUND );
        return E_FILE_NOT_FOUND;
    }

    if( destPath.CompareTo( srcPath ) == 0 ) {
        AppLog( "Failed to create same file" );
        SetLastResult( E_FILE_ALREADY_EXIST );;
        return E_FILE_ALREADY_EXIST;
    }

    if( Tizen::Io::File::IsFileExist( destPath ) ) {
        AppLog( "File is already existed" );
        SetLastResult( E_FILE_ALREADY_EXIST );
        return E_FILE_ALREADY_EXIST;
    }

    ContentId id;
    r = createContent( srcPath, destPath, true, id );

    return r;
}

result TizenContents::getContentId(ContentType type, String path, ContentId &id) {
    result r = E_SUCCESS;

    if ( path.IsEmpty() ) {
        AppLog("Failed to getContentId: wrong path value inputed");
        return E_FAILURE;
    }

    ContentSearch search;
    r = search.Construct( type );
    if ( IsFailed( r ) ) {
        AppLog("Failed to create ContentSearch: %s", GetErrorMessage( r ) );
        return r;
    }

    int totalPageCount = 0;
    int totalCount = 0;
    IList *pContentInfoList = search.SearchN(1,1,totalPageCount,totalCount,L"",L"",SORT_ORDER_NONE); // all content
    r = GetLastResult();
    if ( IsFailed( r ) || pContentInfoList == null || totalCount == 0 ) {
        AppLog("Failed to get content list");
        return r;
    }
    // free
    TRY_DELETE( pContentInfoList );
    // get all content in one page
    pContentInfoList = search.SearchN(1,totalCount,totalPageCount,totalCount,L"",L"",SORT_ORDER_NONE); // all content
    IEnumerator *pEnum = pContentInfoList->GetEnumeratorN(); // must free
    bool bfind = false;
    while ( pEnum->MoveNext() == E_SUCCESS ) {
        ContentSearchResult *pSearchResult = static_cast<ContentSearchResult*>(pEnum->GetCurrent());
        ContentInfo *pInfo = pSearchResult->GetContentInfo();

        if ( type == pInfo->GetContentType() &&
                path.Equals( pInfo->GetContentPath() ) ) {
            id = pInfo->GetContentId();
            bfind = true;
            break;
        }
    }

    // free
    TRY_DELETE( pEnum );
    TRY_DELETE( pContentInfoList );

    if ( !bfind ) {
        return E_FAILURE;
    }

    return E_SUCCESS;
}


result TizenContents::getContentId(String path, ContentId &id) {
    result r = E_SUCCESS;

    if ( path.IsEmpty() ) {
        AppLog("Failed to getContentId: wrong path value inputed");
        return E_FAILURE;
    }

    ContentSearch search;
    r = search.Construct( CONTENT_TYPE_ALL );
    if ( IsFailed( r ) ) {
        AppLog("Failed to create ContentSearch: %s", GetErrorMessage( r ) );
        return r;
    }

    int totalPageCount = 0;
    int totalCount = 0;
    IList *pContentInfoList = search.SearchN(1,1,totalPageCount,totalCount,L"",L"",SORT_ORDER_NONE); // all content
    r = GetLastResult();
    if ( IsFailed( r ) || pContentInfoList == null || totalCount == 0 ) {
        AppLog("Failed to get content list");
        return r;
    }
    // free
    TRY_DELETE( pContentInfoList );
    // get all content in one page
    pContentInfoList = search.SearchN(1,totalCount,totalPageCount,totalCount,L"",L"",SORT_ORDER_NONE); // all content
    IEnumerator *pEnum = pContentInfoList->GetEnumeratorN(); // must free
    bool bfind = false;
    while ( pEnum->MoveNext() == E_SUCCESS ) {
        ContentSearchResult *pSearchResult = static_cast<ContentSearchResult*>(pEnum->GetCurrent());
        ContentInfo *pInfo = pSearchResult->GetContentInfo();

        if ( path.Equals( pInfo->GetContentPath() ) ) {
            id = pInfo->GetContentId();
            bfind = true;
            break;
        }
    }

    // free
    TRY_DELETE( pEnum );
    TRY_DELETE( pContentInfoList );

    if ( !bfind ) {
        return E_FAILURE;
    }

    return E_SUCCESS;
}

result TizenContents::getContentPath(ContentId contentId, String &path) {
    result r = E_SUCCESS;

    ContentManager contentManager;
    r = contentManager.Construct();
    if( IsFailed( r ) ) {
        AppLog("Failed to create content manager");
        return r;
    }

    ContentInfo *pInfo = contentManager.GetContentInfoN( contentId ); // must free
    if ( IsFailed( GetLastResult() ) ) {
        AppLog("Failed to GetContentInfoN");
        TRY_DELETE( pInfo ); // free
        return E_FAILURE;
    }
    path = pInfo->GetContentPath();
    r = GetLastResult();

    // free
    TRY_DELETE( pInfo );

    return r;
}

IList* TizenContents::getAllContentsListN(Tizen::Content::ContentType contentType) {
    ContentSearch search;
    search.Construct( contentType );
    if ( IsFailed( GetLastResult() ) ) {
        return null;
    }

    int totalPageCount = 0;
    int totalCount = 0;
    IList *pContentInfoList = search.SearchN(1,1,totalPageCount,totalCount,L"",L"",SORT_ORDER_NONE);
    if ( pContentInfoList == null || totalCount == 0 ) {
        return null;
    } else {
        delete pContentInfoList;
        pContentInfoList = null;
    }

    pContentInfoList = search.SearchN(1,totalCount,totalPageCount,totalCount,L"",L"",SORT_ORDER_NONE);
    if ( pContentInfoList == null || totalCount == 0 ) {
        return null;
    }
    return pContentInfoList;
}

IList* TizenContents::getAllContentsListForColumnN(Tizen::Content::ContentType contentType, Tizen::Base::String column) {
    if ( column == null || column.IsEmpty() ) {
        return TizenContents::getAllContentsListN( contentType );
    }

    ContentSearch search;
    search.Construct( contentType );
    if ( IsFailed( GetLastResult() ) ) {
        return null;
    }

    int totalPageCount = 0;
    int totalCount = 0;
    IList *pContentInfoList = search.SearchN(1, 1, totalPageCount, totalCount, L"", column, SORT_ORDER_NONE);
    if ( pContentInfoList == null || totalCount == 0 ) {
        return null;
    } else {
        delete pContentInfoList;
        pContentInfoList = null;
    }

    pContentInfoList = search.SearchN(1, totalCount, totalPageCount, totalCount, L"", column, SORT_ORDER_NONE);
    if ( pContentInfoList == null || totalCount == 0 ) {
        return null;
    }
    return pContentInfoList;
}

