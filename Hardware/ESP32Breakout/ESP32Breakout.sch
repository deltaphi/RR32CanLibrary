EESchema Schematic File Version 4
EELAYER 29 0
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
L Mechanical:MountingHole_Pad H6
U 1 1 5C67C6B3
P 10650 5750
F 0 "H6" H 10750 5796 50  0000 L CNN
F 1 "PCB Screw" H 10750 5705 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.5mm_Pad_Via" H 10650 5750 50  0001 C CNN
F 3 "~" H 10650 5750 50  0001 C CNN
	1    10650 5750
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad H5
U 1 1 5C67C6E9
P 10650 5500
F 0 "H5" H 10750 5546 50  0000 L CNN
F 1 "PCB Screw" H 10750 5455 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.5mm_Pad_Via" H 10650 5500 50  0001 C CNN
F 3 "~" H 10650 5500 50  0001 C CNN
	1    10650 5500
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad H4
U 1 1 5C67C721
P 10650 5250
F 0 "H4" H 10750 5296 50  0000 L CNN
F 1 "PCB Screw" H 10750 5205 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.5mm_Pad_Via" H 10650 5250 50  0001 C CNN
F 3 "~" H 10650 5250 50  0001 C CNN
	1    10650 5250
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad H3
U 1 1 5C67C753
P 10650 5000
F 0 "H3" H 10750 5046 50  0000 L CNN
F 1 "PCB Screw" H 10750 4955 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.5mm_Pad_Via" H 10650 5000 50  0001 C CNN
F 3 "~" H 10650 5000 50  0001 C CNN
	1    10650 5000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0101
U 1 1 5C688870
P 10300 6300
F 0 "#PWR0101" H 10300 6050 50  0001 C CNN
F 1 "GND" H 10305 6127 50  0000 C CNN
F 2 "" H 10300 6300 50  0001 C CNN
F 3 "" H 10300 6300 50  0001 C CNN
	1    10300 6300
	1    0    0    -1  
$EndComp
Wire Wire Line
	10650 5100 10500 5100
Wire Wire Line
	10500 5100 10500 5350
Wire Wire Line
	10500 6300 10300 6300
Wire Wire Line
	10650 5850 10500 5850
Connection ~ 10500 5850
Wire Wire Line
	10650 5600 10500 5600
Connection ~ 10500 5600
Wire Wire Line
	10500 5600 10500 5850
Wire Wire Line
	10650 5350 10500 5350
Connection ~ 10500 5350
Wire Wire Line
	10500 5350 10500 5600
$Comp
L Connector:8P8C J1
U 1 1 5C688D10
P 7350 5800
F 0 "J1" H 7405 6467 50  0000 C CNN
F 1 "8P8C" H 7405 6376 50  0000 C CNN
F 2 "Connector_RJ:RJ45_Amphenol_54602-x08_Horizontal" V 7350 5825 50  0001 C CNN
F 3 "~" V 7350 5825 50  0001 C CNN
	1    7350 5800
	1    0    0    -1  
$EndComp
$Comp
L Connector:USB_B_Micro J3
U 1 1 5C694D46
P 800 5900
F 0 "J3" H 855 6367 50  0000 C CNN
F 1 "USB_B_Micro" H 855 6276 50  0000 C CNN
F 2 "Connector_USB:USB_Micro-B_Molex_47346-0001" H 950 5850 50  0001 C CNN
F 3 "~" H 950 5850 50  0001 C CNN
	1    800  5900
	1    0    0    -1  
$EndComp
$Comp
L Interface_CAN_LIN:TJA1051T U2
U 1 1 5C6951BB
P 9050 5300
F 0 "U2" H 8750 5650 50  0000 C CNN
F 1 "TJA1051T" H 9300 5650 50  0000 C CNN
F 2 "Package_SO:SOIC-8_3.9x4.9mm_P1.27mm" H 9050 4800 50  0001 C CIN
F 3 "http://www.nxp.com/documents/data_sheet/TJA1051.pdf" H 9050 5300 50  0001 C CNN
	1    9050 5300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0102
U 1 1 5C6959A4
P 2200 3600
F 0 "#PWR0102" H 2200 3350 50  0001 C CNN
F 1 "GND" H 2205 3427 50  0000 C CNN
F 2 "" H 2200 3600 50  0001 C CNN
F 3 "" H 2200 3600 50  0001 C CNN
	1    2200 3600
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR0104
U 1 1 5C697535
P 2200 700
F 0 "#PWR0104" H 2200 550 50  0001 C CNN
F 1 "VCC" H 2217 873 50  0000 C CNN
F 2 "" H 2200 700 50  0001 C CNN
F 3 "" H 2200 700 50  0001 C CNN
	1    2200 700 
	1    0    0    -1  
$EndComp
Wire Wire Line
	2200 700  2200 800 
$Comp
L power:VCC #PWR0117
U 1 1 5C737D53
P 1050 4350
F 0 "#PWR0117" H 1050 4200 50  0001 C CNN
F 1 "VCC" H 1067 4523 50  0000 C CNN
F 2 "" H 1050 4350 50  0001 C CNN
F 3 "" H 1050 4350 50  0001 C CNN
	1    1050 4350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0118
U 1 1 5C74DC98
P 800 6300
F 0 "#PWR0118" H 800 6050 50  0001 C CNN
F 1 "GND" H 805 6127 50  0000 C CNN
F 2 "" H 800 6300 50  0001 C CNN
F 3 "" H 800 6300 50  0001 C CNN
	1    800  6300
	1    0    0    -1  
$EndComp
Wire Wire Line
	700  6300 800  6300
Wire Wire Line
	3500 1400 2800 1400
Wire Wire Line
	3550 1500 2800 1500
Wire Wire Line
	7750 6100 8150 6100
Wire Wire Line
	7750 6000 8150 6000
Wire Wire Line
	7750 5600 7750 5700
Wire Wire Line
	7750 5700 7800 5700
