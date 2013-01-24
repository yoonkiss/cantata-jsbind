var tizen = require( './tizen-native.node' );

exports.test_getPhonenumber = function( test ) {
	tizen.System.getPhonenumber()
}

