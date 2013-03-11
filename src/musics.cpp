#include <v8.h>
#include <node.h>
#include <stdlib.h>
#include <iostream>
#include <FIo.h>
#include <FBaseSysLog.h>
#include <FBase.h>
#include <FIo.h>


#include "contents.h"
#include "musics.h"
#include "tizenair-util.h"

using namespace Tizen::Base::Collection;
using namespace Tizen::Content;

void NODE_EXTERN Musics::Init(v8::Handle<v8::Object> target) {
    AppLog("Entered Musics::Init");

    // strange?
    v8::Local<v8::FunctionTemplate> funcTemplate = v8::Local<v8::FunctionTemplate>::New(v8::FunctionTemplate::New(Musics::New));

    funcTemplate->SetClassName(v8::String::NewSymbol("Musics"));
    funcTemplate->Set(v8::String::NewSymbol("getPlaylistNames"), v8::FunctionTemplate::New(getPlaylistNames)->GetFunction());
    funcTemplate->Set(v8::String::NewSymbol("getPlaylist"), v8::FunctionTemplate::New(getPlaylist)->GetFunction());
    funcTemplate->Set(v8::String::NewSymbol("post"), v8::FunctionTemplate::New(post)->GetFunction());
    funcTemplate->Set(v8::String::NewSymbol("remove"), v8::FunctionTemplate::New(remove)->GetFunction());
    funcTemplate->Set(v8::String::NewSymbol("moveTo"), v8::FunctionTemplate::New(moveTo)->GetFunction());

    funcTemplate->Set(v8::String::NewSymbol("getAllMusicInfo"), v8::FunctionTemplate::New(getAllMusicInfo)->GetFunction());
    funcTemplate->Set(v8::String::NewSymbol("getAllMusicInfoToAlbum"), v8::FunctionTemplate::New(getAllMusicInfoToAlbum)->GetFunction());
    funcTemplate->Set(v8::String::NewSymbol("getAllMusicInfoToArtist"), v8::FunctionTemplate::New(getAllMusicInfoToArtist)->GetFunction());
    funcTemplate->Set(v8::String::NewSymbol("getAllMusicInfoToComposer"), v8::FunctionTemplate::New(getAllMusicInfoToComposer)->GetFunction());
    funcTemplate->Set(v8::String::NewSymbol("getAllMusicInfoToGenre"), v8::FunctionTemplate::New(getAllMusicInfoToGenre)->GetFunction());

    target->Set(v8::String::NewSymbol("Musics"), funcTemplate->GetFunction());
}

v8::Handle<v8::Value> Musics::New(const v8::Arguments& args) {
    v8::HandleScope scope;
    // nothing to do new()
    return args.This();
}

