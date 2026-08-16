const express = require("express");

const app = express();

app.get("/", (req, res) => {
  res.send(`
    <html>
      <head><title>My Web Server</title></head>
      <body>
        <form action="/message" method="POST">
          <input type="text" name="message">
          <button type="submit">Submit</button>
        </form>
      </body>
    </html>
  `);
});

app.use((req, res) => {
  res.send("<html><head><title>My Web Server</title></head><body><h1>Hello, World!</h1></body></html>");
});

app.listen(3000, () => {
  console.log("Server is running on http://localhost:3000");
});