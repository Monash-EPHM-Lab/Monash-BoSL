EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Heterodyne-rescue:VPP-power #PWR0101
U 1 1 5DE5A2B7
P 6050 2250
F 0 "#PWR0101" H 6050 2100 50  0001 C CNN
F 1 "VPP" V 6065 2377 50  0000 L CNN
F 2 "" H 6050 2250 50  0001 C CNN
F 3 "" H 6050 2250 50  0001 C CNN
	1    6050 2250
	0    1    1    0   
$EndComp
$Comp
L Heterodyne-rescue:GND-power #PWR0102
U 1 1 5DE5A5CA
P 4150 2550
F 0 "#PWR0102" H 4150 2300 50  0001 C CNN
F 1 "GND" V 4155 2423 50  0000 R CNN
F 2 "" H 4150 2550 50  0001 C CNN
F 3 "" H 4150 2550 50  0001 C CNN
	1    4150 2550
	0    1    1    0   
$EndComp
Wire Wire Line
	6050 2250 5850 2250
Wire Wire Line
	4750 2250 4750 2050
Wire Wire Line
	4700 2400 4750 2400
$Comp
L Heterodyne-rescue:C-Device C2
U 1 1 5DE5B5FE
P 4550 2400
F 0 "C2" V 4301 2400 50  0000 C CNN
F 1 "100 nF" V 4390 2400 50  0000 C CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 4588 2250 50  0001 C CNN
F 3 "~" H 4550 2400 50  0001 C CNN
	1    4550 2400
	0    1    1    0   
$EndComp
$Comp
L Heterodyne-rescue:Speaker_Ultrasound-Device LS2
U 1 1 5DE5E303
P 8500 1800
F 0 "LS2" H 8466 1386 50  0000 C CNN
F 1 "Emmiter" H 8466 1475 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02_Pitch2.54mm" H 8465 1750 50  0001 C CNN
F 3 "~" H 8465 1750 50  0001 C CNN
	1    8500 1800
	1    0    0    1   
$EndComp
$Comp
L Heterodyne-rescue:GND-power #PWR0103
U 1 1 5DE5E81E
P 8050 1700
F 0 "#PWR0103" H 8050 1450 50  0001 C CNN
F 1 "GND" V 8055 1573 50  0000 R CNN
F 2 "" H 8050 1700 50  0001 C CNN
F 3 "" H 8050 1700 50  0001 C CNN
	1    8050 1700
	0    1    -1   0   
$EndComp
Wire Wire Line
	8300 1700 8050 1700
Wire Wire Line
	8300 1800 8100 1800
Wire Wire Line
	8100 1800 8100 2550
$Comp
L Heterodyne-rescue:Speaker_Ultrasound-Device LS1
U 1 1 5DE60CCA
P 3550 2150
F 0 "LS1" H 3516 1736 50  0000 C CNN
F 1 "Receiver" H 3516 1825 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02_Pitch2.54mm" H 3515 2100 50  0001 C CNN
F 3 "~" H 3515 2100 50  0001 C CNN
	1    3550 2150
	-1   0    0    1   
$EndComp
$Comp
L Heterodyne-rescue:C-Device C1
U 1 1 5DE6141E
P 4450 2050
F 0 "C1" V 4201 2050 50  0000 C CNN
F 1 "100 nF" V 4290 2050 50  0000 C CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 4488 1900 50  0001 C CNN
F 3 "~" H 4450 2050 50  0001 C CNN
	1    4450 2050
	0    1    1    0   
$EndComp
Wire Wire Line
	4750 2050 4600 2050
Wire Wire Line
	6150 3250 6150 3550
NoConn ~ 5850 2400
Wire Wire Line
	8100 2550 8300 2550
$Comp
L Heterodyne-rescue:GND-power #PWR0106
U 1 1 5DE74913
P 6450 4050
F 0 "#PWR0106" H 6450 3800 50  0001 C CNN
F 1 "GND" H 6455 3879 50  0000 C CNN
F 2 "" H 6450 4050 50  0001 C CNN
F 3 "" H 6450 4050 50  0001 C CNN
	1    6450 4050
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6150 3850 6150 4050
$Comp
L Heterodyne-rescue:R-Device R2
U 1 1 5DE786A6
P 7600 2550
F 0 "R2" V 7396 2550 50  0000 C CNN
F 1 "3.3 kΩ" V 7485 2550 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 7530 2550 50  0001 C CNN
F 3 "~" H 7600 2550 50  0001 C CNN
	1    7600 2550
	0    1    1    0   
