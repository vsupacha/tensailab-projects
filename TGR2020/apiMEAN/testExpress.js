var express = require("express");
var bodyParser = require("body-parser");

var app = express();
app.use(bodyParser.json());

const port = process.env.PORT;
if (port == null || port == "") {
    port = 8000;
}
console.log('API service starting');
app.listen(port);

// URL endpoint for GET request
app.get("/api/register", function(req, res) {
    console.log(req.query);
    if (req.query.name != null && req.query.name != "") {
        reply = {status:'OK', method:'GET'};
    } else {
        reply = {status:'ERR', method:'GET'};
    }
    res.status(200).json(reply);
});

// URL endpoint for POST request
app.post("/api/register", function(req, res) {
    console.log(req.body);
    if (req.body.name != null && req.body.name != "") {
        reply = {status:'OK', method:'POST'};
    } else {
        reply = {status:'ERR', method:'POST'};
    }
    res.status(200).json(reply);
});
