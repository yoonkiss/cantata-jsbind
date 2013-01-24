#include <string>
#include <stdio.h>
#include <v8.h>
#include <node.h>

#include "tizenair-util.h"

extern "C" {
	void NODE_EXTERN init( v8::Handle<v8::Object> target )
	{
		Util::Init( target );
	}
	NODE_MODULE( tizen, init );
}


