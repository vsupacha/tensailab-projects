# Building API server with MEAN stack

## MEAN เฟส 1 เตรียม Node.js

### Node.js ใน Windows
1. ดาวน์โหลดและติดตั้ง Node.js จาก https://nodejs.org/en/download/
2. พิมพ์คำสั่งเพื่อทดสอบ node และ npm
   ```
   node --version
   npm --version
   ```
3. ติดตั้งไลบรารี express mqtt body-parser และ mongodb
   ```
   npm install -g express mqtt body-parser mongodb
   ```
4. ใช้ Control Panel > System and Security > System จากนั้นเลือก Advanced system setting แล้วตั้งค่า **Environment variables** เพื่อตั้งค่า `NODE_PATH` ชี้ไปที่ `C:\Users\<USER_NAME>\AppData\Roaming\npm\node_modules`

### Node.js ใน Windows Subsystem for Linux
1. ติดตั้ง Windows Subsystem for Linux หรือ Ubuntu ใน VMware Player
   - ไดเร็กทอรีของ Windows Subsystem for Linux อ้างด้วย `\\wsl$`
2. ติดตั้ง Node.js จาก PPA
   ```
   curl -sL https://deb.nodesource.com/setup_12.x | sudo -E bash -
   sudo apt install -y nodejs
   ```
3. พิมพ์คำสั่งเพื่อทดสอบ node และ npm
   ```
   node --version
   npm --version
   ```
4. ติดตั้ง express mqtt และ mongodb
   ```
   sudo npm install -g express mqtt body-parser mongodb
   ```

## MEAN เฟส 2 ทดสอบ Express
1. โค้ดตัวอย่าง **testExpress.js** สาธิตการสร้าง URL endpoint `/api/register` สำหรับ GET และ POST 
2. พิมพ์คำสั่งเพื่อประกาศตัวแปร environmental variables
   ```
   export PORT=3000
   ```
3. พิมพ์คำสั่ง `node testExpress.js` เพื่อเริ่มเว็บบริการทาง `http://localhost:3000/api/register`
4. ใช้ Postman ในการทดสอบ
	- ร้องขอแบบ GET ไปที่ `http://localhost:3000/api/register?name=me`
	- ร้องขอแบบ POST ไปที่ `http://localhost:3000/api/register` โดยเพิ่ม header คือ 'Content-Type' เป็น 'application/json' และให้ body เป็นแบบ RAW ด้วยข้อความ `{"name":"me"}`
	
## MEAN เฟส 2.5 เตรียมการและทดสอบ MQTT

### การเตรียม MQTT broker
1. ลงทะเบียนที่ https://portal.netpie.io/
2. สร้าง project 
3. ลงทะเบียน device จำนวน 3 ชุดสำหรับบอร์ด/คอมพิวเตอร์/เซิร์ฟเวอร์
   - ข้อมูล client ID ใช้เป็น ID
   - ข้อมูล token ใช้เป็น username
4. สร้าง device group 
   - คลิกเลือก device แล้ว Move ไปอยู่ใน group เดียวกัน
5. ส่วน topic ใช้ @msg นำข้อความ topic เพื่อสื่อสารในกลุ่มเดียวกัน

### การทดสอบ MQTT topic
1. โค้ดตัวอย่าง **testMQTT.js** สำหรับสาธิตการ subscribe รับ message ทั้งหมดจาก topic `@msg/tgr2020/followUp/#`
2. พิมพ์คำสั่งเพื่อประกาศตัวแปร environmental variables
   ```
   export NETPIE_ID='client ID from NETPIE device registration'
   export NETPIE_TOKEN='token from NETPIE device registration'
   ```
3. พิมพ์คำสั่ง `node testMQTT.js` เพื่อเชื่อมต่อ MQTT broker
4. ทดสอบการเชื่อมต่อผ่าน MQTT ไปที่ topic `@msg/tgr2020/followUp/1` โดยพิมพ์คำสั่ง mqtt เพื่อส่งข้อความ JSON ด้วย id/token ของ **device ชุดที่ 2** 
	```
	mqtt publish -t '@msg/tgr2020/followUp/1' -m '{"name":"YOUR NAME"}' -h 'mqtt.netpie.io' -i 'second client ID from NETPIE device registration' -u 'second client token from NETPIE device registration'
	```	
