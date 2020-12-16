from gpiozero import DigitalOutputDevice
def __init():
    """
    Setup GPIO 17 as digital output
    """
    fan_switch = DigitalOutputDevice(17,True, False) 

def fan_on():
    print("turning fan on")
    fan_switch.on()

def fan_off():
    print("turning fan off")
    fan_switch.off()



from gpiozero import CPUTemperature
from time import sleep

fan = DigitalOutputDevice(17)

TEMP_TH = 40.0 # CPU Temp threshold (C))

cpu = CPUTemperature()
if(cpu.temperature > TEMP_TH):
    print("turning fan on")
    fan.on()
    sleep(5)
else:
    print("turning fan off")
    fan.off()