$EndComp
Connection ~ 8100 2550
$Comp
L Heterodyne-rescue:Conn_01x02-Connector_Generic J3
U 1 1 5DE85660
P 7350 1650
F 0 "J3" H 7430 1642 50  0000 L CNN
F 1 "PWR" H 7430 1553 50  0000 L CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02_Pitch2.54mm" H 7350 1650 50  0001 C CNN
F 3 "~" H 7350 1650 50  0001 C CNN
	1    7350 1650
	1    0    0    -1  
$EndComp
$Comp
L Heterodyne-rescue:GND-power #PWR0108
U 1 1 5DE85F8F
P 6900 1450
F 0 "#PWR0108" H 6900 1200 50  0001 C CNN
F 1 "GND" V 6905 1323 50  0000 R CNN
F 2 "" H 6900 1450 50  0001 C CNN
F 3 "" H 6900 1450 50  0001 C CNN
	1    6900 1450
	-1   0    0    1   
$EndComp
$Comp
L Heterodyne-rescue:VPP-power #PWR0109
U 1 1 5DE8643B
P 6900 2000
F 0 "#PWR0109" H 6900 1850 50  0001 C CNN
F 1 "VPP" V 6915 2126 50  0000 L CNN
F 2 "" H 6900 2000 50  0001 C CNN
F 3 "" H 6900 2000 50  0001 C CNN
	1    6900 2000
	-1   0    0    1   
$EndComp
Wire Wire Line
	4150 2550 4750 2550
Wire Wire Line
	3750 2050 4300 2050
Wire Wire Line
	4000 2400 4000 2150
Wire Wire Line
	4000 2150 3750 2150
Wire Wire Line
	4000 2400 4400 2400
$Comp
L Heterodyne-rescue:Conn_01x02-Connector_Generic J5
U 1 1 5DE97BF0
P 8600 2550
F 0 "J5" H 8680 2542 50  0000 L CNN
F 1 "V_Square" H 8680 2453 50  0000 L CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02_Pitch2.54mm" H 8600 2550 50  0001 C CNN
F 3 "~" H 8600 2550 50  0001 C CNN
	1    8600 2550
	1    0    0    -1  
$EndComp
$Comp
L Heterodyne-rescue:Conn_01x02-Connector_Generic J4
U 1 1 5DE98025
P 6000 4800
F 0 "J4" H 6080 4792 50  0000 L CNN
F 1 "Doppler_Out" H 6080 4703 50  0000 L CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02_Pitch2.54mm" H 6000 4800 50  0001 C CNN
F 3 "~" H 6000 4800 50  0001 C CNN
	1    6000 4800
	0    1    1    0   
$EndComp
$Comp
L Heterodyne-rescue:GND-power #PWR0104
U 1 1 5DE995B1
P 6000 4600
F 0 "#PWR0104" H 6000 4350 50  0001 C CNN
F 1 "GND" V 6005 4473 50  0000 R CNN
F 2 "" H 6000 4600 50  0001 C CNN
F 3 "" H 6000 4600 50  0001 C CNN
	1    6000 4600
	-1   0    0    1   
$EndComp
$Comp
L Heterodyne-rescue:GND-power #PWR0110
U 1 1 5DE99816
P 8400 2650
F 0 "#PWR0110" H 8400 2400 50  0001 C CNN
F 1 "GND" V 8405 2523 50  0000 R CNN
F 2 "" H 8400 2650 50  0001 C CNN
F 3 "" H 8400 2650 50  0001 C CNN
	1    8400 2650
	0    1    1    0   
$EndComp
$Comp
L Heterodyne-rescue:NE5532-Amplifier_Operational U2
U 3 1 5DE9ED64
P 8450 3600
F 0 "U2" H 8263 3555 50  0000 R CNN
F 1 "NE5532" H 8263 3644 50  0000 R CNN
F 2 "Housings_DIP:DIP-8_W7.62mm_LongPads" H 8450 3600 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/ne5532.pdf" H 8450 3600 50  0001 C CNN
	3    8450 3600
	1    0    0    -1  
