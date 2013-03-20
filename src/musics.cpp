#include <v8.h>
#include <node.h>
#include <stdlib.h>
#include <iostream>
#include <FSystem.h>
#include <FIo.h>
#include <FBaseSysLog.h>
#include <FBase.h>


#include "contents.h"
#include "musics.h"
#include "tizenair-util.h"

using namespace Tizen::Base::Collection;
using namespace Tizen::Content;
using namespace Tizen::System;

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

    funcTemplate->Set(v8::String::NewSymbol("getAllIMusicIDAndPathInfo"), v8::FunctionTemplate::New(getAllIMusicIDAndPathInfo)->GetFunction());

    funcTemplate->Set(v8::String::NewSymbol("createMusic"), v8::FunctionTemplate::New(createMusic)->GetFunction());
    funcTemplate->Set(v8::String::NewSymbol("removeMusicForId"), v8::FunctionTemplate::New(removeMusicForId)->GetFunction());
    funcTemplate->Set(v8::String::NewSymbol("removeMusicForPath"), v8::FunctionTemplate::New(removeMusicForPath)->GetFunction());
    funcTemplate->Set(v8::String::NewSymbol("moveMusicForId"), v8::FunctionTemplate::New(moveMusicForId)->GetFunction());
    funcTemplate->Set(v8::String::NewSymbol("moveMusicForPath"), v8::FunctionTemplate::New(moveMusicForPath)->GetFunction());

    funcTemplate->Set(v8::String::NewSymbol("getAllPlayListInfo"), v8::FunctionTemplate::New(getAllPlayListInfo)->GetFunction());
    funcTemplate->Set(v8::String::NewSymbol("createPlayList"), v8::FunctionTemplate::New(createPlayList)->GetFunction());
    funcTemplate->Set(v8::String::NewSymbol("removePlayList"), v8::FunctionTemplate::New(removePlayList)->GetFunction());
    funcTemplate->Set(v8::String::NewSymbol("updatePlayListName"), v8::FunctionTemplate::New(updatePlayListName)->GetFunction());
    funcTemplate->Set(v8::String::NewSymbol("addPlayListItemForId"), v8::FunctionTemplate::New(addPlayListItemForId)->GetFunction());
    funcTemplate->Set(v8::String::NewSymbol("removePlayListItemForId"), v8::FunctionTemplate::New(removePlayListItemForId)->GetFunction());

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

    r = TizenContents::removeContent( CONTENT_TYPE_AUDIO, contentPath );
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

    result r = TizenContents::moveContent( srcPath, destPath );
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
    v8::Local<v8::Object> info = v8::Object::New();
    v8::Local<v8::Object> album = v8::Object::New();
    v8::Local<v8::Array> itemArray = v8::Array::New();
    v8::Local<v8::Array> albumArray = v8::Array::New();

    char *pResult = null;
    IList *pMusicInfoList = TizenContents::getAllContentsListForColumnN( CONTENT_TYPE_AUDIO, L"Album" ); // must free
    if ( pMusicInfoList == null ) {
        return scope.Close( v8::Undefined() );
    }

    String prevAlbumName = L"Unknown";
    bool isfirst = true;
    int albumCount = 0;
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
            String albumName = pAudioInfo->GetAlbumName();
            if ( isfirst ) {
                prevAlbumName = albumName;
                isfirst = false;
            }

            if ( prevAlbumName.Equals( albumName ) ) {
                v8::Local<v8::Object> item = v8::Object::New();

                // set item info
                String id = pAudioInfo->GetContentId().ToString();
                String path = pAudioInfo->GetContentPath();
                String artist = pAudioInfo->GetArtist();
                String composer = pAudioInfo->GetComposer();
                String copyrigth = pAudioInfo->GetCopyright();
                String genre = pAudioInfo->GetGenre();
                String title = pAudioInfo->GetTitle();
                String trackInfo = pAudioInfo->GetTrackInfo();
                String rating = pAudioInfo->GetRating();
                int bitrate = pAudioInfo->GetBitrate();
                int releaseYear = pAudioInfo->GetReleaseYear();

                // set Music title
                pResult = Util::toAnsi( title );
                item->Set( v8::String::New( "title" ), v8::String::New( pResult ) );
                delete pResult;

                // set Music id
                pResult = Util::toAnsi( id );
                item->Set( v8::String::New( "id" ), v8::String::New( pResult ) );
                delete pResult;

                // set Music path
                pResult = Util::toAnsi( path );
                item->Set( v8::String::New( "path" ), v8::String::New( pResult ) );
                delete pResult;

                // set Music artist
                pResult = Util::toAnsi( artist );
                item->Set( v8::String::New( "artist" ), v8::String::New( pResult ) );
                delete pResult;

                // set Music composer
                pResult = Util::toAnsi( composer );
                item->Set( v8::String::New( "composer" ), v8::String::New( pResult ) );
                delete pResult;

                // set Music copyright
                pResult = Util::toAnsi( copyrigth );
                item->Set( v8::String::New( "copyrigth" ), v8::String::New( pResult ) );
                delete pResult;

                // set Music genre
                pResult = Util::toAnsi( genre );
                item->Set( v8::String::New( "genre" ), v8::String::New( pResult ) );
                delete pResult;

                // set Music trackInfo
                pResult = Util::toAnsi( trackInfo );
                item->Set( v8::String::New( "track-info" ), v8::String::New( pResult ) );
                delete pResult;

                // set Music rating
                pResult = Util::toAnsi( rating );
                item->Set( v8::String::New( "rating" ), v8::String::New( pResult ) );
                delete pResult;

                // set Music bitrate
                pResult = Util::toAnsi( bitrate );
                item->Set( v8::String::New( "bitrate" ), v8::String::New( pResult ) );
                delete pResult;

                // set Music release year
                pResult = Util::toAnsi( releaseYear );
                item->Set( v8::String::New( "release-year" ), v8::String::New( pResult ) );
                delete pResult;

                itemArray->Set( count++, item );
            }

            if ( !prevAlbumName.Equals( albumName ) ) {
                pResult = Util::toAnsi( prevAlbumName );
                album->Set( v8::String::New( "album" ), v8::String::New( pResult ) );
                delete pResult;

                pResult = Util::toAnsi( count );
                album->Set( v8::String::New( "items-length" ), v8::String::New( pResult ) );
                delete pResult;

                album->Set( v8::String::New( "items" ), itemArray );
                itemArray = v8::Array::New();
                count = 0;
                albumArray->Set( albumCount++, album );
                album = v8::Object::New();
                prevAlbumName = albumName;

                // itself item
                v8::Local<v8::Object> item = v8::Object::New();

                // set item info
                String id = pAudioInfo->GetContentId().ToString();
                String path = pAudioInfo->GetContentPath();
                String artist = pAudioInfo->GetArtist();
                String composer = pAudioInfo->GetComposer();
                String copyrigth = pAudioInfo->GetCopyright();
                String genre = pAudioInfo->GetGenre();
                String title = pAudioInfo->GetTitle();
                String trackInfo = pAudioInfo->GetTrackInfo();
                String rating = pAudioInfo->GetRating();
                int bitrate = pAudioInfo->GetBitrate();
                int releaseYear = pAudioInfo->GetReleaseYear();

                // set Music title
                pResult = Util::toAnsi( title );
                item->Set( v8::String::New( "title" ), v8::String::New( pResult ) );
                delete pResult;

                // set Music id
                pResult = Util::toAnsi( id );
                item->Set( v8::String::New( "id" ), v8::String::New( pResult ) );
                delete pResult;

                // set Music path
                pResult = Util::toAnsi( path );
                item->Set( v8::String::New( "path" ), v8::String::New( pResult ) );
                delete pResult;

                // set Music artist
                pResult = Util::toAnsi( artist );
                item->Set( v8::String::New( "artist" ), v8::String::New( pResult ) );
                delete pResult;

                // set Music composer
                pResult = Util::toAnsi( composer );
                item->Set( v8::String::New( "composer" ), v8::String::New( pResult ) );
                delete pResult;

                // set Music copyright
                pResult = Util::toAnsi( copyrigth );
                item->Set( v8::String::New( "copyrigth" ), v8::String::New( pResult ) );
                delete pResult;

                // set Music genre
                pResult = Util::toAnsi( genre );
                item->Set( v8::String::New( "genre" ), v8::String::New( pResult ) );
                delete pResult;

                // set Music trackInfo
                pResult = Util::toAnsi( trackInfo );
                item->Set( v8::String::New( "track-info" ), v8::String::New( pResult ) );
                delete pResult;

                // set Music rating
                pResult = Util::toAnsi( rating );
                item->Set( v8::String::New( "rating" ), v8::String::New( pResult ) );
                delete pResult;

                // set Music bitrate
                pResult = Util::toAnsi( bitrate );
                item->Set( v8::String::New( "bitrate" ), v8::String::New( pResult ) );
                delete pResult;

                // set Music release year
                pResult = Util::toAnsi( releaseYear );
                item->Set( v8::String::New( "release-year" ), v8::String::New( pResult ) );
                delete pResult;

                itemArray->Set( count++, item );
            }
        }
    }

    // set last album
    pResult = Util::toAnsi( prevAlbumName );
    album->Set( v8::String::New( "album" ), v8::String::New( pResult ) );
    delete pResult;

    pResult = Util::toAnsi( count );
    album->Set( v8::String::New( "items-length" ), v8::String::New( pResult ) );
    delete pResult;

    album->Set( v8::String::New( "items" ), itemArray );
    itemArray = v8::Array::New();
    albumArray->Set( albumCount++, album );

    // set info
    pResult = Util::toAnsi( albumCount );
    info->Set( v8::String::New( "albumInfo-length" ), v8::String::New( pResult ) );
    delete pResult;

    info->Set( v8::String::New( "albumInfo" ), albumArray );


    // free
    TRY_DELETE( pMusicInfoList );
    TRY_DELETE( pEnum );

    // return info
    if ( !info.IsEmpty() ) {
        return scope.Close( info );
    }

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

