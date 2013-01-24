var tizen = require( './tizen-native.node' );

exports.test_startsWith = function( test ) {
	test.ok( tizen.Util.startsWith( 'ABC', '' ) );
	test.ok( tizen.Util.startsWith( 'ABC', 'A' ) );
	test.ok( tizen.Util.startsWith( 'ABC', 'AB' ) );
	test.ok( tizen.Util.startsWith( 'ABC', 'ABC' ) );
	test.done();
};

exports.test_endsWith = function( test ) {
	test.ok( tizen.Util.endsWith( 'ABC', '' ) );
	test.ok( tizen.Util.endsWith( 'ABC', 'C' ) );
	test.ok( tizen.Util.endsWith( 'ABC', 'BC' ) );
	test.ok( tizen.Util.endsWith( 'ABC', 'ABC' ) );
	test.done();
};

exports.test_addPath = function( test ) {
	test.equal( tizen.Util.addPath( '/ABC', '/C' ), '/ABC/C' );
	test.done();
};

exports.test_getFilenameFrom = function( test ) {
	test.equal( tizen.Util.getFilenameFrom( '/testdir/testfilename' ), 'testfilename' );
	test.done();
};

exports.test_getParentFrom = function( test ) {
	test.equal( tizen.Util.getParentFrom( '/testdir/testfilename' ), '/testdir' );
	test.done();
};

