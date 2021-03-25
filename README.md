# Event Sockets
### Library for abstracted POSIX type sockets with callbacks on different operations.

<br/>

## Examples
```cpp
// Listener socket
EventSocket listener = new EventSocket();
listener.Bind("127.0.0.1", 8888, AF_INET);
listener.Listen(10);
```
```cpp
// Client socket
EventSocket client = new EventSocket();
client.Bind("127.0.0.1", 8889, AF_INET);
client.Connect("127.0.0.1", 8888, AF_INET);
```
