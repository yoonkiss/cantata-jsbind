#ifndef __TIZENAIR_COMMON__
#define __TIZENAIR_COMMON__

#define TRY_DELETE( x ) { if ( NULL != (x) ) delete (x);  }

// std::string을 v8::String으로 변환
#define WRAP_STRING( x ) ( ( std::string() ==(x))?(v8::Undefined()):(v8::String::New( ( (x).c_str() ) ) ) )

// v8::String을 std::string으로 변환
#define UNWRAP_STRING( x ) ( ((x)->IsUndefined())?(std::string()):( std::string( *( v8::String::Utf8Value( x ) ) ) ) )

#endif
