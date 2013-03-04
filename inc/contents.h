#ifndef __CONTENTS__
#define __CONTENTS__

#include <FBase.h>
#include <FContent.h>
#include <FSystem.h>

class TizenContents {
public:
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
#endif