Wire Wire Line
	7800 5700 7800 5800
Connection ~ 7750 5700
Connection ~ 800  6300
Text Label 1150 4700 0    50   ~ 0
I2C_SCL
Text Label 1150 4550 0    50   ~ 0
I2C_SDA
$Comp
L Interface_USB:CH340G U4
U 1 1 5C6BD2A9
P 2800 7050
F 0 "U4" H 2800 6364 50  0000 C CNN
F 1 "CH340C" H 2800 6273 50  0000 C CNN
F 2 "Package_SO:SOIC-16_3.9x9.9mm_P1.27mm" H 2850 6500 50  0001 L CNN
F 3 "http://www.datasheet5.com/pdf-local-2195953" H 2450 7850 50  0001 C CNN
	1    2800 7050
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR0126
U 1 1 5C6BD52B
P 2800 6450
F 0 "#PWR0126" H 2800 6300 50  0001 C CNN
F 1 "VCC" H 2817 6623 50  0000 C CNN
F 2 "" H 2800 6450 50  0001 C CNN
F 3 "" H 2800 6450 50  0001 C CNN
	1    2800 6450
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0127
U 1 1 5C6BD606
P 3100 7650
F 0 "#PWR0127" H 3100 7400 50  0001 C CNN
F 1 "GND" H 3105 7477 50  0000 C CNN
F 2 "" H 3100 7650 50  0001 C CNN
F 3 "" H 3100 7650 50  0001 C CNN
	1    3100 7650
	1    0    0    -1  
$EndComp
Wire Wire Line
	2800 7650 3100 7650
Wire Wire Line
	2800 1300 3600 1300
Wire Wire Line
	3700 6750 3200 6750
Wire Wire Line
	2800 1100 3600 1100
Wire Wire Line
	3700 6650 3200 6650
Wire Wire Line
	3600 1000 2800 1000
$Comp
L Device:C C11
U 1 1 5C7E458C
P 600 1800
F 0 "C11" H 715 1846 50  0000 L CNN
F 1 "100n" H 715 1755 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D3.4mm_W2.1mm_P2.50mm" H 638 1650 50  0001 C CNN
F 3 "~" H 600 1800 50  0001 C CNN
	1    600  1800
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0128
U 1 1 5C7E4652
P 600 2150
F 0 "#PWR0128" H 600 1900 50  0001 C CNN
F 1 "GND" H 605 1977 50  0000 C CNN
F 2 "" H 600 2150 50  0001 C CNN
F 3 "" H 600 2150 50  0001 C CNN
	1    600  2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	600  1000 1600 1000
Wire Wire Line
	3800 2400 2800 2400
$Comp
L Connector:Screw_Terminal_01x05 J5
U 1 1 5C8635C7
P 6800 6900
F 0 "J5" H 6880 6942 50  0000 L CNN
F 1 "Screw_Terminal_01x05" H 6880 6851 50  0000 L CNN
F 2 "TerminalBlock_4Ucon:TerminalBlock_4Ucon_1x05_P3.50mm_Vertical" H 6800 6900 50  0001 C CNN
F 3 "~" H 6800 6900 50  0001 C CNN
	1    6800 6900
	1    0    0    -1  
$EndComp
$Comp
L Device:D_Schottky D2
U 1 1 5C86388C
P 4950 6300
F 0 "D2" H 4950 6516 50  0000 C CNN
F 1 "D_Schottky" H 4950 6425 50  0000 C CNN
F 2 "Diode_THT:D_A-405_P7.62mm_Horizontal" H 4950 6300 50  0001 C CNN
F 3 "~" H 4950 6300 50  0001 C CNN
	1    4950 6300
	-1   0    0    1   
$EndComp
$Comp
L Device:D_Schottky D1
U 1 1 5C863AE2
P 4950 5900
F 0 "D1" H 4950 6116 50  0000 C CNN
F 1 "D_Schottky" H 4950 6025 50  0000 C CNN
F 2 "Diode_THT:D_A-405_P7.62mm_Horizontal" H 4950 5900 50  0001 C CNN
F 3 "~" H 4950 5900 50  0001 C CNN
	1    4950 5900
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR0121
U 1 1 5C887A4B
P 6500 7200
F 0 "#PWR0121" H 6500 6950 50  0001 C CNN
F 1 "GND" H 6505 7027 50  0000 C CNN
F 2 "" H 6500 7200 50  0001 C CNN
F 3 "" H 6500 7200 50  0001 C CNN
	1    6500 7200
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR0129
U 1 1 5C8983BC
P 6150 6650
F 0 "#PWR0129" H 6150 6500 50  0001 C CNN
F 1 "VCC" H 6167 6823 50  0000 C CNN
F 2 "" H 6150 6650 50  0001 C CNN
F 3 "" H 6150 6650 50  0001 C CNN
	1    6150 6650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0130
U 1 1 5C8A9444
P 5950 6550
F 0 "#PWR0130" H 5950 6300 50  0001 C CNN
F 1 "GND" H 5955 6377 50  0000 C CNN
F 2 "" H 5950 6550 50  0001 C CNN
F 3 "" H 5950 6550 50  0001 C CNN
	1    5950 6550
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR0131
U 1 1 5C8DCB6F
P 6500 6300
F 0 "#PWR0131" H 6500 6150 50  0001 C CNN
F 1 "VCC" H 6517 6473 50  0000 C CNN
F 2 "" H 6500 6300 50  0001 C CNN
F 3 "" H 6500 6300 50  0001 C CNN
	1    6500 6300
	1    0    0    -1  
$EndComp
Wire Wire Line
	5100 5900 5100 6300
$Comp
L Connector:Conn_01x04_Female J2
U 1 1 5C694C1B
P 600 4950
F 0 "J2" H 627 4926 50  0000 L CNN
F 1 "LCD Header" V 627 4835 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 600 4950 50  0001 C CNN
F 3 "~" H 600 4950 50  0001 C CNN
	1    600  4950
	-1   0    0    1   
