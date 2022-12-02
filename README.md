# tcp-server-client-ping-pong-timing
C++ socket ping-pong timing

The base source code belongs to:
https://simpledevcode.wordpress.com/2016/06/16/client-server-chat-in-c-using-sockets/

To use on Linux:

Open terminal and copy-paste following lines:

```
mkdir build
cd build
cmake ..
make
./server 9999
```

Open another terminal and copy-paste following lines:

```
./client 127.0.0.1 9999 10000
```

Note: This is for only Local usage.
