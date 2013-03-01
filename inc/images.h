#ifndef __IMAGES__
#define __IMAGES__

#include <v8.h>
#include <FBase.h>
#include <FContent.h>

#include "tizenair-common.h"

using namespace Tizen::Base;
using namespace Tizen::Content;

class Images : node::ObjectWrap
{
    public:
        static void NODE_EXTERN Init( v8::Handle<v8::Object> target );
        static v8::Handle<v8::Value> New(const v8::Arguments& args);
        static v8::Handle<v8::Value> list(const v8::Arguments& args);
        static v8::Handle<v8::Value> viewMetaInfo(const v8::Arguments& args);
        static v8::Handle<v8::Value> remove(const v8::Arguments& args);
        static v8::Handle<v8::Value> rename(const v8::Arguments& args);
        static v8::Handle<v8::Value> moveTo(const v8::Arguments& args);


        static v8::Handle<v8::Value> createAlbum(const v8::Arguments& args);
        static v8::Handle<v8::Value> listAlbum(const v8::Arguments& args);

    private:
        /**
         * Returns the contentInfo from the given abs path.
         *
         * @return      result
         * @param[in]   path          full path
         * @param[out]  contentInfo      contentInfo
         */
        static ContentInfo* getContentInfo(String path);

};

#endif
