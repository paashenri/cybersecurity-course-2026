const http = require("http");

const server = http.createServer((req, res) => {
  res.setHeader("Content-Type", "text/html");
  res.write(
    "<html><head><title>My Web Server</title></head><body><h1>Hello, World!</h1></body></html>",
  );
  res.end();
});

server.listen(3000, () => {
  console.log("Server is running on http://localhost:3000");
});
