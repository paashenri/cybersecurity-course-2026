const http = require("http");

const server = http.createServer((req, res) => {
  console.log(req.headers, req.method, req.url, req.body);
});

server.listen(3000, () => {
  console.log("Server is running on http://localhost:3000");
});
