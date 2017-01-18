import MySQLdb
import serial
import time
from datetime import datetime, date

ser = serial.Serial('/dev/ttyUSB0', 9600)
db = MySQLdb.connect("localhost", "monitor", "password", "database")
mode = 0;

#CREATE TABLE tempdat (date CHAR(12), time CHAR(10) ,info VARCHAR(20), DHTtemp FLOAT, DHThum FLOAT, soilHum SMALLINT, Light SMALLINT);


cursor = db.cursor()
print("-----------------------------")
print("|SQL data grabber - ABP V0.1|")
print("-----------------------------")
print(" ")
print("Starting conection time...")

while True:

	if mode==0:
		#print(ser.readline())
		x = ser.readline()
		if "c" in x:
			mode = 1
			print("")
			print("Starting data Gathering...")
			print(time.strftime('%H:%M:%S')+ " " + str(datetime.now().date()))
			print("-----------------------------")
			print("")
	if mode==1:


		#date1 = date1.strftime("%Y-%m-%d')
		#time1 = datetime.now().time()
		#time1 = str(time1)
		#time1 = time1[:-4]

		print("Waiting for more data...")
		print("++++++++++++++++++++++++")

		time1 = time.strftime('%H:%M:%S')
		date1 = datetime.now().date()

		x1 = ser.readline()
		x1 = x1[:-2]
		print("Temperature is: " + x1 + "*C")
		x1 = float(x1)
		
		x2 = ser.readline()
		x2 = x2[:-2]
		print("Air humidity is: " + x2 + "%")
		x2 = float(x2)
		
		y = ser.readline()
		y = y[:-2]
		print("Soil moisture rate is: " + y + " (0-1023)")
		y = int(y)
		
		z = ser.readline()
		z = z[:-2]
		print("Light rate is: " + z + " (0-1023)")
		z = int(z)	
		
		print("")
		print("Saving sql data")
		print("time: " + str(time1) + " date: " + str(date1))
		sql = """INSERT INTO tempdat(date, time, info, DHTtemp, DHThum, soilHum, Light) VALUES ("%s", "%s", "6 W LED", %f, %f, %d, %d)""" %(date1, time1,x1,x2,y,z)
		cursor.execute(sql)
		db.commit()
		print("------------------------")
		print(" ")
		print("------------------------")




