#ifndef __MUSICS__
#define __MUSICS__

#include <v8.h>
#include <FBase.h>
#include <FContent.h>

#include "tizenair-common.h"

using namespace Tizen::Base;
using namespace Tizen::Content;

class Musics : node::ObjectWrap
{
    public:
        static void NODE_EXTERN Init( v8::Handle<v8::Object> target );
        static v8::Handle<v8::Value> New(const v8::Arguments& args);
        static v8::Handle<v8::Value> getPlaylistNames(const v8::Arguments& args);
        static v8::Handle<v8::Value> getPlaylist(const v8::Arguments& args);
        static v8::Handle<v8::Value> post(const v8::Arguments& args);
        static v8::Handle<v8::Value> remove(const v8::Arguments& args);
        static v8::Handle<v8::Value> moveTo(const v8::Arguments& args);

        static v8::Handle<v8::Value> getAllMusicInfo(const v8::Arguments& args);
        static v8::Handle<v8::Value> getAllMusicInfoToAlbum(const v8::Arguments& args);
        static v8::Handle<v8::Value> getAllMusicInfoToArtist(const v8::Arguments& args);
        static v8::Handle<v8::Value> getAllMusicInfoToComposer(const v8::Arguments& args);
        static v8::Handle<v8::Value> getAllMusicInfoToGenre(const v8::Arguments& args);

        static v8::Handle<v8::Value> getAllIMusicIDAndPathInfo(const v8::Arguments& args);

        static v8::Handle<v8::Value> createMusic(const v8::Arguments& args);
        static v8::Handle<v8::Value> removeMusicForId(const v8::Arguments& args);
        static v8::Handle<v8::Value> removeMusicForPath(const v8::Arguments& args);
        static v8::Handle<v8::Value> moveMusicForId(const v8::Arguments& args);
        static v8::Handle<v8::Value> moveMusicForPath(const v8::Arguments& args);

        // play list handle
        static v8::Handle<v8::Value> getAllPlayListInfo(const v8::Arguments& args);
        static v8::Handle<v8::Value> createPlayList(const v8::Arguments& args);
        static v8::Handle<v8::Value> removePlayList(const v8::Arguments& args);
        static v8::Handle<v8::Value> updatePlayListName(const v8::Arguments& args);
        static v8::Handle<v8::Value> addPlayListItemForId(const v8::Arguments& args);
        static v8::Handle<v8::Value> removePlayListItemForId(const v8::Arguments& args);

    private:
        static Tizen::Base::Collection::IList* GetPlayListsN(void);
        static v8::Local<v8::Object> CreateMusicObject(void);
        static v8::Local<v8::Array> getMusicList(Tizen::Base::Collection::IEnumerator* pListEnum);
};

#endif
