EESchema Schematic File Version 4
LIBS:BoSL Probe-cache
EELAYER 29 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "BoSL Water Probe"
Date "2019-08-15"
Rev "0.1"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text GLabel 3300 2900 0    50   Input ~ 0
OrangeHalf
Text GLabel 3300 3000 0    50   Input ~ 0
Orange
Text GLabel 3300 3100 0    50   Input ~ 0
GreenHalf
Text GLabel 3300 3200 0    50   Input ~ 0
Blue
Text GLabel 3300 3300 0    50   Input ~ 0
BlueHalf
Text GLabel 3300 3400 0    50   Input ~ 0
Green
Text GLabel 3300 3500 0    50   Input ~ 0
BrownHalf
Text GLabel 3300 3600 0    50   Input ~ 0
Brown
Text Notes 2750 2800 0    50   ~ 0
Eithernet Cable
Wire Wire Line
	3300 2900 3700 2900
Wire Wire Line
	3300 3000 3600 3000
Wire Wire Line
	3300 3100 3500 3100
Wire Wire Line
	3300 3200 3400 3200
Wire Wire Line
	3300 3400 3500 3400
Wire Wire Line
	3300 3500 3600 3500
Wire Wire Line
	3300 3600 3700 3600
$Comp
L Connector:8P8C_Shielded J1
U 1 1 5D54DE2A
P 4150 3200
F 0 "J1" H 3821 3148 50  0000 R CNN
F 1 "8P8C_Shielded" H 4450 3650 50  0000 R CNN
F 2 "Project:RJ_45" V 4150 3225 50  0001 C CNN
F 3 "~" V 4150 3225 50  0001 C CNN
	1    4150 3200
	-1   0    0    1   
$EndComp
Wire Notes Line
	4600 2700 4600 3750
Wire Wire Line
	3700 2900 3700 2650
Connection ~ 3700 2900
Wire Wire Line
	3700 2900 3750 2900
Wire Wire Line
	3600 3000 3600 2650
Connection ~ 3600 3000
Wire Wire Line
	3600 3000 3750 3000
Wire Wire Line
	3500 3100 3500 2650
Connection ~ 3500 3100
Wire Wire Line
	3500 3100 3750 3100
Wire Wire Line
	3400 3200 3400 2650
Connection ~ 3400 3200
Wire Wire Line
	3400 3200 3750 3200
Wire Wire Line
	3300 3300 3400 3300
Wire Wire Line
	3400 3300 3400 3800
Connection ~ 3400 3300
Wire Wire Line
	3400 3300 3750 3300
Wire Wire Line
	3500 3400 3500 3800
Connection ~ 3500 3400
Wire Wire Line
	3500 3400 3750 3400
Wire Wire Line
	3600 3500 3600 3800
Connection ~ 3600 3500
Wire Wire Line
	3600 3500 3750 3500
Wire Wire Line
	3700 3600 3700 3800
Connection ~ 3700 3600
Wire Wire Line
	3700 3600 3750 3600
Wire Notes Line
	2700 2700 2700 3750
Wire Notes Line
	2700 3750 4600 3750
Text GLabel 3700 3800 3    50   Input ~ 0
SDA
Text GLabel 3600 3800 3    50   Input ~ 0
A3
$Comp
L power:GND #PWR03
U 1 1 5D55B14E
P 3500 3800
F 0 "#PWR03" H 3500 3550 50  0001 C CNN
F 1 "GND" H 3505 3629 50  0000 C CNN
F 2 "" H 3500 3800 50  0001 C CNN
F 3 "" H 3500 3800 50  0001 C CNN
	1    3500 3800
	1    0    0    -1  
$EndComp
Text GLabel 3400 3800 3    50   Input ~ 0
DATA_TEMP
Text GLabel 3400 2650 1    50   Input ~ 0
SCL
Text GLabel 3500 2650 1    50   Input ~ 0
A1
$Comp
L power:+3.3V #PWR04
U 1 1 5D55B7FB
P 3600 2650
F 0 "#PWR04" H 3600 2500 50  0001 C CNN
F 1 "+3.3V" H 3615 2821 50  0000 C CNN
F 2 "" H 3600 2650 50  0001 C CNN
F 3 "" H 3600 2650 50  0001 C CNN
	1    3600 2650
	1    0    0    -1  