$EndComp
Wire Wire Line
	7750 5800 7750 5900
Wire Wire Line
	6600 6800 6500 6800
Wire Wire Line
	6500 6800 6500 7200
Wire Wire Line
	6600 6700 6150 6700
Wire Wire Line
	6150 6700 6150 6650
Wire Wire Line
	6200 7100 6600 7100
Wire Wire Line
	6200 7000 6600 7000
Text Label 6250 7000 0    50   ~ 0
CANL
Text Label 6250 7100 0    50   ~ 0
CANH
$Comp
L Device:CP C12
U 1 1 5CC9FCEC
P 650 2900
F 0 "C12" H 768 2946 50  0000 L CNN
F 1 "470u" H 768 2855 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D10.0mm_P5.00mm" H 688 2750 50  0001 C CNN
F 3 "~" H 650 2900 50  0001 C CNN
	1    650  2900
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR0132
U 1 1 5CC9FED9
P 650 2650
F 0 "#PWR0132" H 650 2500 50  0001 C CNN
F 1 "VCC" H 667 2823 50  0000 C CNN
F 2 "" H 650 2650 50  0001 C CNN
F 3 "" H 650 2650 50  0001 C CNN
	1    650  2650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0133
U 1 1 5CC9FF5A
P 650 3100
F 0 "#PWR0133" H 650 2850 50  0001 C CNN
F 1 "GND" H 655 2927 50  0000 C CNN
F 2 "" H 650 3100 50  0001 C CNN
F 3 "" H 650 3100 50  0001 C CNN
	1    650  3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	650  3100 650  3050
Wire Wire Line
	650  2750 650  2650
$Comp
L Device:CP C13
U 1 1 5CCE58DA
P 1050 2900
F 0 "C13" H 1168 2946 50  0000 L CNN
F 1 "4u7" H 1168 2855 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D5.0mm_P2.50mm" H 1088 2750 50  0001 C CNN
F 3 "~" H 1050 2900 50  0001 C CNN
	1    1050 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	650  2750 1050 2750
Connection ~ 650  2750
Wire Wire Line
	650  3050 1050 3050
Connection ~ 650  3050
Wire Wire Line
	10500 5850 10500 6300
Text Label 3150 1100 0    50   ~ 0
BOARD_TX0
Text Label 3150 1300 0    50   ~ 0
BOARD_RX0
Text Label 3150 1000 0    50   ~ 0
GPIO0
Text Label 1000 1000 0    50   ~ 0
EN
Text Label 5100 6300 0    50   ~ 0
VinCommon
Text Label 4600 6300 0    50   ~ 0
VinUsb
Text Label 4600 5900 0    50   ~ 0
VinCAN
Text Label 1150 5900 0    50   ~ 0
USB+
Text Label 1150 6000 0    50   ~ 0
USB-
Wire Wire Line
	3550 3200 2800 3200
Text Label 8250 5100 0    50   ~ 0
CANTx
Text Label 8250 5200 0    50   ~ 0
CANRx
Wire Wire Line
	3350 1600 2800 1600
Wire Wire Line
	3350 1800 2800 1800
Wire Wire Line
	3350 1700 2800 1700
Wire Wire Line
	3350 1900 2800 1900
Wire Wire Line
	3350 2200 2800 2200
Wire Wire Line
	3800 2300 2800 2300
Wire Wire Line
	2800 2000 3350 2000
NoConn ~ 4800 -400
$Comp
L RF_Module:ESP32-WROOM-32 U1
U 1 1 5C688B86
P 2200 2200
F 0 "U1" H 1850 3550 50  0000 C CNN
F 1 "ESP32-WROOM-32" H 2600 3550 50  0000 C CNN
F 2 "RF_Module:ESP32-WROOM-32" H 2200 700 50  0001 C CNN
F 3 "https://www.espressif.com/sites/default/files/documentation/esp32-wroom-32d_esp32-wroom-32u_datasheet_en.pdf" H 1900 2250 50  0001 C CNN
	1    2200 2200
	1    0    0    -1  
$EndComp
$Comp
L Jumper:SolderJumper_3_Open JP1
U 1 1 5C9758C6
P 1050 5050
F 0 "JP1" V 1004 5117 50  0000 L CNN
F 1 "PIN1" V 1095 5117 50  0000 L CNN
F 2 "Jumper:SolderJumper-3_P1.3mm_Open_Pad1.0x1.5mm_NumberLabels" H 1050 5050 50  0001 C CNN
F 3 "~" H 1050 5050 50  0001 C CNN
	1    1050 5050
	0    1    1    0   
$EndComp
$Comp
L Jumper:SolderJumper_3_Open JP2
U 1 1 5C98D090
P 1450 4950
F 0 "JP2" V 1404 5017 50  0000 L CNN
F 1 "PIN2" V 1495 5017 50  0000 L CNN
F 2 "Jumper:SolderJumper-3_P1.3mm_Open_Pad1.0x1.5mm_NumberLabels" H 1450 4950 50  0001 C CNN
F 3 "~" H 1450 4950 50  0001 C CNN
	1    1450 4950
	0    1    1    0   
$EndComp
Wire Wire Line
	800  5050 900  5050
Wire Wire Line
	800  4950 1300 4950
Wire Wire Line
	1050 4850 1050 4750
Wire Wire Line
	1450 4750 1050 4750
Connection ~ 1050 4750
Wire Wire Line
	1050 4750 1050 4350
Wire Wire Line
	1250 5150 1250 5250
Wire Wire Line
	1250 5250 1050 5250
Connection ~ 1450 5150
Wire Wire Line
	1450 5150 1250 5150
Wire Wire Line
	900  4550 1500 4550
Wire Wire Line
	800  4700 1500 4700
Wire Wire Line
	900  4550 900  4850
Wire Wire Line
	900  4850 800  4850
Wire Wire Line
	800  4700 800  4750
Wire Wire Line
	6200 6900 6600 6900
