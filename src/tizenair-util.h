#include <string>

#include <v8.h>
#include <node.h>

class Util : node::ObjectWrap
{
	public:
	static v8::Persistent<v8::FunctionTemplate> funcTemplate;
	static v8::Persistent<v8::FunctionTemplate> _startsWith;
	static v8::Persistent<v8::FunctionTemplate> _endsWith;
	static v8::Persistent<v8::FunctionTemplate> _addPath;
	static v8::Persistent<v8::FunctionTemplate> _getFilenameFrom;
	static v8::Persistent<v8::FunctionTemplate> _getParentFrom;

	// require에 의해 로드될 때 호출되는 메쏘드
	static void NODE_EXTERN Init( v8::Handle<v8::Object> target );

	static v8::Handle<v8::Value> New( const v8::Arguments& args );

	static v8::Handle<v8::Value> startsWith( const v8::Arguments& args );
	static v8::Handle<v8::Value> endsWith( const v8::Arguments& args );
	static v8::Handle<v8::Value> addPath( const v8::Arguments& args );
	static v8::Handle<v8::Value> getFilenameFrom( const v8::Arguments& args );
	static v8::Handle<v8::Value> getParentFrom( const v8::Arguments& args );

	protected:
	static std::string addPath( std::string path1, std::string path2 );
};