$EndComp
Text GLabel 3700 2650 1    50   Input ~ 0
A0
$Comp
L power:+3.3V #PWR09
U 1 1 5D55C861
P 5450 4450
F 0 "#PWR09" H 5450 4300 50  0001 C CNN
F 1 "+3.3V" H 5465 4621 50  0000 C CNN
F 2 "" H 5450 4450 50  0001 C CNN
F 3 "" H 5450 4450 50  0001 C CNN
	1    5450 4450
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR010
U 1 1 5D55CA50
P 5450 5250
F 0 "#PWR010" H 5450 5000 50  0001 C CNN
F 1 "GND" H 5455 5079 50  0000 C CNN
F 2 "" H 5450 5250 50  0001 C CNN
F 3 "" H 5450 5250 50  0001 C CNN
	1    5450 5250
	1    0    0    -1  
$EndComp
Text GLabel 4450 4850 0    50   Input ~ 0
SDA
Text GLabel 4450 4950 0    50   Input ~ 0
SCL
$Comp
L power:+3.3V #PWR07
U 1 1 5D55D359
P 5050 4650
F 0 "#PWR07" H 5050 4500 50  0001 C CNN
F 1 "+3.3V" V 5150 4600 50  0000 L CNN
F 2 "" H 5050 4650 50  0001 C CNN
F 3 "" H 5050 4650 50  0001 C CNN
	1    5050 4650
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4450 4850 4700 4850
Wire Wire Line
	5050 4950 4700 4950
NoConn ~ 5050 4750
$Comp
L power:+3.3V #PWR08
U 1 1 5D55E42B
P 5050 5050
F 0 "#PWR08" H 5050 4900 50  0001 C CNN
F 1 "+3.3V" V 4950 5000 50  0000 L CNN
F 2 "" H 5050 5050 50  0001 C CNN
F 3 "" H 5050 5050 50  0001 C CNN
	1    5050 5050
	0    -1   -1   0   
$EndComp
$Comp
L power:+3.3V #PWR01
U 1 1 5D55E774
P 2950 4750
F 0 "#PWR01" H 2950 4600 50  0001 C CNN
F 1 "+3.3V" H 2965 4921 50  0000 C CNN
F 2 "" H 2950 4750 50  0001 C CNN
F 3 "" H 2950 4750 50  0001 C CNN
	1    2950 4750
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR02
U 1 1 5D55EA34
P 2950 5050
F 0 "#PWR02" H 2950 4800 50  0001 C CNN
F 1 "GND" H 2955 4879 50  0000 C CNN
F 2 "" H 2950 5050 50  0001 C CNN
F 3 "" H 2950 5050 50  0001 C CNN
	1    2950 5050
	1    0    0    -1  
$EndComp
$Comp
L Device:C C1
U 1 1 5D55F20D
P 2950 4900
F 0 "C1" H 3065 4945 50  0000 L CNN
F 1 "100nF" H 3065 4856 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 2988 4750 50  0001 C CNN
F 3 "~" H 2950 4900 50  0001 C CNN
	1    2950 4900
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 5D55F5AF
P 4700 4700
F 0 "R1" H 4550 4750 50  0000 L CNN
F 1 "10 kΩ" H 4400 4650 50  0000 L CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 4630 4700 50  0001 C CNN
F 3 "~" H 4700 4700 50  0001 C CNN
	1    4700 4700
	1    0    0    -1  
$EndComp
Connection ~ 4700 4850
Wire Wire Line
	4700 4850 5050 4850
$Comp
L Device:R R2
U 1 1 5D55FE88
P 4700 5100
F 0 "R2" H 4550 5150 50  0000 L CNN
F 1 "10 kΩ" H 4400 5050 50  0000 L CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 4630 5100 50  0001 C CNN
F 3 "~" H 4700 5100 50  0001 C CNN
	1    4700 5100
	1    0    0    -1  
$EndComp
Connection ~ 4700 4950
Wire Wire Line
	4700 4950 4450 4950
$Comp
L power:+3.3V #PWR05
U 1 1 5D56030B
P 4700 4550
F 0 "#PWR05" H 4700 4400 50  0001 C CNN
F 1 "+3.3V" H 4715 4721 50  0000 C CNN
F 2 "" H 4700 4550 50  0001 C CNN
F 3 "" H 4700 4550 50  0001 C CNN
	1    4700 4550
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR06
U 1 1 5D5604F5
P 4700 5250
F 0 "#PWR06" H 4700 5100 50  0001 C CNN
F 1 "+3.3V" H 4715 5421 50  0000 C CNN
F 2 "" H 4700 5250 50  0001 C CNN
F 3 "" H 4700 5250 50  0001 C CNN
	1    4700 5250
	-1   0    0    1   
