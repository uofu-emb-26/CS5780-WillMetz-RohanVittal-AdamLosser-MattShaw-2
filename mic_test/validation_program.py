import serial

ser = serial.Serial('/dev/tty.usbserial-AB6ZMAG2', 115200)

while True:
    b = ser.read(1)          # read 1 byte
    value = b[0]             # convert byte to integer 0-255
    print(value)