Wire Wire Line
	4550 6300 4800 6300
Text Label 3250 6750 0    50   ~ 0
BOARD_TX0
Text Label 3250 6650 0    50   ~ 0
BOARD_RX0
Wire Wire Line
	6350 6300 6500 6300
Wire Wire Line
	5950 6500 5950 6550
Connection ~ 5100 6300
Text Label 5200 7750 0    50   ~ 0
EN
Text Label 5150 6850 0    50   ~ 0
GPIO0
Text Label 3400 1400 2    50   ~ 0
CANRx
Text Label 3150 1500 0    50   ~ 0
CANTx
Text Label 3150 3200 0    50   ~ 0
S88_DATA
Text Label 7850 6000 0    50   ~ 0
CANL
Text Label 7850 6100 0    50   ~ 0
CANH
Text Label 6200 6900 0    50   ~ 0
VinCAN
Wire Wire Line
	4550 5900 4800 5900
Text Label 7850 5900 0    50   ~ 0
VinCAN
Wire Wire Line
	7750 5900 8150 5900
Text Label 1150 5700 0    50   ~ 0
VinUsb
Text Label 3150 1600 0    50   ~ 0
F0
Text Label 3150 1700 0    50   ~ 0
F1
Text Label 3150 1900 0    50   ~ 0
F2
Text Label 3150 1800 0    50   ~ 0
F3
Text Label 3150 2100 0    50   ~ 0
F4
Text Label 3150 2000 0    50   ~ 0
Shift
Text Label 3150 2300 0    50   ~ 0
Encoder_A
Text Label 3150 2400 0    50   ~ 0
EncoderB
Text Label 3150 2500 0    50   ~ 0
Encoder_Button
Text Label 3150 2200 0    50   ~ 0
Stop
Wire Wire Line
	2800 2100 3350 2100
Wire Wire Line
	2800 2500 3800 2500
NoConn ~ 6350 6050
NoConn ~ 5550 6050
NoConn ~ 6150 6150
Text Label 3150 3300 0    50   ~ 0
S88_CLOCK_UC
Text Label 3150 3100 0    50   ~ 0
S88_RESET_UC
Text Label 3150 3000 0    50   ~ 0
S88_PS_UC
Wire Wire Line
	2800 3100 3550 3100
Wire Wire Line
	2800 3000 3550 3000
Wire Wire Line
	2800 3300 3550 3300
NoConn ~ 1600 2200
NoConn ~ 1600 2300
NoConn ~ 1600 2400
NoConn ~ 1600 2500
NoConn ~ 1600 2600
NoConn ~ 1600 2700
Text Label 3150 2800 0    50   ~ 0
I2C_SCL
Text Label 3150 2900 0    50   ~ 0
I2C_SDA
Wire Wire Line
	2800 2900 3550 2900
Wire Wire Line
	2800 2800 3550 2800
Wire Wire Line
	6250 2950 5550 2950
Wire Wire Line
	6300 3050 5550 3050
Wire Wire Line
	5550 2850 6350 2850
Wire Wire Line
	5550 2650 6350 2650
Wire Wire Line
	6550 3950 5550 3950
Text Label 5900 2650 0    50   ~ 0
BOARD_TX0
Text Label 5900 2850 0    50   ~ 0
BOARD_RX0
Wire Wire Line
	6300 4750 5550 4750
Wire Wire Line
	6100 3150 5550 3150
Wire Wire Line
	6100 3350 5550 3350
Wire Wire Line
	6100 3250 5550 3250
Wire Wire Line
	6100 3450 5550 3450
Wire Wire Line
	6100 3750 5550 3750
Wire Wire Line
	6550 3850 5550 3850
Wire Wire Line
	5550 3550 6100 3550
Text Label 6150 2950 2    50   ~ 0
CANRx
Text Label 5900 3050 0    50   ~ 0
CANTx
Text Label 5900 4750 0    50   ~ 0
S88_DATA
Text Label 5900 3150 0    50   ~ 0
F0
Text Label 5900 3250 0    50   ~ 0
F1
Text Label 5900 3450 0    50   ~ 0
F2
Text Label 5900 3350 0    50   ~ 0
F3
Text Label 5900 3650 0    50   ~ 0
F4
Text Label 5900 3550 0    50   ~ 0
Shift
Text Label 5900 3850 0    50   ~ 0
Encoder_A
Text Label 5900 3950 0    50   ~ 0
EncoderB
Text Label 5900 4050 0    50   ~ 0
Encoder_Button
Text Label 5900 3750 0    50   ~ 0
Stop
Wire Wire Line
	5550 3650 6100 3650
Wire Wire Line
	5550 4050 6550 4050
Text Label 5900 4850 0    50   ~ 0
S88_CLOCK_UC
Text Label 5900 4650 0    50   ~ 0
S88_RESET_UC
Text Label 5900 4550 0    50   ~ 0
S88_PS_UC
Wire Wire Line
	5550 4650 6300 4650
Wire Wire Line
	5550 4550 6300 4550
Wire Wire Line
	5550 4850 6300 4850
Text Label 5900 4350 0    50   ~ 0
I2C_SCL
Text Label 5900 4450 0    50   ~ 0
I2C_SDA
Wire Wire Line
	5550 4450 6300 4450
Wire Wire Line
	5550 4350 6300 4350
Wire Wire Line
	4350 2550 4200 2550
Text Label 4200 2550 0    50   ~ 0
EN
$Comp
L power:VCC #PWR0134
U 1 1 5D3841C8
P 4950 2300
F 0 "#PWR0134" H 4950 2150 50  0001 C CNN
F 1 "VCC" H 4967 2473 50  0000 C CNN
F 2 "" H 4950 2300 50  0001 C CNN
F 3 "" H 4950 2300 50  0001 C CNN
	1    4950 2300
	1    0    0    -1  
$EndComp
Wire Wire Line
	4950 2350 4950 2300