$EndComp
$Comp
L Heterodyne-rescue:Jumper_NC_Dual-Device JP1
U 1 1 5DEA6580
P 5900 3250
F 0 "JP1" H 5900 3485 50  0000 C CNN
F 1 "Jumper_NC_Dual" H 5900 3396 50  0000 C CNN
F 2 "Connectors:GS3" H 5900 3250 50  0001 C CNN
F 3 "~" H 5900 3250 50  0001 C CNN
	1    5900 3250
	1    0    0    1   
$EndComp
Wire Wire Line
	5900 2700 5850 2700
$Comp
L Heterodyne-rescue:Jumper_NC_Dual-Device JP2
U 1 1 5DEA2822
P 5900 4050
F 0 "JP2" H 5900 4285 50  0000 C CNN
F 1 "Jumper_NC_Dual" H 5900 4196 50  0000 C CNN
F 2 "Connectors:GS3" H 5900 4050 50  0001 C CNN
F 3 "~" H 5900 4050 50  0001 C CNN
	1    5900 4050
	-1   0    0    -1  
$EndComp
$Comp
L Heterodyne-rescue:NE5532-Amplifier_Operational U2
U 1 1 5DEBD03F
P 2150 3850
F 0 "U2" H 2150 4150 50  0000 C CNN
F 1 "NE5532" H 2150 4250 50  0000 C CNN
F 2 "Housings_DIP:DIP-8_W7.62mm_LongPads" H 2150 3850 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/ne5532.pdf" H 2150 3850 50  0001 C CNN
	1    2150 3850
	0    -1   1    0   
$EndComp
Connection ~ 6150 4050
Wire Wire Line
	5900 4150 5900 4600
Wire Wire Line
	8300 2350 8300 2550
Connection ~ 8300 2550
Wire Wire Line
	8300 2550 8400 2550
$Comp
L Heterodyne-rescue:MAX7375AXR105-Oscillator U3
U 1 1 5DEE484A
P 9400 2350
F 0 "U3" H 9170 2305 50  0000 R CNN
F 1 "MAX7375AXR105" H 9170 2394 50  0000 R CNN
F 2 "TO_SOT_Packages_SMD:SOT-323_SC-70_Handsoldering" H 10500 2000 50  0001 C CNN
F 3 "https://datasheets.maximintegrated.com/en/ds/MAX7375.pdf" H 9300 2350 50  0001 C CNN
	1    9400 2350
	-1   0    0    1   
$EndComp
Wire Wire Line
	8300 2350 9000 2350
$Comp
L Heterodyne-rescue:VPP-power #PWR0105
U 1 1 5DEE79E3
P 9400 2900
F 0 "#PWR0105" H 9400 2750 50  0001 C CNN
F 1 "VPP" H 9415 3071 50  0000 C CNN
F 2 "" H 9400 2900 50  0001 C CNN
F 3 "" H 9400 2900 50  0001 C CNN
	1    9400 2900
	-1   0    0    1   
$EndComp
$Comp
L Heterodyne-rescue:GND-power #PWR0107
U 1 1 5DEE82A6
P 9400 2050
F 0 "#PWR0107" H 9400 1800 50  0001 C CNN
F 1 "GND" H 9405 1879 50  0000 C CNN
F 2 "" H 9400 2050 50  0001 C CNN
F 3 "" H 9400 2050 50  0001 C CNN
	1    9400 2050
	-1   0    0    1   
$EndComp
Wire Wire Line
	5850 2550 6300 2550
$Comp
L Heterodyne-rescue:C-Device C12
U 1 1 5DEEB130
P 7300 2700
F 0 "C12" H 7415 2745 50  0000 L CNN
F 1 "330 pF" H 7415 2656 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 7338 2550 50  0001 C CNN
F 3 "~" H 7300 2700 50  0001 C CNN
	1    7300 2700
	1    0    0    -1  
$EndComp
$Comp
L Heterodyne-rescue:C-Device C9
U 1 1 5DEEB73A
P 6450 2550
F 0 "C9" V 6201 2550 50  0000 C CNN
F 1 "330 pF" V 6290 2550 50  0000 C CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 6488 2400 50  0001 C CNN
F 3 "~" H 6450 2550 50  0001 C CNN
	1    6450 2550
	0    1    1    0   
$EndComp
$Comp
L Heterodyne-rescue:C-Device C10
U 1 1 5DEEBC96
P 6700 2700
F 0 "C10" H 6585 2655 50  0000 R CNN
F 1 "330 pF" H 6585 2744 50  0000 R CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 6738 2550 50  0001 C CNN
F 3 "~" H 6700 2700 50  0001 C CNN
	1    6700 2700
	-1   0    0    1   