v8::Handle<v8::Value> Musics::getAllIMusicIDAndPathInfo(const v8::Arguments& args) {
    AppLog("Entered Musics::getAllIMusicIDAndPathInfo");
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

            // set Music id
            pResult = Util::toAnsi( id );
            musicInfo->Set( v8::String::New( "id" ), v8::String::New( pResult ) );
            delete pResult;

            // set Music path
            pResult = Util::toAnsi( path );
            musicInfo->Set( v8::String::New( "path" ), v8::String::New( pResult ) );
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

v8::Handle<v8::Value> Musics::createMusic(const v8::Arguments& args) {
    AppLog("Entered Musics::createMusic");
    v8::HandleScope scope;
    v8::Local<v8::Object> infoSet = v8::Object::New();

    result r;
    String *psourcePath = null; // first arg - music source full address
    String *pdestPath = null; // second arg - music destination full address
    bool delSource = false;  // third arg - music source delete option

    if ( args.Length() < 1 || args[0]->IsUndefined() || !args[0]->IsString() ) {
        infoSet->Set( v8::String::New( "result" ), v8::False() );
        infoSet->Set( v8::String::New( "desc" ), v8::String::New( "wrong argument" ) );
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
            pdestPath = new String( Environment::GetMediaPath() + L"Sounds" + *pName );

            // free
            TRY_DELETE( pResult );
            TRY_DELETE( pName );
        }

        // third argument
        if ( !args[2]->IsUndefined() && args[2]->IsBoolean() ) {
            delSource = args[2]->BooleanValue();
        }

        // create music operation
        ContentId id;
        r = TizenContents::createContent( *psourcePath, *pdestPath, delSource, id);
        if ( IsFailed( r ) ) {
            infoSet->Set( v8::String::New( "result" ), v8::False() );
            infoSet->Set( v8::String::New( "desc" ), v8::String::New( GetErrorMessage( r ) ) );
        } else {
            pResult = Util::toAnsi( id.ToString() );
            infoSet->Set( v8::String::New( "id" ),  v8::String::New( pResult ) );
            delete pResult;

            infoSet->Set( v8::String::New( "result" ), v8::True() );
        }
    }

CATCH:
    // free
    TRY_DELETE( psourcePath );
    TRY_DELETE( pdestPath );

    // info return
    if ( !infoSet.IsEmpty() ) {
        return scope.Close( infoSet );
    }

    return scope.Close( v8::Undefined() );
}

