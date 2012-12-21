
var node = require( './tizen-native.node' );

contacts = new node.Contacts();
console.log( JSON.stringify( contacts.list() ) );

