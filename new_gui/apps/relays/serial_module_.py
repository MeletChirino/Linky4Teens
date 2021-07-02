import os
import sys
import glob
import serial
import time

from apps.start_block.views import generate_file_name

def serial_ports():
    """ Lists serial port names

        :raises EnvironmentError:
            On unsupported or unknown platforms
        :returns:
            A list of the serial ports available on the system
    """
    if sys.platform.startswith('win'):
        ports = ['COM%s' % (i + 1) for i in range(256)]
    elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
        # this excludes your current terminal "/dev/tty"
        ports = glob.glob('/dev/tty[A-Za-z]*')
    elif sys.platform.startswith('darwin'):
        ports = glob.glob('/dev/tty.*')
    else:
        raise EnvironmentError('Unsupported platform')

    result = []
    for port in ports:
        try:
            s = serial.Serial(port)
            s.close()
            result.append(port)
        except (OSError, serial.SerialException):
            pass
    #pop out default serial device
    index = result.index('/dev/ttyAMA0')
    result.pop(index)

    return result

def get_data():
    print("Connecting to serial device")
    temoin = serial.Serial(
            '/dev/ttyUSB0',
            9600,
            timeout=.1
            )
    file_name = generate_file_name()
    file_path = F"{file_name}.csv"
    print("esperalooo")
    time.sleep(4)
    print("sale\n")


    file = open(file_path, "a")

    temoin.write(b'start\n')
    incomming_data = b''
    llega = b''
    try:
        while not incomming_data == b'!':
            incomming_data = temoin.read()
            if( not(
                incomming_data == b'\n'
                or
                incomming_data == b'\r'
                )
            ):
                llega += incomming_data

            if(incomming_data == b'\n'):
                print(F'==> {ascii(llega.decode())}')
                file.write(F"{ascii(llega.decode())}\n")
                llega = b""
                incomming_data = b''
    except:
        pass

    temoin.close()


if __name__ == '__main__':
    get_data()