$Comp
L power:GND #PWR0135
U 1 1 5D3982AF
P 4950 5400
F 0 "#PWR0135" H 4950 5150 50  0001 C CNN
F 1 "GND" H 4955 5227 50  0000 C CNN
F 2 "" H 4950 5400 50  0001 C CNN
F 3 "" H 4950 5400 50  0001 C CNN
	1    4950 5400
	1    0    0    -1  
$EndComp
Wire Wire Line
	4950 5400 4950 5150
Wire Wire Line
	4650 2200 4150 2200
Text Label 4150 2200 0    50   ~ 0
VinCommon
Wire Wire Line
	2800 2600 3150 2600
Wire Wire Line
	2800 2700 3150 2700
Wire Wire Line
	2800 1200 3150 1200
Wire Wire Line
	5550 4150 5900 4150
Wire Wire Line
	5550 4250 5900 4250
Wire Wire Line
	5550 2750 5900 2750
Text Label 2850 1200 0    50   ~ 0
IO2
Text Label 2850 2600 0    50   ~ 0
IO23
Text Label 2850 2700 0    50   ~ 0
IO25
Text Label 5650 2750 0    50   ~ 0
IO2
Text Label 5650 4150 0    50   ~ 0
IO23
Text Label 5650 4250 0    50   ~ 0
IO25
Wire Wire Line
	4350 2750 4200 2750
Wire Wire Line
	4350 2850 4200 2850
Text Label 4200 2750 0    50   ~ 0
VP
Text Label 4200 2850 0    50   ~ 0
VN
Wire Wire Line
	1600 1200 1450 1200
Wire Wire Line
	1600 1300 1450 1300
Text Label 1450 1200 0    50   ~ 0
VP
Text Label 1450 1300 0    50   ~ 0
VN
$Comp
L Transistor_BJT:BC547 Q1
U 1 1 5CEDA052
P 4950 7050
F 0 "Q1" H 5141 7096 50  0000 L CNN
F 1 "BC547" H 5141 7005 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 5150 6975 50  0001 L CIN
F 3 "http://www.fairchildsemi.com/ds/BC/BC547.pdf" H 4950 7050 50  0001 L CNN
	1    4950 7050
	1    0    0    -1  
$EndComp
$Comp
L Transistor_BJT:BC547 Q2
U 1 1 5CEDBC79
P 4950 7550
F 0 "Q2" H 5141 7504 50  0000 L CNN
F 1 "BC547" H 5141 7595 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 5150 7475 50  0001 L CIN
F 3 "http://www.fairchildsemi.com/ds/BC/BC547.pdf" H 4950 7550 50  0001 L CNN
	1    4950 7550
	1    0    0    1   
$EndComp
$Comp
L Device:R R21
U 1 1 5CEDC084
P 4550 7050
F 0 "R21" V 4343 7050 50  0000 C CNN
F 1 "10k" V 4434 7050 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 4480 7050 50  0001 C CNN
F 3 "~" H 4550 7050 50  0001 C CNN
	1    4550 7050
	0    1    1    0   
$EndComp
$Comp
L Device:R R22
U 1 1 5CEDCB26
P 4550 7550
F 0 "R22" V 4343 7550 50  0000 C CNN
F 1 "10k" V 4434 7550 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 4480 7550 50  0001 C CNN
F 3 "~" H 4550 7550 50  0001 C CNN
	1    4550 7550
	0    1    1    0   
$EndComp
Wire Wire Line
	4700 7050 4750 7050
Wire Wire Line
	4700 7550 4750 7550
Wire Wire Line
	5050 6850 5350 6850
Wire Wire Line
	4400 7050 4400 7250
Wire Wire Line
	4400 7350 5050 7350
Wire Wire Line
	4400 7550 4300 7550
Wire Wire Line
	4100 7250 4400 7250
Connection ~ 4400 7250
Wire Wire Line
	4400 7250 4400 7350
Wire Wire Line
	4300 7550 4300 7300
Wire Wire Line
	4300 7300 5050 7300
Wire Wire Line
	5050 7300 5050 7250
Connection ~ 4300 7550
Wire Wire Line
	4300 7550 4100 7550
Wire Wire Line
	5050 7750 5350 7750
$Comp
L Device:CP C14
U 1 1 5D029F71
P 1000 1800
F 0 "C14" H 1118 1846 50  0000 L CNN
F 1 "10u" H 1118 1755 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D10.0mm_P5.00mm" H 1038 1650 50  0001 C CNN
F 3 "~" H 1000 1800 50  0001 C CNN
	1    1000 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	600  1000 600  1650
Wire Wire Line
	1000 1650 600  1650
Connection ~ 600  1650
Wire Wire Line
	600  1950 600  2050
$Comp
L Switch:SW_Push SW9
U 1 1 5D10ED6D
P 1350 1850
F 0 "SW9" V 1304 1998 50  0000 L CNN
F 1 "SW_EN" V 1395 1998 50  0000 L CNN
F 2 "Button_Switch_THT:SW_PUSH_6mm" H 1350 2050 50  0001 C CNN
F 3 "~" H 1350 2050 50  0001 C CNN
	1    1350 1850
	0    1    1    0   
$EndComp
$Comp
L Switch:SW_Push SW10
U 1 1 5D10FE38
P 1250 3550
F 0 "SW10" H 1250 3835 50  0000 C CNN
F 1 "SW_GPIO0" H 1250 3744 50  0000 C CNN
F 2 "Button_Switch_THT:SW_PUSH_6mm" H 1250 3750 50  0001 C CNN
F 3 "~" H 1250 3750 50  0001 C CNN
	1    1250 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	1350 2050 1000 2050
Connection ~ 600  2050
Wire Wire Line
	600  2050 600  2150
Wire Wire Line
	1000 1950 1000 2050
Connection ~ 1000 2050
Wire Wire Line
	1000 2050 600  2050
Wire Wire Line
	1000 1650 1350 1650
Connection ~ 1000 1650
Wire Wire Line
	1050 3550 600  3550