v8::Handle<v8::Value> Musics::getPlaylistNames(const v8::Arguments& args) {
    AppLog("Entered Musics::getPlaylistNames (args length:%d)", args.Length());
    if ( args.Length() > 0) {
        AppLog("Bad parameters");
        return v8::ThrowException(v8::String::New("Bad parameters"));
    }
    v8::HandleScope scope;
    int cnt = 0;
    char buf[STRING_MAX];
    v8::Local<v8::Array> playArray = v8::Array::New();
    playArray->Set(cnt++, v8::String::New(Util::toAnsi(buf, DEFAULT_PLAYLIST, STRING_MAX)));

    IList* pPlayList = PlayListManager::GetInstance()->GetAllPlayListNameN();

   if (IsFailed(GetLastResult())) {
      AppLog("Failed to get playlist name: %s", GetErrorMessage(GetLastResult()));
      return scope.Close(playArray);
   }

   if (pPlayList->GetCount()) {
       IEnumerator* pListEnum = pPlayList->GetEnumeratorN();
       while (pListEnum->MoveNext() == E_SUCCESS) {
           String* playName = static_cast<String*>(pListEnum->GetCurrent());
           Util::toAnsi(buf, *playName, STRING_MAX);
           playArray->Set(cnt++, v8::String::New(buf));
       }
   }

   pPlayList->RemoveAll(true);
   delete pPlayList;

   return scope.Close(playArray);
#if 0
    int category = 0;
    if(!Util::isArgumentNull(args[0])) {
        category = args[0]->NumberValue();
    }

    // FIXME:
    category = 0; // stands for all lists
    v8::HandleScope scope;

    ContentDirectory directory;
    Tizen::Base::String contentDirectoryPath = STORAGE_PHONE_PATH;
    result r = E_SUCCESS;
    /*IListT< ContentType >* pListT = new (std::nothrow) ArrayListT<ContentType>();
    pListT->Add(CONTENT_TYPE_IMAGE);
    pListT->Add(CONTENT_TYPE_VIDEO);
    result r = directory.Construct(*pListT);
    delete pListT;*/
    r = directory.Construct(CONTENT_TYPE_AUDIO);
    if (IsFailed(r))
    {
        AppLog("Failed to get content directory: %s", GetErrorMessage(GetLastResult()));
        return scope.Close(v8::Null());
    }

    int pageNo = 1;
    int countPerPage = directory.GetContentDirectoryItemCount(contentDirectoryPath);
    countPerPage = 5;
    IList* pContentInfoList = directory.GetContentDirectoryItemListN(contentDirectoryPath, pageNo, countPerPage, CONTENT_INFO_ORDER, SORT_ORDER_ASCENDING);

    if ((pContentInfoList != null) && (pContentInfoList->GetCount() > 0))
    {
        IEnumerator* pEnum = pContentInfoList->GetEnumeratorN();

        v8::Local<v8::Object> musicObject = v8::Object::New();
        v8::Local<v8::Array> musicInfoList = v8::Array::New();
        musicObject->Set(v8::String::New("total"), v8::Number::New(pContentInfoList->GetCount()));
        musicObject->Set(v8::String::New("category"), v8::String::New("all"));
        musicObject->Set(v8::String::New("page"), v8::Number::New(pageNo));
        musicObject->Set(v8::String::New("countperpage"), v8::Number::New(countPerPage));
        int cnt = 0;
        while (pEnum->MoveNext() == E_SUCCESS)
        {
            char buf[STRING_MAX];
            ContentInfo* pContentInfo =  static_cast<ContentInfo*>(pEnum->GetCurrent());
            AudioContentInfo* pAudioContentInfo = static_cast<AudioContentInfo*>(pContentInfo);
            v8::Local<v8::Object> musicInfoObject = v8::Object::New();

            musicInfoObject->Set(v8::String::New("title"), v8::String::New(Util::toCString(buf, pAudioContentInfo->GetTitle(), STRING_MAX)));
            musicInfoObject->Set(v8::String::New("artist"), v8::String::New(Util::toCString(buf, pAudioContentInfo->GetArtist(), STRING_MAX)));
            musicInfoObject->Set(v8::String::New("duration"), v8::Number::New(pAudioContentInfo->GetDuration()));
            musicInfoObject->Set(v8::String::New("path"), v8::String::New(Util::toCString(buf, pContentInfo->GetContentPath(), STRING_MAX)));
            musicInfoList->Set(cnt++, musicInfoObject);
        }
        musicObject->Set(v8::String::New("name"), musicInfoList);
        delete pEnum;
        delete pContentInfoList;
        return scope.Close(musicObject);
    }

    delete pContentInfoList;
    return scope.Close(v8::Null());
#endif
}

v8::Handle<v8::Value> Musics::getPlaylist(const v8::Arguments& args) {
    AppLog("Entered Musics::getPlaylist (args length:%d)", args.Length());

    if ( args.Length() < 1 || Util::isArgumentNull(args[0])) {
        AppLog("Bad parameters");
        return v8::ThrowException(v8::String::New("Bad parameters"));
    }
    v8::HandleScope scope;
    int pageNo = 1;
    int cnt = 0;
    int countPerPage = TizenContents::MAX_CONTENTSEARCH_COUNTPERPAGE;
    int totalPageCount = 0, totalCount = 0;
    String playName = UNWRAP_STRING(args[0]).c_str();

    ContentSearch search;
    result r = search.Construct(CONTENT_TYPE_AUDIO);

    if (IsFailed(r))
    {
        AppLog("Failed to create search instance");
        return v8::Null();
    }
    v8::Local<v8::Array> playArray = v8::Array::New();
    IList* pAudioContents = null;
    char buf[STRING_MAX];
    if (playName.Equals(DEFAULT_PLAYLIST)) {
        pAudioContents = search.SearchN(pageNo, countPerPage, totalPageCount, totalCount, L"", L"", SORT_ORDER_NONE);
        if (!IsFailed(GetLastResult()) && pAudioContents != null && pAudioContents->GetCount() > 0) {
            IEnumerator* pAudioEnum = pAudioContents->GetEnumeratorN();
            while (pAudioEnum->MoveNext() == E_SUCCESS) {
                ContentSearchResult* pSearchInfo = static_cast<ContentSearchResult*>(pAudioEnum->GetCurrent());
                ContentInfo* pContentInfo = static_cast<ContentInfo*>(pSearchInfo->GetContentInfo());
                String fileName = Tizen::Io::File::GetFileName(pContentInfo->GetContentPath());
                playArray->Set(cnt++, v8::String::New(Util::toAnsi(buf, fileName, STRING_MAX)));
            }
            pAudioContents->RemoveAll();
            delete pAudioContents;
        }
    } else {
        PlayList* pPlaylist = PlayListManager::GetInstance()->GetPlayListN(playName);
        if (IsFailed(GetLastResult())) {
            AppLog("Failed to get playlist: %s", GetErrorMessage(GetLastResult()));
        } else {
            pAudioContents = pPlaylist->GetContentInfoListN();
            if (!IsFailed(GetLastResult()) && pAudioContents != null && pAudioContents->GetCount() > 0) {
                IEnumerator* pAudioEnum = pAudioContents->GetEnumeratorN();
                while (pAudioEnum->MoveNext() == E_SUCCESS) {
                    ContentInfo* pContentInfo = static_cast<ContentInfo*>(pAudioEnum->GetCurrent());
                    String fileName = Tizen::Io::File::GetFileName(pContentInfo->GetContentPath());
                    playArray->Set(cnt++, v8::String::New(Util::toAnsi(buf, fileName, STRING_MAX)));
                }
                pAudioContents->RemoveAll();
                delete pAudioContents;
            }
            delete pPlaylist;
        }
    }
    return scope.Close(playArray);
}