v8::Handle<v8::Value> Musics::removeMusicForId(const v8::Arguments& args) {
    AppLog("Entered Musics::removeMusicForId");

    v8::HandleScope scope;
    v8::Local<v8::Object> infoSet = v8::Object::New();

    result r;
    String *pstr = null;

    if ( args.Length() < 1 || args[0]->IsUndefined() || !args[0]->IsString() ) {
        infoSet->Set( v8::String::New( "result" ), v8::False() );
        infoSet->Set( v8::String::New( "desc" ), v8::String::New( "wrong id value" ) );

        goto CATCH;
    } else {
        pstr = Util::toTizenStringN( args[0]->ToString() );  // must free
        pstr->ToUpper();

        ContentId id;
        UuId::Parse( *pstr, id );

        // Delete
        r = TizenContents::removeContent( id );
        if ( IsFailed(r) )
        {
            infoSet->Set( v8::String::New( "result" ), v8::False() );
            infoSet->Set( v8::String::New( "desc" ), v8::String::New( GetErrorMessage( r ) ) );
        } else {
            infoSet->Set( v8::String::New( "result" ), v8::True() );
        }
    }

CATCH:
    // free
    TRY_DELETE( pstr );

    // info return
    if ( !infoSet.IsEmpty() ) {
        return scope.Close( infoSet );
    }

    return scope.Close( v8::Undefined() );
}

