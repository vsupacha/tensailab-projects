import paho.mqtt.client as mqtt
import time
import socket

MQTTbroker = 'mqtt.netpie.io'
id = 'xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx'
token = 'xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx'
topic = '@msg/tgr2020/followUp/1'

def on_connect(client, userdata, flags, rc):
    client.subscribe(topic)
    print('Subscribed')
    
def on_message(client, userdata, msg):
    print(msg.topic, ':', msg.payload)

def on_disconnect(client, userdata, rc):
    print(userdata)
    
client = mqtt.Client(client_id=id)
client.username_pw_set(token)
client.connect(MQTTbroker)
client.on_connect = on_connect
client.on_message = on_message
client.on_disconnect = on_disconnect
client.socket().setsockopt(socket.SOL_SOCKET, socket.SO_SNDBUF, 2048)
client.loop_forever()

while True:
    time.sleep(10)
    print('Waiting')