v8::Handle<v8::Value> Musics::post( const v8::Arguments& args ) {
    AppLog("Entered Musics::post (args length:%d)", args.Length());

    if ( args.Length() < 1 || Util::isArgumentNull(args[0])) {
        AppLog("Bad parameters");
        return v8::ThrowException(v8::String::New("Bad parameters"));
    }
    v8::HandleScope scope;
    result r = E_SUCCESS;
    Tizen::Base::String contentPath(UNWRAP_STRING(args[0]).c_str());

    ContentId contentId;
    r = TizenContents::getContentId(contentPath, contentId);
    if (IsFailed(r))
    {
       return scope.Close(v8::Boolean::New(false));
    }
    AppLog("Success post:%ls", contentPath.GetPointer());
    return scope.Close(v8::Boolean::New(true));
}

v8::Handle<v8::Value> Musics::remove( const v8::Arguments& args ) {
    AppLog("Entered Musics::remove (args length:%d)", args.Length());

    if ( args.Length() < 1 || Util::isArgumentNull(args[0])) {
        AppLog("Bad parameters");
        return v8::ThrowException(v8::String::New("Bad parameters"));
    }
    v8::HandleScope scope;
    result r = E_SUCCESS;
    Tizen::Base::String contentPath(UNWRAP_STRING(args[0]).c_str());

    r = TizenContents::removeContent(CONTENT_TYPE_AUDIO, contentPath);
    if (IsFailed(r))
    {
       return scope.Close(v8::Boolean::New(false));
    }
    AppLog("Success remove:%ls", contentPath.GetPointer());
    return scope.Close(v8::Boolean::New(true));
}

v8::Handle<v8::Value> Musics::moveTo( const v8::Arguments& args ) {
    AppLog("Entered Musics::remove (args length:%d)", args.Length());

    v8::HandleScope scope;
    if ( args.Length() < 2 || Util::isArgumentNull(args[0]) || Util::isArgumentNull(args[1])) {
        AppLog("Bad parameters");
        return v8::ThrowException(v8::String::New("Bad parameters"));
    }

    String srcPath = UNWRAP_STRING(args[0]).c_str();
    String destPath = UNWRAP_STRING(args[1]).c_str();

    result r = TizenContents::moveContent(CONTENT_TYPE_AUDIO, srcPath, destPath);
    if (IsFailed(r)) {
        return scope.Close(v8::Boolean::New(false));
    }
    AppLog("Success move:%ls -> %ls", srcPath.GetPointer(), destPath.GetPointer());
    return scope.Close(v8::Boolean::New(true));
}