v8::Handle<v8::Value> Musics::removeMusicForPath(const v8::Arguments& args) {
    AppLog("Entered Musics::removeMusicForPath");

    v8::HandleScope scope;
    v8::Local<v8::Object> infoSet = v8::Object::New();

    result r;
    String *pstr = null;

    if ( args.Length() < 1 || args[0]->IsUndefined() || !args[0]->IsString() ) {
        infoSet->Set( v8::String::New( "result" ), v8::False() );
        infoSet->Set( v8::String::New( "desc" ), v8::String::New( "wrong path value" ) );

        goto CATCH;
    } else {
        pstr = Util::toTizenStringN( args[0]->ToString() );  // must free

        // Delete
        r = TizenContents::removeContent( CONTENT_TYPE_AUDIO, *pstr );
        if ( IsFailed(r) )
        {
            infoSet->Set( v8::String::New( "result" ), v8::False() );
            infoSet->Set( v8::String::New( "desc" ), v8::String::New( GetErrorMessage( r ) ) );
        } else {
            infoSet->Set( v8::String::New( "result" ), v8::True() );
        }
    }

CATCH:
    // free
    TRY_DELETE( pstr );

    // info return
    if ( !infoSet.IsEmpty() ) {
        return scope.Close( infoSet );
    }

    return scope.Close( v8::Undefined() );
}

v8::Handle<v8::Value> Musics::moveMusicForId(const v8::Arguments& args) {
    AppLog("Entered Musics::moveMusicForId");

    v8::HandleScope scope;
    v8::Local<v8::Object> infoSet = v8::Object::New();

    result r;
    String *pstr = null;

    // check argument
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

        // src Path
        String srcPath;
        r = TizenContents::getContentPath( id, srcPath );
        if ( IsFailed( r ) ) {
            infoSet->Set( v8::String::New( "result" ), v8::False() );
            infoSet->Set( v8::String::New( "desc" ), v8::String::New( "failed to get path from given id" ) );
            goto CATCH;
        }

        // get music file name and New path setting
        pResult = Util::toAnsi( srcPath ); // must free
        String *pName = new String( strrchr( pResult, '/' ) ); // must free
        String *pInputPath = Util::toTizenStringN( args[1]->ToString() ); // must free
        String destPath = *pInputPath + *pName;
        TRY_DELETE( pInputPath );
        TRY_DELETE( pName );
        TRY_DELETE( pResult );

        AppLog ( "srcPath : %s" , Util::toAnsi( srcPath ) );
        AppLog ( "desPath : %s" , Util::toAnsi( destPath ) );

        // move music content
        ContentId newId;
        r = TizenContents::moveContent( srcPath, destPath, newId );
        if ( IsFailed( r ) ) {
            // failed
            infoSet->Set( v8::String::New( "result" ), v8::False() );
            infoSet->Set( v8::String::New( "desc" ), v8::String::New( "failed to move music content" ) );
            goto CATCH;
        } else {
            // success
            infoSet->Set( v8::String::New( "result" ), v8::True() );

            pResult = Util::toAnsi( newId.ToString() );
            infoSet->Set( v8::String::New( "new-id" ),  v8::String::New( pResult ) );
            TRY_DELETE( pResult );
        }
    }


CATCH:
    // free
    TRY_DELETE( pstr );

    // info return
    if ( !infoSet.IsEmpty() ) {
        return scope.Close( infoSet );
    }

    return scope.Close( v8::Undefined() );
}

v8::Handle<v8::Value> Musics::moveMusicForPath(const v8::Arguments& args) {
    AppLog("Entered Musics::moveMusicForPath");
    v8::HandleScope scope;
    v8::Local<v8::Object> infoSet = v8::Object::New();

    result r;
    String *pstr = null;

    // check argument
    if ( args.Length() < 2 || args[0]->IsUndefined() || !args[0]->IsString() ||
            args[1]->IsUndefined() || !args[1]->IsString()) {
        infoSet->Set( v8::String::New( "result" ), v8::False() );
        infoSet->Set( v8::String::New( "desc" ), v8::String::New( "wrong input value" ) );
        goto CATCH;
    } else {
        char *pResult = null;
        // src Path
        pstr = Util::toTizenStringN( args[0]->ToString() ); // must free
        String srcPath = *pstr;

        // get music file name and New path setting
        pResult = Util::toAnsi( srcPath ); // must free
        String *pName = new String( strrchr( pResult, '/' ) ); // must free
        String *pInputPath = Util::toTizenStringN( args[1]->ToString() ); // must free
        String destPath = *pInputPath + *pName;
        TRY_DELETE( pInputPath );
        TRY_DELETE( pName );
        TRY_DELETE( pResult );

        AppLog ( "srcPath : %s" , Util::toAnsi( srcPath ) );
        AppLog ( "desPath : %s" , Util::toAnsi( destPath ) );

        // move music content
        ContentId newId;
        r = TizenContents::moveContent( srcPath, destPath, newId );
        if ( IsFailed( r ) ) {
            // failed
            infoSet->Set( v8::String::New( "result" ), v8::False() );
            infoSet->Set( v8::String::New( "desc" ), v8::String::New( "failed to move music content" ) );
            goto CATCH;
        } else {
            // success
            infoSet->Set( v8::String::New( "result" ), v8::True() );

            pResult = Util::toAnsi( newId.ToString() );
            infoSet->Set( v8::String::New( "new-id" ),  v8::String::New( pResult ) );
            TRY_DELETE( pResult );
        }
    }


CATCH:
    // free
    TRY_DELETE( pstr );

    // info return
    if ( !infoSet.IsEmpty() ) {
        return scope.Close( infoSet );
    }

    return scope.Close( v8::Undefined() );
}


