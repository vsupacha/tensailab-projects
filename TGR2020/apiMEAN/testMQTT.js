var mqtt = require('mqtt');
const options={clientId:process.env.NETPIE_ID, username:process.env.NETPIE_TOKEN};
var mqttClient  = mqtt.connect('mqtt://mqtt.netpie.io', options);

mqttClient.on('connect', function () {
    mqttClient.subscribe('@msg/tgr2020/followUp/#', function (err) {
        if (err) {
            console.log('Error subscribe to NETPIE MQTT')
        } else {
            console.log('Subscribed to NETPIE MQTT');
        }
    });
});
  
mqttClient.on('message', function(topic, message) {
    console.log(topic);
    let msg = JSON.parse(message);
    console.log(msg);
    let echoTopic = '@msg/tgr2020/echo/';
    var substr = topic.split('/');
    echoTopic = echoTopic + substr[substr.length-1];
    console.log(echoTopic);
    mqttClient.publish(echoTopic, msg.toString());
});