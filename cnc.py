import serial
import time

ser = serial.Serial("/dev/cu.usbmodem14101", 9600, parity=serial.PARITY_EVEN)

print("Waiting for ARDC to do it's thing...")

time.sleep(3);

print("Let's go!")

print("\nGimme da report!")
ser.write(b"freport\n")

while True:
    s = ser.readline()
    print(s)