v8::Handle<v8::Value> Musics::getAllPlayListInfo(const v8::Arguments& args) {
    AppLog("Entered Musics::getAllPlayListInfo");
    v8::HandleScope scope;

    v8::Local<v8::Object> infoSet = v8::Object::New();
    v8::Local<v8::Array> playListArray = v8::Array::New();
    int cnt = 0;
    char *pResult = null;

    PlayListManager *pManager = PlayListManager::GetInstance();
    if ( IsFailed( GetLastResult() ) ) {
        AppLog("Failed to get playlist instance: %s", GetErrorMessage( GetLastResult() ) );
        return scope.Close( v8::Undefined() );
    }

    IList *pListName = pManager->GetAllPlayListNameN(); // must free
    if ( IsFailed( GetLastResult() ) ) {
        AppLog("Failed to get playlist: %s", GetErrorMessage( GetLastResult() ) );
        return scope.Close( v8::Undefined() );
    }

    // set playList count
    int playListCount = pManager->GetAllPlayListCount();
    pResult = Util::toAnsi( playListCount );
    infoSet->Set( v8::String::New("playListInfo-length"), v8::String::New( pResult ) );
    TRY_DELETE( pResult );

    // set each playList info
    IEnumerator* pListEnum = pListName->GetEnumeratorN(); // must free
    while( pListEnum->MoveNext() == E_SUCCESS ) {
        v8::Local<v8::Object> playList = v8::Object::New();
        v8::Local<v8::Array> playListItemArray = v8::Array::New();

        // set play list name
        String *playListName = static_cast<String*>(pListEnum->GetCurrent());
        pResult = Util::toAnsi( *playListName );
        playList->Set( v8::String::New("playListName"), v8::String::New( pResult ) );
        TRY_DELETE( pResult );

        // set play list item length
        PlayList *pPlayList = pManager->GetPlayListN( *playListName ); // must free
        int itemCnt = pPlayList->GetPlayListItemCount();
        pResult = Util::toAnsi( itemCnt );
        playList->Set( v8::String::New("playListItem-length"), v8::String::New( pResult ) );
        TRY_DELETE( pResult );

        // set play list item info
        IList *pItemList = pPlayList->GetContentInfoListN(); // must free
        IEnumerator *pItemEnum = pItemList->GetEnumeratorN(); // must free
        int incnt = 0;
        while ( pItemEnum->MoveNext() == E_SUCCESS ) {
             ContentInfo *pContentInfo = static_cast<ContentInfo*>( pItemEnum->GetCurrent() );
             AudioContentInfo *pAudioInfo = null;
             if ( pContentInfo->GetContentType() == CONTENT_TYPE_AUDIO ) {
                 pAudioInfo = static_cast<AudioContentInfo*>(pContentInfo);
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

                 // set
                 playListItemArray->Set( incnt++, musicInfo );
             }
        }
        playList->Set( v8::String::New("playListItemInfo"), playListItemArray );

        // set all info
        playListArray->Set( cnt++, playList );

        // free
        TRY_DELETE( pItemEnum );
        TRY_DELETE( pItemList );
        TRY_DELETE( pPlayList );
    }

    // set info
    infoSet->Set( v8::String::New("playListInfo-List"), playListArray );

    // free
    TRY_DELETE( pListEnum );
    if ( pListName != null ) {
        pListName->RemoveAll( true );
        TRY_DELETE( pListName );
    }

    // return info
    if ( !infoSet.IsEmpty() ) {
        return scope.Close( infoSet );
    }

    return scope.Close( v8::Undefined() );
}

