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

    private:
        static Tizen::Base::Collection::IList* GetPlayListsN(void);
        static v8::Local<v8::Object> CreateMusicObject(void);
        static v8::Local<v8::Array> getMusicList(Tizen::Base::Collection::IEnumerator* pListEnum);
};

#endif