$EndComp
$Comp
L Heterodyne-rescue:R-Device R10
U 1 1 5DEEC06C
P 6950 2550
F 0 "R10" V 6746 2550 50  0000 C CNN
F 1 "1 kΩ" V 6835 2550 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 6880 2550 50  0001 C CNN
F 3 "~" H 6950 2550 50  0001 C CNN
	1    6950 2550
	0    1    1    0   
$EndComp
$Comp
L Heterodyne-rescue:GND-power #PWR0113
U 1 1 5DEECFCF
P 7300 2850
F 0 "#PWR0113" H 7300 2600 50  0001 C CNN
F 1 "GND" H 7305 2679 50  0000 C CNN
F 2 "" H 7300 2850 50  0001 C CNN
F 3 "" H 7300 2850 50  0001 C CNN
	1    7300 2850
	1    0    0    -1  
$EndComp
$Comp
L Heterodyne-rescue:GND-power #PWR0114
U 1 1 5DEEDBB9
P 6700 2850
F 0 "#PWR0114" H 6700 2600 50  0001 C CNN
F 1 "GND" H 6705 2679 50  0000 C CNN
F 2 "" H 6700 2850 50  0001 C CNN
F 3 "" H 6700 2850 50  0001 C CNN
	1    6700 2850
	1    0    0    -1  
$EndComp
Wire Wire Line
	6600 2550 6700 2550
Wire Wire Line
	6700 2550 6800 2550
Connection ~ 6700 2550
Wire Wire Line
	7100 2550 7300 2550
Wire Wire Line
	7300 2550 7450 2550
Connection ~ 7300 2550
Wire Wire Line
	7750 2550 8100 2550
$Comp
L Heterodyne-rescue:Conn_01x01-Connector_Generic J7
U 1 1 5DEEDE00
P 3600 2300
F 0 "J7" H 3519 2079 50  0000 C CNN
F 1 "Reciever" H 3700 2400 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x01_Pitch2.54mm" H 3600 2300 50  0001 C CNN
F 3 "~" H 3600 2300 50  0001 C CNN
	1    3600 2300
	-1   0    0    1   
$EndComp
$Comp
L Heterodyne-rescue:GND-power #PWR0115
U 1 1 5DEEE7E4
P 3800 2300
F 0 "#PWR0115" H 3800 2050 50  0001 C CNN
F 1 "GND" V 3805 2173 50  0000 R CNN
F 2 "" H 3800 2300 50  0001 C CNN
F 3 "" H 3800 2300 50  0001 C CNN
	1    3800 2300
	0    -1   -1   0   
$EndComp
$Comp
L Heterodyne-rescue:Conn_01x01-Connector_Generic J8
U 1 1 5DEEE964
P 8450 1950
F 0 "J8" H 8369 1729 50  0000 C CNN
F 1 "Emmiter" H 8550 2050 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x01_Pitch2.54mm" H 8450 1950 50  0001 C CNN
F 3 "~" H 8450 1950 50  0001 C CNN
	1    8450 1950
	1    0    0    -1  
$EndComp
$Comp
L Heterodyne-rescue:GND-power #PWR0116
U 1 1 5DEEF658
P 8250 1950
F 0 "#PWR0116" H 8250 1700 50  0001 C CNN
F 1 "GND" V 8255 1823 50  0000 R CNN
F 2 "" H 8250 1950 50  0001 C CNN
F 3 "" H 8250 1950 50  0001 C CNN
	1    8250 1950
	0    1    1    0   
$EndComp
$Comp
L Heterodyne-rescue:C-Device C11
U 1 1 5DEF08D7
P 6900 1700
F 0 "C11" H 6600 1750 50  0000 L CNN
F 1 "330 μF" H 6500 1650 50  0000 L CNN
F 2 "Capacitors_SMD:CP_Elec_8x10" H 6938 1550 50  0001 C CNN
F 3 "~" H 6900 1700 50  0001 C CNN
	1    6900 1700
	-1   0    0    1   
$EndComp
Wire Wire Line
	7150 1750 7150 1850
Wire Wire Line
	7150 1850 6900 1850
Wire Wire Line
	6900 2000 6900 1850
Wire Wire Line
	7150 1650 7150 1550
