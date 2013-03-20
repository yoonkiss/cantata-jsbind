#ifndef __CONTENTS__
#define __CONTENTS__

#include <FBase.h>
#include <FContent.h>
#include <FSystem.h>

#include "tizenair-common.h"
#include "tizenair-util.h"

class TizenContents {
public:
/*
    TizenContents(Tizen::Content::ContentType type);
    TizenContents(Tizen::Content::ContentType type, int pageNo, int countPerPage);
    TizenContents(Tizen::Content::ContentType type, Tizen::Base::SortOrder sortOrder);
    virtual ~TizenContents(void);

    Tizen::Base::Collection::IList* getContentSearchList(Tizen::Base::String whereExpr);
    Tizen::Base::Collection::IList* getContentDirectoryList();
    Tizen::Base::Collection::IList* getContentDirectoryItemList(Tizen::Base::String contentDirectoryPath);

    void    setPageNo(int pageNo);
    void    setCoutPerPage(int coutPerPage);
    int     getTotalPageCount() const;
    int     getTotalCount() const;
*/
    static result createContent(Tizen::Base::String srcPath, Tizen::Base::String destPath, bool deleteSource, Tizen::Content::ContentId &newContentId);
    static result removeContent(Tizen::Content::ContentId id);
    static result removeContent(Tizen::Content::ContentType type, Tizen::Base::String path);
    static result moveContent(Tizen::Base::String srcPath, Tizen::Base::String destPath, Tizen::Content::ContentId &newContentId);
    static result moveContent(Tizen::Base::String srcPath, Tizen::Base::String destPath);

    /**
     * Gets content id from given type and path<br>
     * path is souces's full path<br>
     */
    static result getContentId(Tizen::Content::ContentType type, Tizen::Base::String path, Tizen::Content::ContentId &id);

    /**
     * Gets content id from the given full source path.
     */
    static result getContentId(Tizen::Base::String path, Tizen::Content::ContentId &id);

    /**
     * Gets contents full path from the given id<br>
     * path is souces's full path<br>
     */
    static result getContentPath(Tizen::Content::ContentId contentId, Tizen::Base::String &path);

    /**
     * Gets All ContentSearchResult List from the given content type
     */
    static Tizen::Base::Collection::IList* getAllContentsListN(Tizen::Content::ContentType contentType);

    /**
     * Gets All ContentSearchResult List from the given content type and column<br>
     * If column is null then same result of getAllContentListN method
     */
    static Tizen::Base::Collection::IList* getAllContentsListForColumnN(Tizen::Content::ContentType contentType, Tizen::Base::String column);
    static const int STRING_CAPACITY = 1024;
    static const int MAX_CONTENTSEARCH_COUNTPERPAGE = 50;

private:
    //result getContentId(String contentPath, ContentId &contentId);

    Tizen::Content::ContentType __contentType;
    int __pageNo;
    int __countPerPage;
    int __totalPageCount;
    int __totalCount;
    Tizen::Base::String __whereExpr;
    Tizen::Base::SortOrder __sortOrder;

};

static const Tizen::Base::String CONTENT_INFO_ORDER = L"Title"; //L"ContentFileName";
static const Tizen::Base::String STORAGE_PHONE_PATH     = Tizen::System::Environment::GetMediaPath() + L"Sounds";
static const Tizen::Base::String STORAGE_SDCARD_PATH    = Tizen::System::Environment::GetExternalStoragePath();
static const Tizen::Base::String STORAGE_EXTERNAL_PATH  = Tizen::System::Environment::GetMediaPath();
static const Tizen::Base::String DEFAULT_PLAYLIST = L"all";
static const Tizen::Base::String DIRECTORY_SEPARATOR = L"/";
#endif
