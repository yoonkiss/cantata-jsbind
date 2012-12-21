INC_NODE=/usr/include/nodejs
INC_OSP=/home/bylee/tizen_sdk/platforms/tizen2.0/rootstraps/tizen-emulator-2.0.cpp/usr/include/osp
INC=-I$(INC_NODE) -I$(INC_OSP)
CFLAGS=-g $(INC)

all : obj/tizen-native.o
	gcc -shared -o tizen-native.so obj/tizen-native.o
obj/tizen-native.o : src/tizen-native.cpp
	mkdir -p obj
	gcc ${CFLAGS} -c src/tizen-native.cpp -o obj/tizen-native.o


clean:
	rm -rf obj
	rm -f tizen-native.so
