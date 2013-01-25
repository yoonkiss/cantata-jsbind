var tizen = require( './tizen-native.node' );

exports.test_getPath = function( test ) {
	var attr = new tizen.FileAttribute( { path: '/test/path' } );
	test.equals( '/test/path', attr.getPath() );
	test.done();
};

exports.test_exists = function( test ) {
	test.ok( !new tizen.FileAttribute( { path: '/test/path' } ).exists() );
	test.ok( new tizen.FileAttribute( { path: '/test/path', type: 'd' } ).exists() );
	test.ok( new tizen.FileAttribute( { path: '/test/path', type: 'f' } ).exists() );
	test.done();
};


exports.test_isDirectory = function( test ) {
	test.ok( !new tizen.FileAttribute( { path: '/test/path' } ).isDirectory() );
	test.ok( new tizen.FileAttribute( { path: '/test/path', type: 'd' } ).isDirectory() );
	test.ok( !new tizen.FileAttribute( { path: '/test/path', type: 'f' } ).isDirectory() );
	test.done();
}

exports.test_isFile = function( test ) {
	test.ok( !new tizen.FileAttribute( { path: '/test/path' } ).isFile() );
	test.ok( !new tizen.FileAttribute( { path: '/test/path', type: 'd' } ).isFile() );
	test.ok( new tizen.FileAttribute( { path: '/test/path', type: 'f' } ).isFile() );
	test.done();
}

exports.test_isFile = function( test ) {
	test.ok( !new tizen.FileAttribute( { path: '/test/path' } ).isFile() );
	test.ok( !new tizen.FileAttribute( { path: '/test/path', type: 'd' } ).isFile() );
	test.ok( new tizen.FileAttribute( { path: '/test/path', type: 'f' } ).isFile() );
	test.done();
}

exports.test_size = function( test ) {
	test.equals( 100, new tizen.FileAttribute( { size: 100 } ).size );
	test.equals( 0, new tizen.FileAttribute().size );
	new tizen.FileAttribute().size = 200;
	test.done();
}

