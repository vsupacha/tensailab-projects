# Demonstration of BLE tracking
โค้ด Arduino ตัวอย่างสำหรับสาธิตการใช้บอร์ด ESP32 เพื่อสแกนหาอุปกรณ์ Bluetooth แล้วรายงานด้วยโพรโทคอล MQTT ไปที่ NETPIE broker จากนั้นใช้โค้ด Python บน Jupyter Notebook เพื่อรับข้อมูลมาบันทึกเป็นไฟล์ CSV ก่อนทำการจำแนกตำแหน่งด้วยเทคนิค machine learning

## การเตรียม MQTT broker ใหม่
1. ลงทะเบียนที่ https://portal.netpie.io/
2. สร้าง project 
3. ลงทะเบียน device สำหรับบอร์ด/คอมพิวเตอร์/เซิร์ฟเวอร์
   - ข้อมูล client ID ใช้เป็น ID
   - ข้อมูล token ใช้เป็น username
4. สร้าง device group 
   - คลิกเลือก device แล้ว Move ไปอยู่ใน group เดียวกัน
5. ส่วน topic ใช้ @msg นำข้อความ topic เพื่อสื่อสารในกลุ่มเดียวกัน

## ESP32
1. ติดตั้ง Arduino Core for ESP32 
2. ใช้ตัวอย่าง BLE_notify
3. แก้ SERVICE_UUID และ CHARACTERISTIC_UUID ให้สอดคล้องกับ NOTI_UUID ของ BLE gateway  
   ```
   #define SERVICE_UUID        "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx"
   #define CHARACTERISTIC_UUID "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx"
   ```

## BLE gateway

### การเตรียมตัว
1. ติดตั้ง Python3
2. ติดตั้งไลบรารี bleak paho และ asyncio
   ```
   pip install bleak paho-mqtt asyncio
   ```

### โค้ดตัวอย่าง
1. **scanBLE.py** สแกนอุปกรณ์ BLE แล้วพิมพ์ชื่อและ MAC ของอุปกรณ์
2. **scanDevice.py** สแกนอุปกรณ์ BLE ที่กำหนดด้วย MAC แล้วพิมพ์ค่า RSSI ที่ตรวจพบ
   - แก้รายการ MAC ของอุปกรณ์ 
   ```
   mac_addrs = ("xx:xx:xx:xx:xx:xx", "xx:xx:xx:xx:xx:xx")
   ```
3. **scanDeviceEventMQTT.py** สแกนอุปกรณ์ BLE ที่กำหนดด้วย MAC แล้วส่งข้อมูลไป MQTT broker
   - แก้ข้อมูลของ MQTT broker และ topic
   ```
   MQTTbroker = 'mqtt.netpie.io'
   id = 'xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx'
   token = 'xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx'
   topic = '@msg/tgr2020/followUp/1'
   ```  
   - แก้รายการ MAC ของอุปกรณ์ที่ต้องการตรวจจับ 
   ```
   mac_addrs = ("xx:xx:xx:xx:xx:xx", "xx:xx:xx:xx:xx:xx")
   ```
   - แก้ NOTIFICATION UUID
   ```
   NOTI_UUID = ('xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx')
   ```
4. **mqttSub.py** ทดสอบ subscribe ไปยัง MQTT broker
   - แก้ข้อมูลของ MQTT broker และ topic
   ```
   MQTTbroker = 'mqtt.netpie.io'
   id = 'xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx'
   token = 'xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx'
   topic = '@msg/tgr2020/followUp/1'
   ``` 

### ขั้นตอน
1. เรียกใช้ **scanBLE.py** เพื่อสแกนหา MAC โดยดูจากชื่ออุปกรณ์
2. ทดสอบการเชื่อมต่อโดยใช้ **scanDevice.py**
3. เรียกใช้ **scanDeviceEventMQTT.py** เพื่อตรวจจับแล้วส่งค่าไป NETPIE 
   - ดูผลการตรวจจับและ JSON ที่ส่งได้จากหน้าจอ command terminal
4. ทดสอบการดึงข้อมูลด้วย **mqttSub.py**

## Machine learning

### การเตรียมตัว
1. ติดตั้ง jupyter pandas และ tensorflow
   ```
   sudo pip3 install jupyter pandas tensorflow paho-mqtt
   ```   

### โค้ดตัวอย่าง
1. **MLflow.ipynb** ไฟล์ Jupyter notebook สำหรับการทำ machine learning

### ขั้นตอน
1. ตั้งชื่อไฟล์ 'stage1.csv' แล้ว run เซลล์แรกเพื่อ subscribe ข้อมูลจาก NETPIE มาบันทึกข้อมูล
2. เรียกใช้ scanDeviceEventMQTT.py โดยตั้ง topic เป็น 1 (gateway = 1)
3. หลังจากบันทึกข้อมูลสัก 10 ค่า ให้หยุดการทำงาน
4. เริ่้มขั้น 1-3 ใหม่ โดยเปลี่ยนเป็น 2,3,... เพื่อแสดงถึงการเก็บข้อมูลใน scenario ที่แตกต่างกัน
5. ทำการ run เซลล์ที่สอง เพื่อรวบรวมและแปลงข้อมูลสำหรับใช้ train 
6. ทำการ run เซลล์ที่สาม เพื่อแปลงข้อมูลเป็น train dataset แล้วใช้ TensorFlow ในการ train model (**ยังมีปัญหาที่ไม่ converge**)