Wire Wire Line
	7150 1550 6900 1550
Wire Wire Line
	6900 1550 6900 1450
$Comp
L Heterodyne-rescue:C-Device C13
U 1 1 5DEF7CB3
P 9550 2800
F 0 "C13" V 9301 2800 50  0000 C CNN
F 1 "1 μF" V 9390 2800 50  0000 C CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 9588 2650 50  0001 C CNN
F 3 "~" H 9550 2800 50  0001 C CNN
	1    9550 2800
	0    1    1    0   
$EndComp
$Comp
L Heterodyne-rescue:GND-power #PWR0119
U 1 1 5DEF8673
P 9700 2800
F 0 "#PWR0119" H 9700 2550 50  0001 C CNN
F 1 "GND" V 9705 2673 50  0000 R CNN
F 2 "" H 9700 2800 50  0001 C CNN
F 3 "" H 9700 2800 50  0001 C CNN
	1    9700 2800
	0    -1   -1   0   
$EndComp
Wire Wire Line
	9400 2650 9400 2800
Wire Wire Line
	9400 2800 9400 2900
Connection ~ 9400 2800
$Comp
L Project:SA612AD U1
U 1 1 5DE59955
P 5300 2500
F 0 "U1" H 5300 3011 50  0000 C CNN
F 1 "SA612AD" H 5300 2922 50  0000 C CNN
F 2 "Housings_SOIC:SO-8_5.3x6.2mm_Pitch1.27mm" H 5300 2500 50  0001 C CNN
F 3 "" H 5300 2500 50  0001 C CNN
	1    5300 2500
	1    0    0    -1  
$EndComp
$Comp
L Heterodyne-rescue:C-Device C8
U 1 1 5DECBB95
P 6300 4050
F 0 "C8" H 6500 4050 50  0000 R CNN
F 1 "100 nF" H 6600 3950 50  0000 R CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 6338 3900 50  0001 C CNN
F 3 "~" H 6300 4050 50  0001 C CNN
	1    6300 4050
	0    -1   1    0   
$EndComp
$Comp
L Heterodyne-rescue:R-Device R11
U 1 1 5DECB81B
P 6150 3700
F 0 "R11" V 5946 3700 50  0000 C CNN
F 1 "330 Ω" V 6035 3700 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 6080 3700 50  0001 C CNN
F 3 "~" H 6150 3700 50  0001 C CNN
	1    6150 3700
	-1   0    0    1   
$EndComp
$Comp
L Heterodyne-rescue:NE5532-Amplifier_Operational U2
U 2 1 5DE9A181
P 4950 4800
F 0 "U2" H 4950 5100 50  0000 C CNN
F 1 "NE5532" H 4950 5200 50  0000 C CNN
F 2 "Housings_DIP:DIP-8_W7.62mm_LongPads" H 4950 4800 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/ne5532.pdf" H 4950 4800 50  0001 C CNN
	2    4950 4800
	0    -1   1    0   
$EndComp
Wire Wire Line
	5650 5100 5650 4050
$Comp
L Heterodyne-rescue:GND-power #PWR0120
U 1 1 5DFB3A2A
P 2250 4350
F 0 "#PWR0120" H 2250 4100 50  0001 C CNN
F 1 "GND" V 2255 4223 50  0000 R CNN
F 2 "" H 2250 4350 50  0001 C CNN
F 3 "" H 2250 4350 50  0001 C CNN
	1    2250 4350
	-1   0    0    1   
$EndComp
$Comp
L Heterodyne-rescue:Conn_01x02-Connector_Generic J6
U 1 1 5DFB3A34
P 2250 4550
F 0 "J6" H 2330 4542 50  0000 L CNN
F 1 "OP_OUT" H 2330 4453 50  0000 L CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02_Pitch2.54mm" H 2250 4550 50  0001 C CNN
F 3 "~" H 2250 4550 50  0001 C CNN
	1    2250 4550
	0    1    1    0   
$EndComp
Wire Wire Line
	2250 3550 2350 3550
Connection ~ 2350 3550
Wire Wire Line
	2150 4150 2150 4350
Wire Wire Line
	5900 2700 5900 2900
Wire Wire Line
	4750 2850 5750 2850
Wire Wire Line
	5750 2850 5750 3150
Wire Wire Line
	5750 3150 5900 3150
Wire Wire Line
	4750 2700 4750 2850
