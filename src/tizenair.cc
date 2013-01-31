#include <string>
#include <stdio.h>
#include <v8.h>
#include <node.h>

#include "tizenair-util.h"
#include "tizenair-fileattr.h"
#include "tizenair-fs.h"
#include "tizenair-file.h"

extern "C" {
	void NODE_EXTERN init( v8::Handle<v8::Object> target )
	{
		Util::Init( target );
		FileAttribute::Init( target );
		FS::Init( target );
	}
	NODE_MODULE( tizen, init );
}


