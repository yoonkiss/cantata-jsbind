var fs = require( 'fs' );
var tizen = require( './tizen-native.node' );
exports.test_getPath = function( test ) {
	var fs = new tizen.FS( '.' );
	test.equal( './', fs.getPath( '/' ) );
	test.done();
};

exports.test_getAttribute = function( test ) {
	var fs = new tizen.FS( '.' );
	var attr = fs.getAttribute( '/fs.js' );
	test.ok( attr.exists(), 'fs.js does NOT exists' );
	test.ok( !attr.isDirectory(), 'fs.js is directory' );
	test.ok( attr.isFile(), 'fs.js is NOT file' );
	test.equal( './fs.js', attr.getPath(), 'path of fs.js is NOT ./fs.js' );
	test.equal( 'fs.js', attr.getName(), 'name of fs.js is NOT fs.js' );


	var attr = fs.getAttribute( 'fs.js' );
	test.ok( attr.exists(), './fs.js does NOT exists' );
	test.ok( !attr.isDirectory(), './fs.js is directory' );
	test.ok( attr.isFile(), './fs.js is NOT file' );
	test.equal( './fs.js', attr.getPath(), 'path of ./fs.js is NOT ./fs.js' );
	test.equal( 'fs.js', attr.getName(), 'name of ./fs.js is NOT fs.js' );

	test.done();
};
