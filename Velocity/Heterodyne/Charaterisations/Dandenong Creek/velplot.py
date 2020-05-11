import csv
from datetime import datetime
import numpy as np
import matplotlib.dates as mpldates
import matplotlib.pyplot as plt

times = []
bat = []


Vel0 = []
Vel1 = []
Vel2 = []
Vel3 = []
Vel4 = []

Vels = [Vel0, Vel1, Vel2, Vel3, Vel4]

PVel0 = []
PVel1 = []
PVel2 = []
PVel3 = []
PVel4 = []

PVels = [PVel0, PVel1, PVel2, PVel3, PVel4]

Amp0 = []
Amp1 = []
Amp2 = []
Amp3 = []
Amp4 = []

Amps = [Amp0, Amp1, Amp2, Amp3, Amp4]

with open('VELOCITY_SITE_TES.csv', newline='') as csvfile:
	spamreader = csv.reader(csvfile, delimiter=',', quotechar='|')
	for i,row in enumerate(spamreader):
		if i < 46:
			continue
		times.append(datetime.strptime(row[0], '%d/%m/%y %I:%M:%S %p'))
		bat.append(float(row[1]))
		Vel0.append(float(row[6]))
		Amp0.append(float(row[7]))
		Vel1.append(float(row[8]))
		Amp1.append(float(row[9]))
		Vel2.append(float(row[10]))
		Amp2.append(float(row[11]))
		Vel3.append(float(row[12]))
		Amp3.append(float(row[13]))
		Vel4.append(float(row[14]))
		Amp4.append(float(row[15]))


for set,lst in enumerate(Vels):		
	for i,val in enumerate(Vels[set]):
		if(Amps[set][i] > 1000):
			PVels[set].append(val)
		else:
			PVels[set].append(float("nan"))
		
pltimes = mpldates.date2num(times)


PVels3Adj = []
PVels4Adj = []
for val in PVels[3]:
	PVels3Adj.append(1750-val)
for val in PVels[4]:
	PVels4Adj.append(val-2200)

ave = []
for i,val in enumerate(PVels[0]):
	ave.append((PVels[0][i] + PVels[1][i] + PVels[2][i])/3)
		
# plt.scatter(Amps[0],Vels[0])
# plt.scatter(Amps[1],Vels[1])
# plt.scatter(Amps[2],Vels[2])
# plt.scatter(Amps[3],Vels[3])
# plt.scatter(Amps[4],Vels[4])
plt.scatter(pltimes,PVels[0])
plt.scatter(pltimes,PVels[1])
plt.scatter(pltimes,PVels[2])
plt.scatter(pltimes,PVels3Adj)
plt.scatter(pltimes,PVels4Adj)

plt.xlabel("Amplitude")
plt.ylabel("Velocity (mm/s)")

plt.show()
