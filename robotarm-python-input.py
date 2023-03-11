## Key codes in: https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes?source=recommendations
import time
import win32api
import serial

startAutomation = False
automationKeyPress = 0

# Open and set serial port for
arduino = serial.Serial('com3', 9600, timeout=0.01)

while 1:
	commands = ""
	# Detect "left shift" key for speed boost
	if win32api.GetKeyState(0xA0) < 0:
		commands += "b"

	# Detect "W" key for lower arm
	if win32api.GetKeyState(0x57) < 0:
		commands += "w"
	# Detect "S" key for lower arm
	elif win32api.GetKeyState(0x53) < 0:
		commands += "s"

	# Detect "A" key for base arm
	if win32api.GetKeyState(0x41) < 0:
		commands += "a"
	# Detect "D" key for base arm
	elif win32api.GetKeyState(0x44) < 0:
		commands += "d"

	# Detect "UP ARROW" key for upper arm
	if win32api.GetKeyState(0x26) < 0:
		commands += "8"
	# Detect "DOWN ARROW" key for upper arm
	elif win32api.GetKeyState(0x28) < 0:
		commands += "2"

	# Detect "RIGHT ARROW" key for claw arm
	if win32api.GetKeyState(0x27) < 0:
		commands += "6"
	# Detect "LEFT ARROW" key for claw arm
	elif win32api.GetKeyState(0x25) < 0:
		commands += "4"


	if commands != "":
		startAutomation = False
	# Start automation
	if win32api.GetKeyState(0x4F) < 0 or startAutomation == True:
		commands += "o"
		startAutomation = True
	if win32api.GetKeyState(0x50) < 0:
		commands += "p"
		startAutomation = False
	if win32api.GetKeyState(0x4C) < 0 and time.time() > automationKeyPress + 1:
		commands += "l"
		startAutomation = False
		automationKeyPress = time.time()
	elif win32api.GetKeyState(0x4B) < 0 and time.time() > automationKeyPress + 1: 
		commands += "k"
		startAutomation = False
		automationKeyPress = time.time()




	# If there is key input send this string via serial port.
	if commands != "":
		print("sending: "+commands)
		arduino.write(bytes(commands, 'utf-8'))
		# Reads nothing since arduino doesn't transmit back. Code gets borken for some reason if this is removed.
		data = arduino.readline()
		print(data.decode('utf-8'))

