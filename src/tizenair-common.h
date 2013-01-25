#ifndef __COMMON__
#define __COMMON__

#define TRY_DELETE( x ) { if ( NULL != (x) ) delete (x);  }

#define WRAP_STRING( x ) ( (NULL==(x))?(v8::Undefined()):(v8::String::New( (x->c_str() ) ) ) )
#define UNWRAP_STRING( x ) ( ((x)->IsUndefined())?(NULL):(new std::string( *( v8::String::Utf8Value( x ) ) ) ) )


#endif