v8::Handle<v8::Value> Musics::getAllMusicInfo(const v8::Arguments& args) {
    AppLog("Entered Musics::getAllMusicInfo");
    v8::HandleScope scope;
    v8::Local<v8::Object> musicInfoList = v8::Object::New();
    v8::Local<v8::Array> musicInfoArray = v8::Array::New(); // [ ..., ... ]

    char *pResult = null;
    IList *pMusicInfoList = TizenContents::getAllContentsListN( CONTENT_TYPE_AUDIO ); // must free
    if ( pMusicInfoList == null ) {
        return scope.Close( v8::Undefined() );
    }

    int count = 0;
    IEnumerator *pEnum = pMusicInfoList->GetEnumeratorN(); // must free
    while ( pEnum->MoveNext() == E_SUCCESS ) {
        ContentSearchResult *pSearchResult = static_cast<ContentSearchResult*>(pEnum->GetCurrent());
        ContentInfo *pInfo = pSearchResult->GetContentInfo();
        AudioContentInfo *pAudioInfo = null;
        if ( pInfo->GetContentType() == CONTENT_TYPE_AUDIO ) {
            pAudioInfo = static_cast<AudioContentInfo*>(pInfo);
        }

        if ( pAudioInfo != null ) {
            v8::Local<v8::Object> musicInfo = v8::Object::New();

            String id = pAudioInfo->GetContentId().ToString();
            String path = pAudioInfo->GetContentPath();
            String albumName = pAudioInfo->GetAlbumName();
            String artist = pAudioInfo->GetArtist();
            String composer = pAudioInfo->GetComposer();
            String copyrigth = pAudioInfo->GetCopyright();
            String genre = pAudioInfo->GetGenre();
            String title = pAudioInfo->GetTitle();
            String trackInfo = pAudioInfo->GetTrackInfo();
            String rating = pAudioInfo->GetRating();
            int bitrate = pAudioInfo->GetBitrate();
            int releaseYear = pAudioInfo->GetReleaseYear();

            // set Music id
            pResult = Util::toAnsi( id );
            musicInfo->Set( v8::String::New( "id" ), v8::String::New( pResult ) );
            delete pResult;

            // set Music path
            pResult = Util::toAnsi( path );
            musicInfo->Set( v8::String::New( "path" ), v8::String::New( pResult ) );
            delete pResult;

            // set Music albumName
            pResult = Util::toAnsi( albumName );
            musicInfo->Set( v8::String::New( "album" ), v8::String::New( pResult ) );
            delete pResult;

            // set Music artist
            pResult = Util::toAnsi( artist );
            musicInfo->Set( v8::String::New( "artist" ), v8::String::New( pResult ) );
            delete pResult;

            // set Music composer
            pResult = Util::toAnsi( composer );
            musicInfo->Set( v8::String::New( "composer" ), v8::String::New( pResult ) );
            delete pResult;

            // set Music copyright
            pResult = Util::toAnsi( copyrigth );
            musicInfo->Set( v8::String::New( "copyrigth" ), v8::String::New( pResult ) );
            delete pResult;

            // set Music genre
            pResult = Util::toAnsi( genre );
            musicInfo->Set( v8::String::New( "genre" ), v8::String::New( pResult ) );
            delete pResult;

            // set Music title
            pResult = Util::toAnsi( title );
            musicInfo->Set( v8::String::New( "title" ), v8::String::New( pResult ) );
            delete pResult;

            // set Music trackInfo
            pResult = Util::toAnsi( trackInfo );
            musicInfo->Set( v8::String::New( "track-info" ), v8::String::New( pResult ) );
            delete pResult;

            // set Music rating
            pResult = Util::toAnsi( rating );
            musicInfo->Set( v8::String::New( "rating" ), v8::String::New( pResult ) );
            delete pResult;

            // set Music bitrate
            pResult = Util::toAnsi( bitrate );
            musicInfo->Set( v8::String::New( "bitrate" ), v8::String::New( pResult ) );
            delete pResult;

            // set Music release year
            pResult = Util::toAnsi( releaseYear );
            musicInfo->Set( v8::String::New( "release-year" ), v8::String::New( pResult ) );
            delete pResult;

            // set array
            musicInfoArray->Set( count++, musicInfo );
        }
    }

    // set info
    pResult = Util::toAnsi( count );
    musicInfoList->Set( v8::String::New( "musicInfo-list-length" ), v8::String::New( pResult ) );
    delete pResult;
    musicInfoList->Set( v8::String::New( "musicInfo-list" ), musicInfoArray );

    // free
    TRY_DELETE( pMusicInfoList );
    TRY_DELETE( pEnum );

    // return info
    if ( !musicInfoList.IsEmpty() ) {
        return scope.Close( musicInfoList );
    }

    return scope.Close( v8::Undefined() );
}

v8::Handle<v8::Value> Musics::getAllMusicInfoToAlbum(const v8::Arguments& args) {
    AppLog("Entered Musics::getAllMusicInfoToAlbum");
    v8::HandleScope scope;

    return scope.Close( v8::Undefined() );
}

v8::Handle<v8::Value> Musics::getAllMusicInfoToArtist(const v8::Arguments& args) {
    AppLog("Entered Musics::getAllMusicInfoToArtist");
    v8::HandleScope scope;

    return scope.Close( v8::Undefined() );
}

v8::Handle<v8::Value> Musics::getAllMusicInfoToComposer(const v8::Arguments& args) {
    AppLog("Entered Musics::getAllMusicInfoToComposer");
    v8::HandleScope scope;

    return scope.Close( v8::Undefined() );
}

v8::Handle<v8::Value> Musics::getAllMusicInfoToGenre(const v8::Arguments& args) {
    AppLog("Entered Musics::getAllMusicInfoToGenre");
    v8::HandleScope scope;

    return scope.Close( v8::Undefined() );
}