v8::Handle<v8::Value> Musics::createPlayList(const v8::Arguments& args) {
    AppLog("Entered Musics::createPlayList");
    v8::HandleScope scope;
    v8::Local<v8::Object> infoSet = v8::Object::New();
    char *pResult = null;
    String *pstr = null;

    if ( args.Length() < 1 || args[0]->IsUndefined() || !args[0]->IsString()) {
        infoSet->Set( v8::String::New( "palyListName" ), v8::Undefined() );
        infoSet->Set( v8::String::New( "result" ), v8::False() );
        infoSet->Set( v8::String::New( "desc" ), v8::String::New( "wrong argument" ) );

        return scope.Close( infoSet );
    } else {
        // first arguments is new play list name
        pstr = Util::toTizenStringN( args[0]->ToString() );  // must free

        PlayList playList;
        result r = playList.Construct( *pstr );
        if ( IsFailed( r ) ) {
            AppLog("Failed to make play list: %s", GetErrorMessage( r ) );

            pResult = Util::toAnsi( *pstr );
            infoSet->Set( v8::String::New( "palyListName" ), v8::String::New( pResult ) );
            TRY_DELETE( pResult );

            infoSet->Set( v8::String::New( "result" ), v8::False() );
            infoSet->Set( v8::String::New( "desc" ), v8::String::New( GetErrorMessage( r ) ) );

            // free
            TRY_DELETE( pstr );

            return scope.Close( infoSet );
        } else {
            AppLog("Success to make play list: %s", GetErrorMessage( r ) );

            pResult = Util::toAnsi( *pstr );
            infoSet->Set( v8::String::New( "palyListName" ), v8::String::New( pResult ) );
            TRY_DELETE( pResult );
            infoSet->Set( v8::String::New( "result" ), v8::True() );

            // free
            TRY_DELETE( pstr );

            return scope.Close( infoSet );
        }
    }

    // free
    TRY_DELETE( pstr );

    return scope.Close( v8::Undefined() );
}

v8::Handle<v8::Value> Musics::removePlayList(const v8::Arguments& args) {
    AppLog("Entered Musics::removePlayList");
    v8::HandleScope scope;
    v8::Local<v8::Object> infoSet = v8::Object::New();
    char *pResult = null;
    String *pstr = null;

    if ( args.Length() < 1 || args[0]->IsUndefined() || !args[0]->IsString()) {
        infoSet->Set( v8::String::New( "palyListName" ), v8::Undefined() );
        infoSet->Set( v8::String::New( "result" ), v8::False() );
        infoSet->Set( v8::String::New( "desc" ), v8::String::New( "wrong argument" ) );

        return scope.Close( infoSet );
    } else {
        // first arguments is remove play list name
        pstr = Util::toTizenStringN( args[0]->ToString() );  // must free

        PlayListManager* pManager = PlayListManager::GetInstance();
        if ( IsFailed( GetLastResult() ) ) {
            pResult = Util::toAnsi( *pstr );
            infoSet->Set( v8::String::New( "palyListName" ), v8::String::New( pResult ) );
            TRY_DELETE( pResult );
            infoSet->Set( v8::String::New( "result" ), v8::False() );
            infoSet->Set( v8::String::New( "desc" ), v8::String::New( GetErrorMessage( GetLastResult() ) ) );

            // free
            TRY_DELETE( pstr );

            return scope.Close( infoSet );
        }

        result r = pManager->RemovePlayList( *pstr );
        if ( IsFailed( r ) ) {
            AppLog("Failed to remove play list: %s", GetErrorMessage( r ) );

            pResult = Util::toAnsi( *pstr );
            infoSet->Set( v8::String::New( "palyListName" ), v8::String::New( pResult ) );
            TRY_DELETE( pResult );

            infoSet->Set( v8::String::New( "result" ), v8::False() );
            infoSet->Set( v8::String::New( "desc" ), v8::String::New( GetErrorMessage( r ) ) );

            // free
            TRY_DELETE( pstr );

            return scope.Close( infoSet );
        } else {
            AppLog("Success to remove play list: %s", GetErrorMessage( r ) );

            pResult = Util::toAnsi( *pstr );
            infoSet->Set( v8::String::New( "palyListName" ), v8::String::New( pResult ) );
            TRY_DELETE( pResult );
            infoSet->Set( v8::String::New( "result" ), v8::True() );

            // free
            TRY_DELETE( pstr );

            return scope.Close( infoSet );
        }
    }

    // free
    TRY_DELETE( pstr );

    return scope.Close( v8::Undefined() );
}

