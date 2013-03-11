cantata-jsbind
==============

### Test

테스트는 nodeunit을 사용한다.

테스트를 위한 테스트케이스는 `test/src/*.js`로 작성되어 있다.

nodeunit을 설치하기 위해서는 다음의 명령을 사용한다.

```
sudo npm i nodeunit -g

cd test/src

nodeunit *.js
```

개발을 위해서는 CI를 추가 설치하는 것을 추천한다.

```
sudo npm i nodeunit-watcher -g

cd test/src

nodeunit-watcher *.js
```


다른 방법으로, curl 을 이용한 테스트도 가능하다.

```
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
```

기타 개발중 팁으로, bind 코드 빌드시 바로 변경된 파일을 cantata-native 프로젝트로 복사해주는 코드를 사용한다.
/home/hyunsik/tizen-sdk/tools push /home/hyunsik/git/tizen-air/cantata-jsbind/Debug-Tizen-Emulator/tizen-native.node /opt/apps/hF1jRugBXt/res/routes



### To build your project
add include path: C/C++ build > Settings > Tool Settings > C++ Compiler > Includes

~/your working directory/cantata-node/node/src

~/your working directory/cantata-node/node/deps/uv/include

~/your working directory/cantata-node/node/deps/v8/include


### Reference
[V8 Document](https://developers.google.com/v8/?hl=ko)

[V8 API Reference](http://izs.me/v8-docs/main.html)

http://nodejs.org/api/addons.html

https://www.cloudkick.com/blog/2010/aug/23/writing-nodejs-native-extensions

http://syskall.com/how-to-write-your-own-native-nodejs-extension/index.html
