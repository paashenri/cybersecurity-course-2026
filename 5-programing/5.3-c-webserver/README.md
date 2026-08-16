# 5.3 C Webserver

These exercises repeat the webserver flow from 5.1 (Node) and 5.2 (Express), using C and POSIX sockets instead.

Each exercise is standalone. Build and run one exercise from its directory:

```bash
gcc -Wall -Wextra -Wpedantic -std=c11 -o server server.c
./server
```

Open `http://localhost:3000` in a browser. Stop the server with `Ctrl+C` before starting another exercise because all examples use port `3000`.

The exercises cover:

1. Reading and logging an HTTP request
2. Returning an HTML response
3. Routing `GET /` and rendering a form
4. Handling a form request and sending a redirect
5. Parsing an URL-encoded request body and saving the submitted message