v8::Handle<v8::Value> Musics::updatePlayListName(const v8::Arguments& args) {
    AppLog("Entered Musics::updatePlayListName");
    v8::HandleScope scope;
    v8::Local<v8::Object> infoSet = v8::Object::New();
    char *pResult = null;
    String *pstr1 = null;
    String *pstr2 = null;

    if ( args.Length() < 1 ) {
        infoSet->Set( v8::String::New( "old-palyListName" ), v8::Undefined() );
        infoSet->Set( v8::String::New( "new-palyListName" ), v8::Undefined() );
        infoSet->Set( v8::String::New( "result" ), v8::False() );
        infoSet->Set( v8::String::New( "desc" ), v8::String::New( "wrong argument" ) );

        return scope.Close( infoSet );
    } else if ( args.Length() == 1 ) {
        if ( args[0]->IsUndefined() || !args[0]->IsString() ) {
            infoSet->Set( v8::String::New( "old-palyListName" ), v8::Undefined() );
        } else {
            pstr1 = Util::toTizenStringN( args[0]->ToString() );
            pResult = Util::toAnsi( *pstr1 );
            infoSet->Set( v8::String::New( "old-palyListName" ), v8::String::New( pResult ) );
            TRY_DELETE( pResult );
        }
        infoSet->Set( v8::String::New( "new-palyListName" ), v8::Undefined() );
        infoSet->Set( v8::String::New( "result" ), v8::False() );
        infoSet->Set( v8::String::New( "desc" ), v8::String::New( "wrong argument" ) );

        // free
        TRY_DELETE( pstr1 );
        TRY_DELETE( pstr2 );

        return scope.Close( infoSet );
    } else if ( args.Length() > 1 ) {
        if ( args[0]->IsUndefined() || !args[0]->IsString() ) {
            infoSet->Set( v8::String::New( "old-palyListName" ), v8::Undefined() );
        } else {
            pstr1 = Util::toTizenStringN( args[0]->ToString() );
            pResult = Util::toAnsi( *pstr1 );
            infoSet->Set( v8::String::New( "old-palyListName" ), v8::String::New( pResult ) );
            TRY_DELETE( pResult );
        }

        if ( args[1]->IsUndefined() || !args[1]->IsString() ) {
            infoSet->Set( v8::String::New( "new-palyListName" ), v8::Undefined() );
        } else {
            pstr2 = Util::toTizenStringN( args[1]->ToString() );
            pResult = Util::toAnsi( *pstr2 );
            infoSet->Set( v8::String::New( "new-palyListName" ), v8::String::New( pResult ) );
            TRY_DELETE( pResult );
        }

        if ( pstr1 == null || pstr2 == null ) { // wrong args
            infoSet->Set( v8::String::New( "result" ), v8::False() );
            infoSet->Set( v8::String::New( "desc" ), v8::String::New( "wrong argument" ) );

            // free
            TRY_DELETE( pstr1 );
            TRY_DELETE( pstr2 );

            return scope.Close( infoSet );
        } else { // normal args
            PlayList* pPlayList = PlayListManager::GetInstance()->GetPlayListN( *pstr1 );
            if ( IsFailed( GetLastResult() ) ) {
                pResult = Util::toAnsi( *pstr1 );
                infoSet->Set( v8::String::New( "old-palyListName" ), v8::String::New( pResult ) );
                TRY_DELETE( pResult );
                pResult = Util::toAnsi( *pstr2 );
                infoSet->Set( v8::String::New( "new-palyListName" ), v8::String::New( pResult ) );
                TRY_DELETE( pResult );
                infoSet->Set( v8::String::New( "result" ), v8::False() );
                infoSet->Set( v8::String::New( "desc" ), v8::String::New( GetErrorMessage( GetLastResult() ) ) );

                // free
                TRY_DELETE( pstr1 );
                TRY_DELETE( pstr2 );
                TRY_DELETE( pPlayList );

                return scope.Close( infoSet );
            }

            result r = pPlayList->SetPlayListName( *pstr2 );
            if ( IsFailed( r ) ) {
                pResult = Util::toAnsi( *pstr1 );
                infoSet->Set( v8::String::New( "old-palyListName" ), v8::String::New( pResult ) );
                TRY_DELETE( pResult );
                pResult = Util::toAnsi( *pstr2 );
                infoSet->Set( v8::String::New( "new-palyListName" ), v8::String::New( pResult ) );
                TRY_DELETE( pResult );
                infoSet->Set( v8::String::New( "result" ), v8::False() );
                infoSet->Set( v8::String::New( "desc" ), v8::String::New( GetErrorMessage( r ) ) );

                // free
                TRY_DELETE( pstr1 );
                TRY_DELETE( pstr2 );
                TRY_DELETE( pPlayList );

                return scope.Close( infoSet );
            } else {
                pResult = Util::toAnsi( *pstr1 );
                infoSet->Set( v8::String::New( "old-palyListName" ), v8::String::New( pResult ) );
                TRY_DELETE( pResult );
                pResult = Util::toAnsi( *pstr2 );
                infoSet->Set( v8::String::New( "new-palyListName" ), v8::String::New( pResult ) );
                TRY_DELETE( pResult );
                infoSet->Set( v8::String::New( "result" ), v8::True() );

                // free
                TRY_DELETE( pstr1 );
                TRY_DELETE( pstr2 );
                TRY_DELETE( pPlayList );

                return scope.Close( infoSet );
            }
        }
    }

    return scope.Close( v8::Undefined() );
}


