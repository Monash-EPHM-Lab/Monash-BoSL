EESchema Schematic File Version 4
EELAYER 29 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Velocity AM Demodulator"
Date "2019-11-28"
Rev "rev 0.1"
Comp "BoSL"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Amplifier_Operational:NE5532 U1
U 1 1 5DDEFB26
P 5900 4400
F 0 "U1" H 5900 4764 50  0000 C CNN
F 1 "NE5532" H 5900 4675 50  0000 C CNN
F 2 "Housings_DIP:DIP-8_W7.62mm_LongPads" H 5900 4400 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/ne5532.pdf" H 5900 4400 50  0001 C CNN
	1    5900 4400
	1    0    0    -1  
$EndComp
$Comp
L Amplifier_Operational:NE5532 U1
U 2 1 5DDF070F
P 5900 3300
F 0 "U1" H 5900 3664 50  0000 C CNN
F 1 "NE5532" H 5900 3575 50  0000 C CNN
F 2 "Housings_DIP:DIP-8_W7.62mm_LongPads" H 5900 3300 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/ne5532.pdf" H 5900 3300 50  0001 C CNN
	2    5900 3300
	1    0    0    -1  
$EndComp
$Comp
L Amplifier_Operational:NE5532 U1
U 3 1 5DDF51BB
P 7550 3300
F 0 "U1" H 7507 3345 50  0000 L CNN
F 1 "NE5532" H 7507 3256 50  0000 L CNN
F 2 "Housings_DIP:DIP-8_W7.62mm_LongPads" H 7550 3300 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/ne5532.pdf" H 7550 3300 50  0001 C CNN
	3    7550 3300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0101
U 1 1 5DDF84D4
P 7450 3600
F 0 "#PWR0101" H 7450 3350 50  0001 C CNN
F 1 "GND" H 7455 3429 50  0000 C CNN
F 2 "" H 7450 3600 50  0001 C CNN
F 3 "" H 7450 3600 50  0001 C CNN
	1    7450 3600
	1    0    0    -1  
$EndComp
$Comp
L power:VPP #PWR0102
U 1 1 5DDF88F6
P 7450 3000
F 0 "#PWR0102" H 7450 2850 50  0001 C CNN
F 1 "VPP" H 7465 3171 50  0000 C CNN
F 2 "" H 7450 3000 50  0001 C CNN
F 3 "" H 7450 3000 50  0001 C CNN
	1    7450 3000
	1    0    0    -1  
$EndComp
Wire Wire Line
	6200 3550 5600 3550
Wire Wire Line
	5600 3550 5600 3400
Wire Wire Line
	5600 3200 5200 3200
$Comp
L Device:R R2
U 1 1 5DDF9A83
P 5200 3050
F 0 "R2" H 5270 3095 50  0000 L CNN
F 1 "2 kΩ" H 5270 3006 50  0000 L CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 5130 3050 50  0001 C CNN
F 3 "~" H 5200 3050 50  0001 C CNN
	1    5200 3050
	1    0    0    -1  
$EndComp
$Comp
L Device:R R3
U 1 1 5DDF9DD3
P 5200 3350
F 0 "R3" H 5270 3395 50  0000 L CNN
F 1 "2 kΩ" H 5270 3306 50  0000 L CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 5130 3350 50  0001 C CNN
F 3 "~" H 5200 3350 50  0001 C CNN
	1    5200 3350
	1    0    0    -1  
$EndComp
Connection ~ 5200 3200
$Comp
L power:VPP #PWR0103
U 1 1 5DDFA90B
P 5200 2900
F 0 "#PWR0103" H 5200 2750 50  0001 C CNN
F 1 "VPP" H 5215 3071 50  0000 C CNN
F 2 "" H 5200 2900 50  0001 C CNN
F 3 "" H 5200 2900 50  0001 C CNN
	1    5200 2900
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0104
U 1 1 5DDFAF6C
P 5200 3500
F 0 "#PWR0104" H 5200 3250 50  0001 C CNN
F 1 "GND" H 5205 3329 50  0000 C CNN
F 2 "" H 5200 3500 50  0001 C CNN
F 3 "" H 5200 3500 50  0001 C CNN
	1    5200 3500
	1    0    0    -1  
$EndComp
Wire Wire Line
	6200 3300 6200 3550
Wire Wire Line
	6200 3300 6600 3300
Connection ~ 6200 3300
Text GLabel 6600 3300 2    50   Input ~ 0
Vref
$Comp
L Device:R R5
U 1 1 5DDFCA2B
P 5400 4050
F 0 "R5" H 5470 4095 50  0000 L CNN
F 1 "2 kΩ" H 5470 4006 50  0000 L CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 5330 4050 50  0001 C CNN
F 3 "~" H 5400 4050 50  0001 C CNN
	1    5400 4050
	1    0    0    -1  
