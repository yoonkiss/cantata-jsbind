
/*Contacts*/
rts.contacts = function( req, res ) {
    console.log( stringify( native.Contacts.list() ) );     
	res.send( stringify( native.Contacts.list() ) );     
};

exports.categories = function() {
};

exports.categories.add = function( req, res ) {
	console.log( 'Name: ' + req.body.name );
	if ( !native.Contacts.isExistCategory( req.body.name ) ) {
		var contacts = native.Contacts.addCategory( req.body.name );
		res.end();
		context.io.sockets.emit( 'categoryAdded', [ req.body.name ] );
	} else {
		res.send( 404, req.body.name + 'is already exist category ' );
		res.end();
	}
};

exports.categories.remove = function( req, res ) {
	var name = req.param('name');
	var force = req.body.force;
	console.log( 'Name: ' + name );
	console.log( 'Force: ' + force );

	if ( native.Contacts.isExistCategory( name ) ) {
		var contacts = native.Contacts.removeCategory( name, force );
		res.end();
		context.io.sockets.emit( 'categoryRemoved', {'categories':name, 'force':force} );
	} else {
		res.send( 404, req.body.name + 'is not exist category ' );
		res.end();
	}
};

exports.categories.move = function( req, res ) {
	var oldName = req.param('name');
	var newName = req.body.name;
	console.log( 'OLD Name: ' + oldName );
	console.log( 'NEW Name: ' + newName );

	if ( native.Contacts.isExistCategory( oldName ) ) {
		var contacts = native.Contacts.renameCategory( oldName, newName );
		res.end();
		context.io.sockets.emit( 'categoryChanged', {'oldName':oldName, 'newName':newName} );
	} else {
		res.send( 404, req.body.name + 'is not exist category ' );
		res.end();
	}
};

exports.contacts.add = function( req, res ) {
	console.log( 'Category: ' + req.body.category );
	console.log( 'Name: ' + req.body.name );
	console.log( 'Number: ' + req.body.number );
	
	if ( native.Contacts.isExistCategory( req.body.category ) ) {
		var contacts = native.Contacts.add( req.body );
		res.end();
		context.io.sockets.emit( 'contactAdded', [ req.body.name ] );
	} else {
		res.send( 404, req.body.category + 'is not exist category ' );
		res.end();
	}
};

exports.contacts.remove = function( req, res ) {
	console.log( 'Id: ' + req.body.id );
	if ( native.Contacts.isExistCategory( req.body.category ) ) {
		var contacts = native.Contacts.remove( req.body.id );
		res.end();
		context.io.sockets.emit( 'contactRemoved', [ req.body.id ] );
	} else {
		res.send( 404, req.body.category + 'is not exist category ' );
		res.end();
	}
};
