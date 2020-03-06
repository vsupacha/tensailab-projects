// Import libraries
var express = require("express");
var bodyParser = require("body-parser");
var mqtt = require('mqtt');
var mongodb = require('mongodb');

// Initialize Express web services
var app = express();
app.use(bodyParser.json());
let port = process.env.PORT;
if (port == null || port == "") {
    port = 8000;
}
app.listen(port);
console.log('REST service started');

// Start MQTT services
const options={clientId:process.env.NETPIE_ID, username:process.env.NETPIE_TOKEN};
var mqttClient  = mqtt.connect('mqtt://mqtt.netpie.io', options);

// Start MongoDB connection
const MongoClient = mongodb.MongoClient;
const uri = process.env.DBASE_URI;
const dbaseClient = new MongoClient(uri, { useNewUrlParser: true, useUnifiedTopology: true });
dbaseClient.connect(err => {
    console.log('Database connected');    
    dbaseClient.close();
});

// URL endpoint for GET request, extract data from req.query object
app.get("/api/register", function(req, res) {
    console.log(req.query);
    if (req.query.name != null && req.query.name != "") {
        insertData(req.query.name)
        reply = {status:'OK'};
    } else {
        reply = {status:'ERR', info:'No name'};
    }
    res.status(200).json(reply);
});

// URL endpoint for POST request, extract data from req.body object
app.post("/api/register", function(req, res) {
    console.log(req.body);
    if (req.body.name != null && req.body.name != "") {
        insertData(req.body.name);
        reply = {status:'OK'};
    } else {
        reply = {status:'ERR', info:'No name'};
    }
    res.status(200).json(reply);
});

// Database connection handler
mqttClient.on('connect', function () {
    mqttClient.subscribe('@msg/tgr2020/followUp/#', function (err) {
        if (err) {
            console.log('Error subscribe to NETPIE MQTT')
        } else {
            console.log('Subscribed to NETPIE MQTT');
        }
    });
});

// Message reception handler
mqttClient.on('message', function(topic, message) {
    let msg = JSON.parse(message);
    if (msg.name) {
        insertData(msg.name);
    } 
    console.log(message.toString());
});

// Function to insert data
function insertData(name) {
    const dbaseClient = new MongoClient(uri, { useNewUrlParser: true, useUnifiedTopology: true });
    dbaseClient.connect(err => {
        if (err) {
            console.log(err);
            return;
        }
        const collection = dbaseClient.db("test").collection("devices");
        const device = {timestamp:new Date().toISOString(), name:name};
        collection.insertOne(device, function(err, result) {
            if (err) {
                console.log(err);
                return;
            }
            console.log('Collection inserted');
            dbaseClient.close();
        });                            
    });
}