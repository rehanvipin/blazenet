import serial
import webbrowser


ser = serial.Serial('COM3',9600)

print("starting")

dhum = 0
dtemp = 0
dppm = 0

ppm_limit = 200
hum_limit = 10
tmp_limit = 5

vals = []

with open('diffs.txt','wb') as wir:
	while True:
		try:

			# Getting a tuple of values, ppm, temperature, 
			for i in range(3):

				if(i==0):
					dppm = ser.readline()
					cleanse = dppm.decode().strip()
					data = dppm
				elif(i==1):
					dhum = ser.readline()
					cleanse = dhum.decode().strip()
					data = dhum
				else:
					dtemp = ser.readline()
					cleanse = dtemp.decode().strip()
					data = dtemp
					tmp_block = (dppm,dhum,dtemp)
					tmp_block = tuple([abs(float(x.decode().strip())) for x in tmp_block])
					vals.append(tmp_block)
					check_block = [(tmp_block[0]>ppm_limit),(tmp_block[1]>hum_limit),(tmp_block[2]>tmp_limit)]

				# Printing out for verbosity
				print(cleanse)
				wir.write(data)

			print(tmp_block)
			print(check_block)
			if(any(check_block)):
				webbrowser.open('file:///C:/Users/rehan/Desktop/Hashcode/gametime/comms/alert.html')
				break


		# Silly escape method
		except KeyboardInterrupt as k:
			break

print("Done for now")