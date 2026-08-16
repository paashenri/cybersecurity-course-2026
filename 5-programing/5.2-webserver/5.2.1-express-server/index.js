const express = require("express");

const app = express();

app.use((req, res, next) => {
  console.log(req.url, req.method, req.headers);
});

app.listen(3000, () => {
  console.log("Server is running on http://localhost:3000");
});
