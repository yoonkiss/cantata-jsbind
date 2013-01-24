#!/usr/bin/node

var tizen = require( './tizen-native.node' );



console.log( tizen.Util.startsWith( 'ABC', 'AB' ) );
console.log( tizen.Util.endsWith( 'ABC', 'C' ) );
console.log( tizen.Util.addPath( '/ABC', '/C' ) );
console.log( tizen.Util.getFilenameFrom( '/testdir/testfilename' ) );
console.log( tizen.Util.getParentFrom( '/testdir/testfilename' ) );