v8::Handle<v8::Value> Musics::addPlayListItemForId(const v8::Arguments& args) {
    AppLog("Entered Musics::addPlayListItemForId");
    v8::HandleScope scope;

    v8::Local<v8::Object> infoSet = v8::Object::New();
    char *pResult = null;
    String *pListName = null; // first argument - playList name
    String *pItemId = null; // second argument - music item id

    if ( args.Length() < 2 ||
         args[0]->IsUndefined() || !args[0]->IsString() ||
         args[1]->IsUndefined() || !args[1]->IsString() ) {
        infoSet->Set( v8::String::New( "result" ), v8::False() );
        infoSet->Set( v8::String::New( "desc" ), v8::String::New( "wrong argument" ) );

        return scope.Close( infoSet );
    }

    // get argument
    pListName = Util::toTizenStringN( args[0]->ToString() );
    pResult = Util::toAnsi( *pListName );
    TRY_DELETE( pResult );

    pItemId = Util::toTizenStringN( args[1]->ToString() );
    pResult = Util::toAnsi( *pItemId );
    TRY_DELETE( pResult );

    ContentId id;
    UuId::Parse( *pItemId, id );

    // add item in play list
    PlayList* pPlayList = PlayListManager::GetInstance()->GetPlayListN( *pListName ); // must free
    result r = pPlayList->AddItem( id );
    if ( IsFailed( r ) ) {
        AppLog("Failed to add play list: %s", GetErrorMessage( r ) );
        infoSet->Set( v8::String::New( "result" ), v8::False() );
        infoSet->Set( v8::String::New( "desc" ), v8::String::New( GetErrorMessage( r ) ) );
    } else {
        infoSet->Set( v8::String::New( "result" ), v8::True() );
    }

    // free
    TRY_DELETE( pPlayList );

    // return info
    if ( !infoSet.IsEmpty() ) {
        return scope.Close( infoSet );
    }

    return scope.Close( v8::Undefined() );
}

v8::Handle<v8::Value> Musics::removePlayListItemForId(const v8::Arguments& args) {
    AppLog("Entered Musics::removePlayListItemForId");
    v8::HandleScope scope;

    v8::Local<v8::Object> infoSet = v8::Object::New();
    char *pResult = null;
    String *pListName = null; // first argument - playList name
    String *pItemId = null; // second argument - music item id

    // check argument
    if ( args.Length() < 2 ||
         args[0]->IsUndefined() || !args[0]->IsString() ||
         args[1]->IsUndefined() || !args[1]->IsString() ) {
        infoSet->Set( v8::String::New( "result" ), v8::False() );
        infoSet->Set( v8::String::New( "desc" ), v8::String::New( "wrong argument" ) );

        return scope.Close( infoSet );
    }

    // get argument
    pListName = Util::toTizenStringN( args[0]->ToString() );
    pResult = Util::toAnsi( *pListName );
    TRY_DELETE( pResult );

    pItemId = Util::toTizenStringN( args[1]->ToString() );
    pResult = Util::toAnsi( *pItemId );
    TRY_DELETE( pResult );

    ContentId id;
    UuId::Parse( *pItemId, id );

    // remove item in play list
    PlayList* pPlayList = PlayListManager::GetInstance()->GetPlayListN( *pListName ); // must free
    result r = pPlayList->RemoveItem( id );
    if ( IsFailed( r ) ) {
        AppLog("Failed to remove play list: %s", GetErrorMessage( r ) );
        infoSet->Set( v8::String::New( "result" ), v8::False() );
        infoSet->Set( v8::String::New( "desc" ), v8::String::New( GetErrorMessage( r ) ) );
    } else {
        infoSet->Set( v8::String::New( "result" ), v8::True() );
    }

    // free
    TRY_DELETE( pPlayList );

    // return info
    if ( !infoSet.IsEmpty() ) {
        return scope.Close( infoSet );
    }

    return scope.Close( v8::Undefined() );
}


