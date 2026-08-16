const express = require("express");

const app = express();

app.use((req, res) => {
  res.setHeader("Content-Type", "text/html");
  res.send(
    "<html><head><title>My Web Server</title></head><body><h1>Hello, World!</h1></body></html>",
  );
});

app.listen(3000, () => {
  console.log("Server is running on http://localhost:3000");
});
