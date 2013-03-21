/*Contacts*/
pp.get( '/contacts', routes.contacts );
app.post( '/categories', routes.categories.add );
app.delete( '/categories/:name', routes.categories.remove ); // categories delete
app.put( '/categories/:name', routes.categories.move ); // categories name change

app.post( '/contacts', routes.contacts.add ); // contacts add
app.delete( '/contacts', routes.contacts.remove ); // categories delete
