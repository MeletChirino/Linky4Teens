from time import sleep
import serial

def read_data(serial):
    incomming_data = ""
    byte_value = b""
    result = {'angle':1}

    while not incomming_data == b"/":
        incomming_data = serial.read()
        if(not incomming_data == b"/"):
            byte_value += incomming_data

    result['angle'] = float(byte_value)

    byte_value = b""

    while not incomming_data == b"\r":
        incomming_data = serial.read()

        if(not (incomming_data == b"\n"
            or incomming_data == b"\r")):

            byte_value += incomming_data

    result['time'] = float(byte_value)

    return result

def main():
    print("starting program")
    temoin = serial.Serial('/dev/ttyUSB0',115200, timeout=.1)
    stop = "a"
    input("pulsa enter")
    temoin.write(b'start\n')
    n = 0
    data = []
    total_data = 32000

    while n <= total_data:
        print(str((n/total_data)*100)+"%")

        data.append(read_data(temoin))

        n+=1


    temoin.write(b'start\n')
    temoin.close()

    print("Datos en memoria = ")
    for number in data:
        print(number)

    print("\n\nfinish Program")

    exit()

main()