$Comp
L power:GND #PWR0136
U 1 1 5D1A9BCD
P 1700 3850
F 0 "#PWR0136" H 1700 3600 50  0001 C CNN
F 1 "GND" H 1705 3677 50  0000 C CNN
F 2 "" H 1700 3850 50  0001 C CNN
F 3 "" H 1700 3850 50  0001 C CNN
	1    1700 3850
	1    0    0    -1  
$EndComp
$Comp
L Device:C C15
U 1 1 5D1A9F89
P 1300 3750
F 0 "C15" H 1415 3796 50  0000 L CNN
F 1 "100n" H 1415 3705 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D3.4mm_W2.1mm_P2.50mm" H 1338 3600 50  0001 C CNN
F 3 "~" H 1300 3750 50  0001 C CNN
	1    1300 3750
	0    1    1    0   
$EndComp
Wire Wire Line
	1050 3550 1050 3750
Wire Wire Line
	1050 3750 1150 3750
Connection ~ 1050 3550
Wire Wire Line
	1450 3550 1700 3550
Wire Wire Line
	1700 3550 1700 3750
Wire Wire Line
	1450 3750 1700 3750
Connection ~ 1700 3750
Wire Wire Line
	1700 3750 1700 3850
Text Label 650  3550 0    50   ~ 0
GPIO0
$Comp
L Connector:Conn_01x05_Female J6
U 1 1 5D230E2F
P 1700 5900
F 0 "J6" H 1728 5926 50  0000 L CNN
F 1 "USBConn BO" H 1728 5835 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x05_P2.54mm_Vertical" H 1700 5900 50  0001 C CNN
F 3 "~" H 1700 5900 50  0001 C CNN
	1    1700 5900
	1    0    0    -1  
$EndComp
Wire Wire Line
	1100 5900 1500 5900
Wire Wire Line
	1100 5700 1500 5700
Wire Wire Line
	1100 6000 1400 6000
Wire Wire Line
	800  6300 1500 6300
Wire Wire Line
	1500 6300 1500 6100
Wire Wire Line
	1400 6000 1400 5800
Wire Wire Line
	1400 5800 1500 5800
Wire Wire Line
	1100 6100 1450 6100
Wire Wire Line
	1450 6100 1450 6000
Wire Wire Line
	1450 6000 1500 6000
Text Label 2050 7250 0    50   ~ 0
USB-
Text Label 2050 7150 0    50   ~ 0
USB+
$Comp
L Connector:Conn_01x08_Female J7
U 1 1 5CF15553
P 1400 7050
F 0 "J7" H 1292 7535 50  0000 C CNN
F 1 "USB BO L" H 1292 7444 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x08_P2.54mm_Vertical" H 1400 7050 50  0001 C CNN
F 3 "~" H 1400 7050 50  0001 C CNN
	1    1400 7050
	-1   0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x08_Female J9
U 1 1 5CF1A90B
P 3650 7250
F 0 "J9" H 3678 7226 50  0000 L CNN
F 1 "USB BO R" H 3678 7135 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x08_P2.54mm_Vertical" H 3650 7250 50  0001 C CNN
F 3 "~" H 3650 7250 50  0001 C CNN
	1    3650 7250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0116
U 1 1 5D00ACF1
P 1450 5150
F 0 "#PWR0116" H 1450 4900 50  0001 C CNN
F 1 "GND" H 1455 4977 50  0000 C CNN
F 2 "" H 1450 5150 50  0001 C CNN
F 3 "" H 1450 5150 50  0001 C CNN
	1    1450 5150
	1    0    0    -1  
$EndComp
Wire Wire Line
	3200 6950 3450 6950
Wire Wire Line
	3200 7050 3450 7050
Wire Wire Line
	3200 7150 3450 7150
Wire Wire Line
	3200 7250 3450 7250
Wire Wire Line
	3200 7350 3450 7350
Wire Wire Line
	3200 7450 3450 7450
Wire Wire Line
	2400 7450 1600 7450
Wire Wire Line
	2400 7250 2350 7250
Wire Wire Line
	2350 7250 2350 7350
Wire Wire Line
	2350 7350 1600 7350
Wire Wire Line
	1600 7250 2300 7250
Wire Wire Line
	2300 7250 2300 7050
Wire Wire Line
	2300 7050 2400 7050
Wire Wire Line
	1600 7150 2250 7150
Wire Wire Line
	2250 7150 2250 6950
Wire Wire Line
	2250 6950 2400 6950
Wire Wire Line
	2700 6450 2200 6450
Wire Wire Line
	2200 6450 2200 7050
Wire Wire Line
	2200 7050 1600 7050
Text Label 1650 6950 0    50   ~ 0
BOARD_TX0
Text Label 1650 6850 0    50   ~ 0
BOARD_RX0
Wire Wire Line
	2100 6950 1600 6950
Wire Wire Line
	2100 6850 1600 6850
$Comp
L power:GND #PWR0120
U 1 1 5C78F49E
P 7800 6200
F 0 "#PWR0120" H 7800 5950 50  0001 C CNN
F 1 "GND" H 7805 6027 50  0000 C CNN
F 2 "" H 7800 6200 50  0001 C CNN
F 3 "" H 7800 6200 50  0001 C CNN
	1    7800 6200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0119
U 1 1 5D302D5F
P 1950 6700
F 0 "#PWR0119" H 1950 6450 50  0001 C CNN
F 1 "GND" H 1955 6527 50  0000 C CNN
F 2 "" H 1950 6700 50  0001 C CNN
F 3 "" H 1950 6700 50  0001 C CNN
	1    1950 6700
	-1   0    0    1   
$EndComp
Wire Wire Line
	1600 6750 1950 6750
Wire Wire Line
	1950 6750 1950 6700
Wire Wire Line
	2400 6750 2150 6750
Wire Wire Line
	2150 6750 2150 7600
Wire Wire Line
	2150 7600 3250 7600
Wire Wire Line
	3250 7600 3250 7550