$EndComp
$Comp
L Sensor_Temperature:DS18B20 U2
U 1 1 5D568B46
P 5550 3300
F 0 "U2" H 5320 3345 50  0000 R CNN
F 1 "DS18B20" H 5320 3256 50  0000 R CNN
F 2 "TO_SOT_Packages_THT:TO-92_Horizontal2_Inline_Narrow_Oval" H 4550 3050 50  0001 C CNN
F 3 "http://datasheets.maximintegrated.com/en/ds/DS18B20.pdf" H 5400 3550 50  0001 C CNN
	1    5550 3300
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR011
U 1 1 5D56AB26
P 5550 3000
F 0 "#PWR011" H 5550 2850 50  0001 C CNN
F 1 "+3.3V" H 5565 3171 50  0000 C CNN
F 2 "" H 5550 3000 50  0001 C CNN
F 3 "" H 5550 3000 50  0001 C CNN
	1    5550 3000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR012
U 1 1 5D56AF60
P 5550 3600
F 0 "#PWR012" H 5550 3350 50  0001 C CNN
F 1 "GND" H 5555 3429 50  0000 C CNN
F 2 "" H 5550 3600 50  0001 C CNN
F 3 "" H 5550 3600 50  0001 C CNN
	1    5550 3600
	1    0    0    -1  
$EndComp
Text GLabel 6150 3300 2    50   Input ~ 0
DATA_TEMP
$Comp
L Device:R R3
U 1 1 5D56B6B6
P 6000 3150
F 0 "R3" H 6070 3195 50  0000 L CNN
F 1 "4.7 kΩ" H 6070 3106 50  0000 L CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 5930 3150 50  0001 C CNN
F 3 "~" H 6000 3150 50  0001 C CNN
	1    6000 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	5850 3300 6000 3300
Wire Wire Line
	6000 3300 6150 3300
Connection ~ 6000 3300
$Comp
L power:+3.3V #PWR013
U 1 1 5D56C33D
P 6000 3000
F 0 "#PWR013" H 6000 2850 50  0001 C CNN
F 1 "+3.3V" H 6015 3171 50  0000 C CNN
F 2 "" H 6000 3000 50  0001 C CNN
F 3 "" H 6000 3000 50  0001 C CNN
	1    6000 3000
	1    0    0    -1  
$EndComp
Text GLabel 7500 3550 3    50   Input ~ 0
A0
Text GLabel 8050 3550 3    50   Input ~ 0
A3
Wire Notes Line
	6750 2700 6750 4000
Wire Notes Line
	4300 4000 4300 3750
Text Notes 4650 2800 0    50   ~ 0
DS18B20\n
Wire Notes Line
	8800 2700 8800 4000
Wire Notes Line
	2700 2700 8800 2700
Text Notes 6800 2800 0    50   ~ 0
EC\n
Wire Notes Line
	2700 4350 4050 4350
Wire Notes Line
	4050 4350 4050 4000
Wire Notes Line
	4050 4000 8800 4000
Wire Notes Line
	6550 4000 6550 5500
Wire Notes Line
	6550 5500 2700 5500
Wire Notes Line
	2700 5500 2700 4350
Text Notes 4100 4100 0    50   ~ 0
MS5803\n
$Comp
L Project:MS5803-01BA U1
U 1 1 5D58BA7C
P 5450 4850
F 0 "U1" H 5780 4895 50  0000 L CNN
F 1 "MS5803-01BA" H 5780 4806 50  0000 L CNN
F 2 "Project:MS5803" H 5450 4850 50  0001 C CNN
F 3 "https://www.te.com/commerce/DocumentDelivery/DDEController?Action=showdoc&DocId=Data+SheetMS5803-01BAB3pdfEnglishENG_DS_MS5803-01BA_B3.pdfCAT-BLPS0038" H 5450 4850 50  0001 C CNN
	1    5450 4850
	1    0    0    -1  
$EndComp
$Comp
L Project:KeyStone_Screw J?
U 1 1 5D953E64
P 8050 3100
F 0 "J?" V 8238 3187 50  0000 L CNN
F 1 "KeyStone_Screw" V 8327 3187 50  0000 L CNN
F 2 "Project:KeyStone_Screw" H 8050 3100 50  0001 C CNN
F 3 "https://docs-apac.rs-online.com/webdocs/13ec/0900766b813ecaa4.pdf" H 8050 3100 50  0001 C CNN
	1    8050 3100
	0    1    1    0   
$EndComp
$Comp
L Project:KeyStone_Screw J?
U 1 1 5D9540DF
P 7500 3100
F 0 "J?" V 7688 3187 50  0000 L CNN
F 1 "KeyStone_Screw" V 7777 3187 50  0000 L CNN
F 2 "Project:KeyStone_Screw" H 7500 3100 50  0001 C CNN
F 3 "https://docs-apac.rs-online.com/webdocs/13ec/0900766b813ecaa4.pdf" H 7500 3100 50  0001 C CNN
	1    7500 3100
	0    1    1    0   
$EndComp
$EndSCHEMATC
