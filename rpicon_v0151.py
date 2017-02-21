import MySQLdb
import serial
import time
from datetime import datetime, date

ser = serial.Serial('/dev/ttyUSB1', 9600)
db = MySQLdb.connect("localhost", "monitor", "password", "prueba1")

# def var that store if lights are on
lights1 = False
lights2 = False

# MYSQL TABLE tempdat (date CHAR(12), time CHAR(10) ,info VARCHAR(20), DHTtemp FLOAT, DHThum FLOAT, soilHum SMALLINT, Light SMALLINT);


def getdata():
    ser.write("0")

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
    sql = """INSERT INTO tempdat(date, time, info, DHTtemp, DHThum, soilHum, Light) VALUES ("%s", "%s", "LED CREC", %f, %f, %d, %d)""" % (
    date1, time1, x1, x2, y, z)
    cursor.execute(sql)
    db.commit()
    print("------------------------")
    print(" ")
    print("------------------------")


def light1ON():
    ser.write("1")
    lights1 = True
    time.sleep(5)
    print(ser.readline())


def light2ON():
    ser.write("2")
    lights2 = True
    time.sleep(5)
    print(ser.readline())


def light1OFF():
    ser.write("3")
    lights1 = False
    time.sleep(5)
    print(ser.readline())


def light2OFF():
    ser.write("4")
    lights2 = False
    time.sleep(5)
    print(ser.readline())


def fan1ON():
    ser.write("5")
    time.sleep(5)
    print(ser.readline())


def fan2ON():
    ser.write("6")
    time.sleep(5)
    print(ser.readline())


def fan1OFF():
    ser.write("7")
    time.sleep(5)
    print(ser.readline())


def fan2OFF():
    ser.write("8")
    time.sleep(5)
    print(ser.readline())


def ledfanON():
    ser.write("9")
    time.sleep(5)
    print(ser.readline())


def ledfanOFF():
    ser.write(":")
    time.sleep(5)
    print(ser.readline())


def ontime1():
    now = datetime.now()
    on1 = now.replace(hour=7, minute=0, second=0, microsecond=0)     # here time when turns on
    off1 = now.replace(hour=23, minute=59, second=0, microsecond=0)  # here time when turns off
    if now < off1 and now > on1:
        return True
    return False


def ontime2():
    now = datetime.now()
    on2 = now.replace(hour=10, minute=0, second=0, microsecond=0)    # here time when turns on
    off2 = now.replace(hour=20, minute=59, second=0, microsecond=0)  # here time when turns off
    if now < off2 and now > on2:
        return True
    return False


# Main

cursor = db.cursor()
print("--------------------------------")
print("|Arduino controller - ABP V0.15|")
print("--------------------------------")
print("|-----niko9797 on jan-2017------")
print("--------------------------------")
print("-----Press ctrol+c to end-------")
print("--------------------------------")
print("Starting conection time...")

# handshake
while True:

    x = ser.readline()
    if "c" in x:
        print("")
        print("Starting data Gathering...")
        print(time.strftime('%H:%M:%S') + " " + str(datetime.now().date()))
        print("-----------------------------")
        print("")
        break


# main loop


while True:
    try:
        # gets the data from sensors and puts it in the MySQL DB
        getdata()

        print("lights 1?: " + str(ontime1()))
        print("lights 2?: " + str(ontime2()))
        print("Lights 1 on?: " + str(lights1))
        print("Lights 2 on?: " + str(lights2))

        # turns on lights if necessary
        if ontime1() is False:
            if lights1 is True:
                light1OFF()
                lights1 = False
                time.sleep(5)
        else:
            if lights1 is False:
                light1ON()
                lights1 = True
                ledfanON()
        if ontime2() is False:
            if lights2 is True:
                light2OFF()
                lights2 = False
        else:
            if lights2 is False:
                light2ON()
                lights2 = True
                ledfanON()

        # turns on the fan over the led if some light is on
        if lights1 is False and lights2 is False:
            ledfanOFF()

        # makes some airflow through the bucket
        fan1ON()
        time.sleep(20)
        fan1OFF()
        fan2ON()
        time.sleep(20)
        fan2OFF()

        # adds some extra time to take lectures at night
        if lights1 is False and lights2 is False:
            time.sleep(30)

    # exit on ctrol+c
    except KeyboardInterrupt:
        db.close()
        light1OFF()
        light2OFF()
        ledfanOFF()
        time.sleep(5)
        break
