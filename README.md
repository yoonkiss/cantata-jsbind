cantata-jsbind
==============

/home/hyunsik/tizen-sdk/tools push /home/hyunsik/git/tizen-air/cantata-jsbind/Debug-Tizen-Emulator/tizen-native.node /opt/apps/hF1jRugBXt/res/routes

addcategory
curl -X POST -F "name=agew" http://localhost:3000/categories

removecategory
curl -X DELETE -F "force=agew" http://localhost:3000/categories/IDE2

renamecategory
curl -X PUT -F "name=WBC" http:///IDElhost:3000/categories/

addcontact
curl -X POST -F "name=agew" http://localhost:3000/contacts


‎‎Test request
  curl -i -X PUT -H 'Content-Type: application/json' -d '{"name": "New Wine", "year": "2010"}' http://localhost:3000/wines/5069b47aa892630aae000007
  curl -i -X POST -H 'Content-Type: application/json' -d '{"name": "New Wine", "year": "2009"}' http://localhost:3000/wines