$Comp
L Heterodyne-rescue:C-Device C14
U 1 1 5E02AB84
P 8050 3600
F 0 "C14" V 7801 3600 50  0000 C CNN
F 1 "1 μF" V 7890 3600 50  0000 C CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 8088 3450 50  0001 C CNN
F 3 "~" H 8050 3600 50  0001 C CNN
	1    8050 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	8050 3300 8050 3450
Wire Wire Line
	8050 3750 8050 3900
Connection ~ 6900 1850
Connection ~ 6900 1550
Wire Wire Line
	8050 3300 8350 3300
Wire Wire Line
	8050 3900 8350 3900
$Comp
L Heterodyne-rescue:VPP-power #PWR0111
U 1 1 5E281BA1
P 8350 3300
F 0 "#PWR0111" H 8350 3150 50  0001 C CNN
F 1 "VPP" H 8365 3471 50  0000 C CNN
F 2 "" H 8350 3300 50  0001 C CNN
F 3 "" H 8350 3300 50  0001 C CNN
	1    8350 3300
	1    0    0    -1  
$EndComp
Connection ~ 8350 3300
$Comp
L Heterodyne-rescue:GND-power #PWR0112
U 1 1 5E282178
P 8350 3900
F 0 "#PWR0112" H 8350 3650 50  0001 C CNN
F 1 "GND" H 8355 3729 50  0000 C CNN
F 2 "" H 8350 3900 50  0001 C CNN
F 3 "" H 8350 3900 50  0001 C CNN
	1    8350 3900
	1    0    0    -1  
$EndComp
Connection ~ 8350 3900
Wire Wire Line
	2050 3050 2050 3550
$Comp
L Heterodyne-rescue:R-Device R4
U 1 1 5E282544
P 2650 3850
F 0 "R4" V 2854 3850 50  0000 C CNN
F 1 "39 kΩ" V 2765 3850 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 2580 3850 50  0001 C CNN
F 3 "~" H 2650 3850 50  0001 C CNN
	1    2650 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	2150 4150 2650 4150
Wire Wire Line
	2650 4150 2650 4000
Connection ~ 2150 4150
Wire Wire Line
	2650 3700 2650 3550
Wire Wire Line
	2350 3550 2650 3550
$Comp
L Heterodyne-rescue:R-Device R1
U 1 1 5E285F20
P 2350 2800
F 0 "R1" V 2554 2800 50  0000 C CNN
F 1 "1 kΩ" V 2465 2800 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 2280 2800 50  0001 C CNN
F 3 "~" H 2350 2800 50  0001 C CNN
	1    2350 2800
	1    0    0    -1  
$EndComp
$Comp
L Heterodyne-rescue:R-Device R3
U 1 1 5E286294
P 2600 3050
F 0 "R3" V 2804 3050 50  0000 C CNN
F 1 "1 kΩ" V 2715 3050 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 2530 3050 50  0001 C CNN
F 3 "~" H 2600 3050 50  0001 C CNN
	1    2600 3050
	0    1    1    0   
$EndComp
Wire Wire Line
	2350 2950 2350 3050
Wire Wire Line
	2450 3050 2350 3050
Connection ~ 2350 3050
Wire Wire Line
	2350 3050 2350 3550
$Comp
L Heterodyne-rescue:VPP-power #PWR0117
U 1 1 5E289D11
P 2350 2650
F 0 "#PWR0117" H 2350 2500 50  0001 C CNN
F 1 "VPP" H 2365 2821 50  0000 C CNN
F 2 "" H 2350 2650 50  0001 C CNN
F 3 "" H 2350 2650 50  0001 C CNN
	1    2350 2650
	1    0    0    -1  
$EndComp
$Comp
L Heterodyne-rescue:GND-power #PWR0118
U 1 1 5E28A11D
P 2750 3050
F 0 "#PWR0118" H 2750 2800 50  0001 C CNN
F 1 "GND" H 2755 2879 50  0000 C CNN
F 2 "" H 2750 3050 50  0001 C CNN
F 3 "" H 2750 3050 50  0001 C CNN
	1    2750 3050
	0    -1   -1   0   
$EndComp
Text GLabel 6700 2550 1    50   Input ~ 0
Driver_Wave
Text GLabel 2050 3050 1    50   Input ~ 0
Driver_Wave
Text Notes 1000 800  0    50   ~ 0
Consider Changing op amp to TL074
Wire Wire Line
	4950 5100 5300 5100