Wire Wire Line
	3250 7550 3450 7550
$Comp
L power:VCC #PWR0137
U 1 1 5D38325F
P 3750 7650
F 0 "#PWR0137" H 3750 7500 50  0001 C CNN
F 1 "VCC" H 3767 7823 50  0000 C CNN
F 2 "" H 3750 7650 50  0001 C CNN
F 3 "" H 3750 7650 50  0001 C CNN
	1    3750 7650
	1    0    0    -1  
$EndComp
Wire Wire Line
	3450 7650 3300 7700
Wire Wire Line
	3300 7700 3750 7700
Wire Wire Line
	3750 7700 3750 7650
Connection ~ 7750 5900
Wire Wire Line
	8550 5100 8250 5100
Wire Wire Line
	8550 5200 8250 5200
Text Label 9600 5400 0    50   ~ 0
CANL
Text Label 9600 5200 0    50   ~ 0
CANH
Wire Wire Line
	9550 5200 9800 5200
$Comp
L power:VCC #PWR0138
U 1 1 5D579C90
P 9050 4900
F 0 "#PWR0138" H 9050 4750 50  0001 C CNN
F 1 "VCC" H 9067 5073 50  0000 C CNN
F 2 "" H 9050 4900 50  0001 C CNN
F 3 "" H 9050 4900 50  0001 C CNN
	1    9050 4900
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0139
U 1 1 5D59B7E3
P 9050 5700
F 0 "#PWR0139" H 9050 5450 50  0001 C CNN
F 1 "GND" H 9055 5527 50  0000 C CNN
F 2 "" H 9050 5700 50  0001 C CNN
F 3 "" H 9050 5700 50  0001 C CNN
	1    9050 5700
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x06_Female J12
U 1 1 5DAC7498
P 9800 5900
F 0 "J12" H 9828 5876 50  0000 L CNN
F 1 "CAN BO" H 9828 5785 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x06_P2.54mm_Vertical" H 9800 5900 50  0001 C CNN
F 3 "~" H 9800 5900 50  0001 C CNN
	1    9800 5900
	1    0    0    -1  
$EndComp
Wire Wire Line
	9550 5400 9800 5400
$Comp
L power:VCC #PWR0141
U 1 1 5DB457F4
P 9550 5650
F 0 "#PWR0141" H 9550 5500 50  0001 C CNN
F 1 "VCC" H 9567 5823 50  0000 C CNN
F 2 "" H 9550 5650 50  0001 C CNN
F 3 "" H 9550 5650 50  0001 C CNN
	1    9550 5650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0142
U 1 1 5DB45B98
P 9550 6250
F 0 "#PWR0142" H 9550 6000 50  0001 C CNN
F 1 "GND" H 9555 6077 50  0000 C CNN
F 2 "" H 9550 6250 50  0001 C CNN
F 3 "" H 9550 6250 50  0001 C CNN
	1    9550 6250
	1    0    0    -1  
$EndComp
Wire Wire Line
	9550 5650 9550 5700
Wire Wire Line
	9550 5700 9600 5700
Wire Wire Line
	9600 5800 9550 5800
Wire Wire Line
	9550 5800 9550 6250
Text Label 9300 6200 0    50   ~ 0
CANL
Text Label 9300 6100 0    50   ~ 0
CANH
Wire Wire Line
	9300 6100 9600 6100
Wire Wire Line
	9300 6200 9600 6200
Text Label 9300 5900 0    50   ~ 0
CANTx
Text Label 9300 6000 0    50   ~ 0
CANRx
Wire Wire Line
	9600 5900 9300 5900
Wire Wire Line
	9600 6000 9300 6000
Text Label 3250 7350 0    50   ~ 0
!DTR
Text Label 3250 7450 0    50   ~ 0
!RTS
Text Label 4100 7250 0    50   ~ 0
!DTR
Text Label 4100 7550 0    50   ~ 0
!RTS
$Comp
L Connector:Conn_01x15_Female J10
U 1 1 5DE86E6F
P 4000 4400
F 0 "J10" H 4028 4426 50  0000 L CNN
F 1 "ESP32 BO L" H 4028 4335 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x15_P2.54mm_Vertical" H 4000 4400 50  0001 C CNN
F 3 "~" H 4000 4400 50  0001 C CNN
	1    4000 4400
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x15_Female J8
U 1 1 5DE8BDE6
P 2850 4450
F 0 "J8" H 2878 4476 50  0000 L CNN
F 1 "ESP32 BO R" H 2878 4385 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x15_P2.54mm_Vertical" H 2850 4450 50  0001 C CNN
F 3 "~" H 2850 4450 50  0001 C CNN
	1    2850 4450
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR0143
U 1 1 5DE8CDB9
P 3750 3650
F 0 "#PWR0143" H 3750 3500 50  0001 C CNN
F 1 "VCC" H 3767 3823 50  0000 C CNN
F 2 "" H 3750 3650 50  0001 C CNN
F 3 "" H 3750 3650 50  0001 C CNN
	1    3750 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	3750 3650 3750 3700
Wire Wire Line
	3750 3700 3800 3700
Wire Wire Line
	3800 3900 3550 3900
Text Label 3550 3900 0    50   ~ 0
F2
Wire Wire Line
	3800 4000 3550 4000
Wire Wire Line
	3800 4100 3550 4100
Wire Wire Line
	3800 4200 3550 4200
Wire Wire Line
	3800 4300 3550 4300
Wire Wire Line
	3800 4400 3550 4400
Wire Wire Line
	3800 4500 3550 4500
Wire Wire Line
	3800 4600 3400 4600
Wire Wire Line
	3800 4700 3400 4700
Wire Wire Line
	3800 4800 3350 4800
Wire Wire Line
	3800 4900 3350 4900
Wire Wire Line
	3800 5000 3200 5000
Wire Wire Line
	3800 5100 3550 5100