4. ทดสอบการรับค่าที่ echo ทาง topic `@msg/tgr2020/echo/1` โดยพิมพ์คำสั่งเพื่อรับข้อความจาก NETPIE ด้วย id/token ของ **device ชุดที่ 3** 
	```
	mqtt subscribe -t '@msg/tgr2020/echo/1' -h 'mqtt.netpie.io' -i 'third client ID from NETPIE device registration' -u 'third client token from NETPIE device registration'
	```	

## MEAN เฟส 3 สร้าง frontend ด้วย Angular 
เนื้อหาจะเพิ่มในการแก้ไขรอบหน้า
	
## MEAN เฟส 4 ฐานข้อมูล MongoDB

### การสร้างฐานข้อมูล
1. ลงทะเบียน mongodb cloud ที่ https://www.mongodb.com/download-center
2. เพิ่ม user โดยเลือกจากเมนู Security > Database Access
3. เพิ่ม IP address 0.0.0.0 (public access) ใน whitelist โดยเลือกจากเมนู Security > Network Access 
4. เลือก Connection to Application แล้วบันทึก URL หรือโค้ดตัวอย่างสำหรับ Node.js เพื่อเชื่อมต่อฐานข้อมูล (scheme คือ `mongodb+srv://`)
5. ประกาศตัวแปร environmental variables เช่นตัวอย่าง
   ```
   export DBASE_URI='mongodb+srv://DBASE_USER:DBASE_PASSWD@cluster0-0weff.mongodb.net/test?retryWrites=true&w=majority'
   ```
   
### โค้ดทดสอบ local server 
1. **apiMEAN.js** 
	1.1 ทดสอบการเชื่อมต่อแบบ local ด้วย Postman ไปที่ http://localhost:3000    
		- เชื่อมต่อด้วย GET ไปที่ /api/greeting โดยมี/ไม่มี parameter คือ name จะคืนคำตอบเป็น JSON แจ้ง status เป็น OK หรือ ERR
		- เชื่อมต่อด้วย POST ไปที่ /api/greeting โดยส่ง JSON แบบมี/ไม่มี key คือ name จะคืนคำตอบเป็น JSON แจ้ง status เป็น OK หรือ ERR
		- ทั้งสอง endpoint จะบันทึกเวลาและ name ไปที่ฐานข้อมูล
	1.2 ทดสอบด้วยการใช้คำสั่ง mqtt publish ไปที่ '@msg/tgr2020/echo/1'

โค้ดตัวอย่างจะเพิ่ม API สำหรับค้นหาในรอบหน้า

## MEAN เฟส 5 ออนไลน์ด้วย Heroku
เนื้อหาจะเพิ่มในการแก้ไขรอบหน้า

### การเตรียม Heroku server 
1. ลงทะเบียน Heroku ที่ https://heroku.com/
2. สร้าง project ใหม่
3. ดาวน์โหลดและติดตั้ง Heroku CLI จาก https://devcenter.heroku.com/articles/heroku-cli
4. พิมพ์คำสั่งในโฟลเดอร์ของ project เพื่อล็อกอิน เชื่อมต่อ Heroku git และตั้งค่า environmental variables
   ```
   heroku login
   git init
   heroku git:remote -a <PROJ_NAME>
   heroku stack:set container
   ```   
5. พิมพ์คำสั่งเพื่อส่งไฟล์ไป Heroku ด้วย git
   ```
   git add .
   git commit -m "Init repo"
   git push heroku master
   ```   

## ข้อมูลอ้างอิง
1. [Create a Web App and RESTful API Server Using the MEAN Stack](https://devcenter.heroku.com/articles/mean-apps-restful-api)
2. [How to Install MongoDB on Ubuntu 18.04](https://www.digitalocean.com/community/tutorials/how-to-install-mongodb-on-ubuntu-18-04)
3. [MongoDB Node.js Driver](https://docs.mongodb.com/ecosystem/drivers/node/)