$EndComp
Wire Wire Line
	5600 4300 5400 4300
Wire Wire Line
	5400 4300 5400 4200
Text GLabel 5400 3900 1    50   Input ~ 0
Vref
$Comp
L Device:R R1
U 1 1 5DDFD405
P 5100 4300
F 0 "R1" V 4896 4300 50  0000 C CNN
F 1 "10 kΩ" V 4985 4300 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 5030 4300 50  0001 C CNN
F 3 "~" H 5100 4300 50  0001 C CNN
	1    5100 4300
	0    1    1    0   
$EndComp
Wire Wire Line
	5400 4300 5250 4300
Connection ~ 5400 4300
Wire Wire Line
	4950 4300 4600 4300
$Comp
L Device:R R6
U 1 1 5DE03D24
P 5900 4800
F 0 "R6" V 5696 4800 50  0000 C CNN
F 1 "10 kΩ" V 5785 4800 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 5830 4800 50  0001 C CNN
F 3 "~" H 5900 4800 50  0001 C CNN
	1    5900 4800
	0    1    1    0   
$EndComp
$Comp
L Device:R R4
U 1 1 5DE040B5
P 5300 4500
F 0 "R4" V 5500 4500 50  0000 C CNN
F 1 "2 kΩ" V 5400 4500 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 5230 4500 50  0001 C CNN
F 3 "~" H 5300 4500 50  0001 C CNN
	1    5300 4500
	0    1    1    0   
$EndComp
Wire Wire Line
	6200 4400 6200 4800
Wire Wire Line
	6200 4800 6050 4800
Wire Wire Line
	5750 4800 5550 4800
Wire Wire Line
	5550 4800 5550 4500
Wire Wire Line
	5550 4500 5600 4500
Wire Wire Line
	5550 4500 5450 4500
Connection ~ 5550 4500
$Comp
L Device:Speaker_Ultrasound LS2
U 1 1 5DE06123
P 4400 4850
F 0 "LS2" H 4700 4800 50  0000 C CNN
F 1 "Receiver" H 4750 4700 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02_Pitch2.54mm" H 4365 4800 50  0001 C CNN
F 3 "~" H 4365 4800 50  0001 C CNN
	1    4400 4850
	-1   0    0    1   
$EndComp
Wire Wire Line
	5150 4500 5150 4750
$Comp
L power:VPP #PWR0105
U 1 1 5DE093E1
P 5050 4850
F 0 "#PWR0105" H 5050 4700 50  0001 C CNN
F 1 "VPP" V 5065 4977 50  0000 L CNN
F 2 "" H 5050 4850 50  0001 C CNN
F 3 "" H 5050 4850 50  0001 C CNN
	1    5050 4850
	0    1    1    0   
$EndComp
$Comp
L Device:C C1
U 1 1 5DE0A18B
P 4600 4150
F 0 "C1" H 4715 4195 50  0000 L CNN
F 1 "1 μF" H 4715 4106 50  0000 L CNN
F 2 "Capacitors_THT:CP_Radial_D5.0mm_P2.50mm" H 4638 4000 50  0001 C CNN
F 3 "~" H 4600 4150 50  0001 C CNN
	1    4600 4150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0106
U 1 1 5DE0B030
P 4600 4000
F 0 "#PWR0106" H 4600 3750 50  0001 C CNN
F 1 "GND" H 4605 3829 50  0000 C CNN
F 2 "" H 4600 4000 50  0001 C CNN
F 3 "" H 4600 4000 50  0001 C CNN
	1    4600 4000
	-1   0    0    1   
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J1
U 1 1 5DE0B133
P 3850 4300
F 0 "J1" V 3814 4213 50  0000 R CNN
F 1 "V Square" V 3950 4500 50  0000 R CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x01_Pitch2.54mm" H 3850 4300 50  0001 C CNN
F 3 "~" H 3850 4300 50  0001 C CNN
	1    3850 4300
	-1   0    0    1   
$EndComp
Wire Wire Line
	4600 4300 4300 4300
Connection ~ 4600 4300
$Comp
L Device:Speaker_Ultrasound LS1
U 1 1 5DE0BBA5
P 3900 3500
F 0 "LS1" H 4250 3450 50  0000 C CNN
F 1 "Emmiter" H 4250 3350 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02_Pitch2.54mm" H 3865 3450 50  0001 C CNN
F 3 "~" H 3865 3450 50  0001 C CNN
	1    3900 3500
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR0107
U 1 1 5DE0CB35
P 4550 3400
F 0 "#PWR0107" H 4550 3150 50  0001 C CNN
F 1 "GND" V 4555 3273 50  0000 R CNN
F 2 "" H 4550 3400 50  0001 C CNN
F 3 "" H 4550 3400 50  0001 C CNN
	1    4550 3400
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4100 3500 4300 3500
Connection ~ 4300 4300
Wire Wire Line
	4300 4300 4050 4300