Text Label 3550 4000 0    50   ~ 0
IO2
Text Label 3550 4100 0    50   ~ 0
CANRx
Text Label 3550 4400 0    50   ~ 0
CANTx
Text Label 3550 4200 0    50   ~ 0
Shift
Text Label 3550 4300 0    50   ~ 0
F4
Text Label 3550 4500 0    50   ~ 0
Stop
Text Label 3400 4600 0    50   ~ 0
Encoder_A
Text Label 3400 4700 0    50   ~ 0
EncoderB
Text Label 3200 5000 0    50   ~ 0
Encoder_Button
Text Label 3550 5100 0    50   ~ 0
IO23
Text Label 3350 4800 0    50   ~ 0
BOARD_RX0
Text Label 3350 4900 0    50   ~ 0
BOARD_TX0
$Comp
L power:GND #PWR0144
U 1 1 5E1443E4
P 3400 3900
F 0 "#PWR0144" H 3400 3650 50  0001 C CNN
F 1 "GND" H 3405 3727 50  0000 C CNN
F 2 "" H 3400 3900 50  0001 C CNN
F 3 "" H 3400 3900 50  0001 C CNN
	1    3400 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	3400 3900 3400 3800
Wire Wire Line
	3400 3800 3800 3800
Wire Wire Line
	2650 3750 2400 3750
Wire Wire Line
	2650 3850 2400 3850
Wire Wire Line
	2650 3950 2400 3950
Wire Wire Line
	2650 4050 2300 4050
Wire Wire Line
	2650 4150 2100 4150
Wire Wire Line
	2650 4250 2250 4250
Wire Wire Line
	2650 4350 2100 4350
Wire Wire Line
	2650 4450 2400 4450
Wire Wire Line
	2650 4550 2350 4550
Wire Wire Line
	2650 4650 2350 4650
Wire Wire Line
	2650 4750 2400 4750
Wire Wire Line
	2650 4850 2400 4850
Wire Wire Line
	2650 4950 2400 4950
Wire Wire Line
	2650 5050 2100 5050
Wire Wire Line
	2650 5150 2200 5150
Text Label 2400 3750 0    50   ~ 0
EN
Text Label 2400 3850 0    50   ~ 0
VP
Text Label 2400 3950 0    50   ~ 0
VN
Text Label 2300 4050 0    50   ~ 0
S88_DATA
Text Label 2100 4150 0    50   ~ 0
S88_CLOCK_UC
Text Label 2250 4250 0    50   ~ 0
S88_PS_UC
Text Label 2100 4350 0    50   ~ 0
S88_RESET_UC
Text Label 2400 4450 0    50   ~ 0
IO25
Text Label 2350 4550 0    50   ~ 0
I2C_SCL
Text Label 2350 4650 0    50   ~ 0
I2C_SDA
Text Label 2400 4750 0    50   ~ 0
F3
Text Label 2400 4850 0    50   ~ 0
F0
Text Label 2400 4950 0    50   ~ 0
F1
Text Label 2200 5150 0    50   ~ 0
VinReg
$Comp
L power:GND #PWR0145
U 1 1 5E3E6169
P 2100 5050
F 0 "#PWR0145" H 2100 4800 50  0001 C CNN
F 1 "GND" H 2105 4877 50  0000 C CNN
F 2 "" H 2100 5050 50  0001 C CNN
F 3 "" H 2100 5050 50  0001 C CNN
	1    2100 5050
	1    0    0    -1  
$EndComp
$Comp
L ESP32EvalBoard:DOIT_ESP32_DEVKIT_V1 U3
U 1 1 5CF2463E
P 4950 3750
F 0 "U3" H 4950 2261 50  0000 C CNN
F 1 "DOIT_ESP32_DEVKIT_V1" H 4950 2170 50  0000 C CNN
F 2 "ESP32EvalBoard:ESP32-DEVKITV1" H 4950 2250 50  0001 C CNN
F 3 "https://www.espressif.com/sites/default/files/documentation/esp32-wroom-32_datasheet_en.pdf" H 4650 3800 50  0001 C CNN
	1    4950 3750
	1    0    0    -1  
$EndComp
$Comp
L PowerBoards:D24V10F3 U5
U 1 1 5CF289AE
P 5950 6200
F 0 "U5" H 5950 6615 50  0000 C CNN
F 1 "D24V10F3" H 5950 6524 50  0000 C CNN
F 2 "PowerBoards:D24V10F3" H 5950 6200 50  0001 C CNN
F 3 "https://www.pololu.com/product/2830" H 5950 6200 50  0001 C CNN
	1    5950 6200
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x06_Female J11
U 1 1 5CFAC4FF
P 8350 5800
F 0 "J11" H 8378 5776 50  0000 L CNN
F 1 "RJ45 BO" H 8378 5685 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x06_P2.54mm_Vertical" H 8350 5800 50  0001 C CNN
F 3 "~" H 8350 5800 50  0001 C CNN
	1    8350 5800
	1    0    0    -1  
$EndComp
Wire Wire Line
	7800 5800 8150 5800
Connection ~ 7800 5800
Wire Wire Line
	7800 5800 7800 6200
Wire Wire Line
	7750 5500 7850 5500
Wire Wire Line
	7850 5500 7850 5700
Wire Wire Line
	7850 5700 8150 5700
Wire Wire Line
	7750 5400 7900 5400
Wire Wire Line
	7900 5400 7900 5600
Wire Wire Line
	7900 5600 8150 5600
Wire Wire Line
	4750 2350 4750 2200
$Comp
L Connector:Conn_01x02_Male J4
U 1 1 5D064780
P 4750 2000
F 0 "J4" V 4812 2044 50  0000 L CNN
F 1 "Conn_01x02_Male" V 4903 2044 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 4750 2000 50  0001 C CNN
F 3 "~" H 4750 2000 50  0001 C CNN
	1    4750 2000
	0    1    1    0   
$EndComp
Text Label 4750 2300 2    50   ~ 0
VinReg
Wire Wire Line
	5100 6300 5550 6300
$EndSCHEMATC