$Comp
L Device:R R?
U 1 1 5E37EFD3
P 5300 4750
F 0 "R?" H 5370 4796 50  0000 L CNN
F 1 "18 kΩ" H 5370 4705 50  0000 L CNN
F 2 "" V 5230 4750 50  0001 C CNN
F 3 "~" H 5300 4750 50  0001 C CNN
	1    5300 4750
	1    0    0    -1  
$EndComp
Wire Wire Line
	5300 4900 5300 5100
Connection ~ 5300 5100
Wire Wire Line
	5300 5100 5650 5100
Wire Wire Line
	5300 4600 5300 4400
Wire Wire Line
	5300 4400 5050 4400
Wire Wire Line
	5050 4400 5050 4500
$Comp
L Device:R R?
U 1 1 5E3820B6
P 4700 4250
F 0 "R?" V 4493 4250 50  0000 C CNN
F 1 "18 kΩ" V 4584 4250 50  0000 C CNN
F 2 "" V 4630 4250 50  0001 C CNN
F 3 "~" H 4700 4250 50  0001 C CNN
	1    4700 4250
	0    1    1    0   
$EndComp
$Comp
L Heterodyne-rescue:VPP-power #PWR?
U 1 1 5E3824B9
P 4550 4250
F 0 "#PWR?" H 4550 4100 50  0001 C CNN
F 1 "VPP" V 4565 4377 50  0000 L CNN
F 2 "" H 4550 4250 50  0001 C CNN
F 3 "" H 4550 4250 50  0001 C CNN
	1    4550 4250
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R?
U 1 1 5E382D5B
P 4850 3800
F 0 "R?" H 5100 3750 50  0000 R CNN
F 1 "1 kΩ" H 5100 3850 50  0000 R CNN
F 2 "" V 4780 3800 50  0001 C CNN
F 3 "~" H 4850 3800 50  0001 C CNN
	1    4850 3800
	-1   0    0    1   
$EndComp
$Comp
L Device:R R?
U 1 1 5E383109
P 5050 3800
F 0 "R?" H 4980 3754 50  0000 R CNN
F 1 "1 kΩ" H 4980 3845 50  0000 R CNN
F 2 "" V 4980 3800 50  0001 C CNN
F 3 "~" H 5050 3800 50  0001 C CNN
	1    5050 3800
	-1   0    0    1   
$EndComp
Wire Wire Line
	5050 3950 5050 4400
Connection ~ 5050 4400
Wire Wire Line
	4850 4500 4850 4250
Wire Wire Line
	4850 3950 4850 4250
Connection ~ 4850 4250
$Comp
L Device:C C?
U 1 1 5E387E65
P 4950 3500
F 0 "C?" V 4698 3500 50  0000 C CNN
F 1 "C" V 4789 3500 50  0000 C CNN
F 2 "" H 4988 3350 50  0001 C CNN
F 3 "~" H 4950 3500 50  0001 C CNN
	1    4950 3500
	0    1    1    0   
$EndComp
Wire Wire Line
	5050 3650 5100 3650
Wire Wire Line
	5100 3650 5100 3500
Wire Wire Line
	4850 3650 4800 3650
Wire Wire Line
	4800 3650 4800 3500
$Comp
L Device:R R?
U 1 1 5E38B726
P 5250 3250
F 0 "R?" V 5457 3250 50  0000 C CNN
F 1 "330 Ω" V 5366 3250 50  0000 C CNN
F 2 "" V 5180 3250 50  0001 C CNN
F 3 "~" H 5250 3250 50  0001 C CNN
	1    5250 3250
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R?
U 1 1 5E38C4E7
P 4800 3100
F 0 "R?" H 5050 3050 50  0000 R CNN
F 1 "330 Ω" H 5050 3150 50  0000 R CNN
F 2 "" V 4730 3100 50  0001 C CNN
F 3 "~" H 4800 3100 50  0001 C CNN
	1    4800 3100
	-1   0    0    1   
$EndComp
Wire Wire Line
	4800 2900 4800 2950
Wire Wire Line
	4800 2900 5900 2900
Wire Wire Line
	4800 3250 4800 3500
Connection ~ 4800 3500
Wire Wire Line
	5100 3500 5100 3250
Connection ~ 5100 3500
Wire Wire Line
	5400 3250 5650 3250
$EndSCHEMATC