$Comp
L Diode:1N4148 D1
U 1 1 5DE0EC7F
P 6650 4400
F 0 "D1" H 6650 4187 50  0000 C CNN
F 1 "1N4148" H 6650 4276 50  0000 C CNN
F 2 "Diodes_THT:D_A-405_P7.62mm_Horizontal" H 6650 4225 50  0001 C CNN
F 3 "https://assets.nexperia.com/documents/data-sheet/1N4148_1N4448.pdf" H 6650 4400 50  0001 C CNN
	1    6650 4400
	-1   0    0    1   
$EndComp
$Comp
L Device:R R7
U 1 1 5DE0F718
P 7000 4550
F 0 "R7" V 7200 4550 50  0000 C CNN
F 1 "2 kΩ" V 7100 4550 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 6930 4550 50  0001 C CNN
F 3 "~" H 7000 4550 50  0001 C CNN
	1    7000 4550
	1    0    0    -1  
$EndComp
Wire Wire Line
	6200 4400 6500 4400
Connection ~ 6200 4400
Wire Wire Line
	6800 4400 7000 4400
Text GLabel 7000 4850 0    50   Input ~ 0
Vref
Wire Wire Line
	7000 4700 7000 4850
$Comp
L Device:R R8
U 1 1 5DE114E0
P 7550 4400
F 0 "R8" V 7750 4400 50  0000 C CNN
F 1 "330 Ω" V 7650 4400 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 7480 4400 50  0001 C CNN
F 3 "~" H 7550 4400 50  0001 C CNN
	1    7550 4400
	0    1    1    0   
$EndComp
$Comp
L Device:C C2
U 1 1 5DE117BA
P 8050 4550
F 0 "C2" H 8165 4595 50  0000 L CNN
F 1 "1 μF" H 8165 4506 50  0000 L CNN
F 2 "Capacitors_THT:CP_Radial_D5.0mm_P2.50mm" H 8088 4400 50  0001 C CNN
F 3 "~" H 8050 4550 50  0001 C CNN
	1    8050 4550
	1    0    0    -1  
$EndComp
Text GLabel 8050 4850 3    50   Input ~ 0
Vref
Wire Wire Line
	8050 4850 8050 4700
Wire Wire Line
	8050 4400 7700 4400
Wire Wire Line
	7400 4400 7000 4400
Connection ~ 7000 4400
$Comp
L Connector_Generic:Conn_01x01 J6
U 1 1 5DE13BD9
P 8650 4400
F 0 "J6" V 8614 4313 50  0000 R CNN
F 1 "Doppler Out" V 8750 4600 50  0000 R CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x01_Pitch2.54mm" H 8650 4400 50  0001 C CNN
F 3 "~" H 8650 4400 50  0001 C CNN
	1    8650 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	8050 4400 8450 4400
Connection ~ 8050 4400
Wire Wire Line
	4100 3400 4550 3400
Wire Wire Line
	4300 3500 4300 4300
Wire Wire Line
	4600 4750 5150 4750
Wire Wire Line
	4600 4850 5050 4850
$Comp
L power:GND #PWR0108
U 1 1 5DE26C3A
P 8550 3900
F 0 "#PWR0108" H 8550 3650 50  0001 C CNN
F 1 "GND" V 8555 3773 50  0000 R CNN
F 2 "" H 8550 3900 50  0001 C CNN
F 3 "" H 8550 3900 50  0001 C CNN
	1    8550 3900
	0    1    1    0   
$EndComp
$Comp
L power:VPP #PWR0109
U 1 1 5DE27123
P 8550 4000
F 0 "#PWR0109" H 8550 3850 50  0001 C CNN
F 1 "VPP" V 8565 4126 50  0000 L CNN
F 2 "" H 8550 4000 50  0001 C CNN
F 3 "" H 8550 4000 50  0001 C CNN
	1    8550 4000
	0    -1   -1   0   
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J2
U 1 1 5DE2938D
P 8750 3900
F 0 "J2" H 8830 3892 50  0000 L CNN
F 1 "Conn_01x02" H 8830 3803 50  0000 L CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02_Pitch2.54mm" H 8750 3900 50  0001 C CNN
F 3 "~" H 8750 3900 50  0001 C CNN
	1    8750 3900
	1    0    0    -1  
$EndComp
$EndSCHEMATC
