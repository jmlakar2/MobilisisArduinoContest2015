#include <LiquidCrystal.h>
#include <stdlib.h>
#include <EEPROM.h>

#define	C0 16.35
#define	Db0	17.32
#define	D0	18.35
#define	Eb0	19.45
#define	E0	20.60
#define	F0	21.83
#define	Gb0	23.12
#define	G0	24.50
#define	Ab0	25.96
#define	LA0	27.50
#define	Bb0	29.14
#define	B0	30.87
#define	C1	32.70
#define	Db1	34.65
#define	D1	36.71
#define	Eb1	38.89
#define	E1	41.20
#define	F1	43.65
#define	Gb1	46.25
#define	G1	49.00
#define	Ab1	51.91
#define	LA1	55.00
#define	Bb1	58.27
#define	B1	61.74
#define	C2	65.41
#define	Db2	69.30
#define	D2	73.42
#define	Eb2	77.78
#define	E2	82.41
#define	F2	87.31
#define	Gb2	92.50
#define	G2	98.00
#define	Ab2	103.83
#define	LA2	110.00
#define	Bb2	116.54
#define	B2	123.47
#define	C3	130.81
#define	Db3	138.59
#define	D3	146.83
#define	Eb3	155.56
#define	E3	164.81
#define	F3	174.61
#define	Gb3	185.00
#define	G3	196.00
#define	Ab3	207.65
#define	LA3	220.00
#define	Bb3	233.08
#define	B3	246.94
#define	C4	261.63
#define	Db4	277.18
#define	D4	293.66
#define	Eb4	311.13
#define	E4	329.63
#define	F4	349.23
#define	Gb4	369.99
#define	G4	392.00
#define	Ab4	415.30
#define	LA4	440.00
#define	Bb4	466.16
#define	B4	493.88
#define	C5	523.25
#define	Db5	554.37
#define	D5	587.33
#define	Eb5	622.25
#define	E5	659.26
#define	F5	698.46
#define	Gb5	739.99
#define	G5	783.99
#define	Ab5	830.61
#define	LA5	880.00
#define	Bb5	932.33
#define	B5	987.77
#define	C6	1046.50
#define	Db6	1108.73
#define	D6	1174.66
#define	Eb6	1244.51
#define	E6	1318.51
#define	F6	1396.91
#define	Gb6	1479.98
#define	G6	1567.98
#define	Ab6	1661.22
#define	LA6	1760.00
#define	Bb6	1864.66
#define	B6	1975.53
#define	C7	2093.00
#define	Db7	2217.46
#define	D7	2349.32
#define	Eb7	2489.02
#define	E7	2637.02
#define	F7	2793.83
#define	Gb7	2959.96
#define	G7	3135.96
#define	Ab7	3322.44
#define	LA7	3520.01
#define	Bb7	3729.31
#define	B7	3951.07
#define	C8	4186.01
#define	Db8	4434.92
#define	D8	4698.64
#define	Eb8	4978.03

#define BPM 120    
#define H 2*Q 
#define Q 60000/BPM 
#define E Q/2   
#define S Q/4 
#define W 4*Q 

#define rLED 2
#define gLED 3
#define bLED 4

#define buzzer 5

#define btn1 6
#define btn2 7

int photo1 = A0;
int photo2 = A2;

int potent = A1;

int avgRead;

LiquidCrystal lcd(8, 9, 10, 11, 12, 13);

int ledArray[] = { 30, rLED, gLED, bLED };
float rgbArray[] = { 0, 0, 0, 0 };
float whiteArray[] = { 0, 0, 0, 0 };
float blackArray[] = { 0, 0, 0, 0 };

char potentValArray[1];
String potentStr = "";

char input[16];
char pass[16] = "12r";
String output = "";
int counter = 0;

bool btn1Pressed = false;
bool btn2Pressed = false;
bool bothPressed = false;

int wrongPassCounter = 0;
int maxWrongPass = 3;
bool wrongPass = false;

bool passChanged = true;

bool addNewPassword = false;
bool addOldPassword = true;

int photo2Input;
bool opened = false;

int light_value = 700;

void setup()
{
	resetInput();

	Serial.begin(9600);

	lcd.begin(16, 2);

	//initial set to output mode for leds and buzzer
	for (int i = 1; i < 4; i++) {
		pinMode(ledArray[i], OUTPUT);
	}

	pinMode(buzzer, OUTPUT);

	//callibration of a color detector

	//wait till right button is pressed and black paper preapeared
	setLCDtext("PUT WHITE PAPER", "PRESS X 0");
	while (true) {
		if (digitalRead(btn1) == HIGH) {
			setLCDtext("WAIT...", "");
			setWhiteBalance();
			break;
		}
	}

	setLCDtext("PUT BLACK PAPER", "PRESS 0 X");
	//wait till right button is pressed and black paper preapeared
	while (true) {
		if (digitalRead(btn2) == HIGH) {
			setLCDtext("WAIT...", "");
			setBlackBalance();
			break;
		}
	}
	setLCDtext("DEVICE IS", "CALIBRATED...");
	delay(2000);

}

void loop()
{ 
	photo2Input = analogRead(photo2);
	//detects opened safe
	//if it's opened sends serial command
	if (photo2Input >= light_value && !opened) {
		Serial.println("opened");
		opened = true;
	}
	else if (photo2Input<light_value){
		opened = false;
	}

	//check if there is already password in a EEPROM
	if (EEPROM.read(0) != 255 && passChanged) {
		int addr = 0;
		int eepromVal;
		pass[0] = 0;
		while ((eepromVal = EEPROM.read(addr)) != 255) {
			pass[addr] = char(eepromVal);
			++addr;
		}
		passChanged = false;
	}
	
	//checks in each iteration if it's time to enter new password or to 
	//confirm new
	if (!addNewPassword) {
		if (addOldPassword) {
			Serial.println("ENTER YOUR OLD PASSWORD:");
			addOldPassword = false;
		}
		bool isCorrectOldPass = true;
		if (Serial.available() > 0)
		{
			int x = 0;
			char passChar;
			while (Serial.available() > 0)
			{
				passChar = char(Serial.read());
				
				if (passChar != '\n' && passChar != '\r'){
					//checks if each char of a an input is equal to password
					if (pass[x] != passChar) {
						isCorrectOldPass = false;
					}
				}
				++x;
				delay(2);
			}
			//if password is not connected prints message to LCD
			if (!isCorrectOldPass) {
				Serial.println("WRONG PASSWORD!");
				addOldPassword = true;
			}
			//else set bool variable to true so entering new password can start
			else {
				addNewPassword = true;
				Serial.println("NEW PASSWORD:");
			}

		}
	}
	else {
		if (Serial.available() > 0)
		{
			int x = 0;
			//saves new password to EEPROM
			while (Serial.available() > 0)
			{
				EEPROM.write(x, Serial.read());
				++x;
				delay(2);
			}
			Serial.println("PASSWORD HAS BEEN CHANGED!");
			passChanged = true;
			addNewPassword = false;
			addOldPassword = true;
		}
	}


	readColor();
	int r = int(rgbArray[1]);
	int g = int(rgbArray[2]);
	int b = int(rgbArray[3]);

	String color = printColor(r, g, b);

	int potentVal = readPotent();
	dtostrf(potentVal, 1, 0, potentValArray);
	potentStr += potentValArray[0];
	delay(100);
	String currentInput;
	if (!wrongPass) {
		currentInput = "INPUT: " + potentStr + " " + color;
	}
	else {
		currentInput = "WRONG! " + potentStr + " " + color;
	}

	setLCDtext(currentInput, output);

	//if both buttons are pressed check password
	if (digitalRead(btn1) == HIGH && digitalRead(btn2) == HIGH && !bothPressed) {
		output = "";
		counter = 0;
		wrongPass = false;
		//itterate trough password and check if input is wrong
		for (int i = 0; i < strlen(pass) - 2; i++){
			if (input[i] != pass[i]) {
				wrongPass = true;
				break;
			}
		}
		//if wron password add to wrong pass counter
		if (wrongPass) {
			++wrongPassCounter;
			if (wrongPassCounter == maxWrongPass) {
				setLCDtext("INTRUDER", "DETECTED!");
				Serial.println("intruder");
				playMusic();
			}
		}
		else {
			setLCDtext("SAFE OPENED", "X X TO CLOSE");
			int wrongPassCounter = 0;
			delay(1500);
			while (digitalRead(btn1) == LOW || digitalRead(btn2) == LOW) {

			}

		}
		resetInput();
		bothPressed = true;
	}
	else if ((digitalRead(btn1) == LOW && digitalRead(btn2)) == LOW && bothPressed) {
		bothPressed = false;
	}
	else {
		//if btn1 is pressed add potentiometer value to password array
		if (digitalRead(btn1) == HIGH && !btn1Pressed) {
			input[counter] = potentValArray[0];
			output += "*";
			tone(buzzer, Ab3, Q);
			btn1Pressed = true;
			++counter;
		}
		else if (digitalRead(btn1) == LOW && btn1Pressed) {
			btn1Pressed = false;
		}
		//if btn2 is pressed add color value to password array
		if (digitalRead(btn2) == HIGH && !btn2Pressed) {
			input[counter] = color[0];
			output += "*";
			tone(buzzer, Ab3, Q);
			btn2Pressed = true;
			++counter;
		}
		else if (digitalRead(btn2) == LOW && btn2Pressed) {
			btn2Pressed = false;
		}
	}
	potentStr = "";

}

void setLCDtext(String firstRow, String secondRow) {
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print(firstRow);
	lcd.setCursor(0, 1);
	lcd.print(secondRow);
}

void setWhiteBalance() {
	for (int i = 1; i <= 3; i++){
		digitalWrite(ledArray[i], HIGH);
		delay(100);
		getReading(5);
		whiteArray[i] = avgRead;
		digitalWrite(ledArray[i], LOW);
		delay(100);
	}
}

void setBlackBalance() {
	for (int i = 1; i <= 3; i++){
		digitalWrite(ledArray[i], HIGH);
		delay(100);
		getReading(5);
		blackArray[i] = avgRead;
		digitalWrite(ledArray[i], LOW);
		delay(100);
	}
}

void getReading(int times){
	int reading;
	int tally = 0;
	for (int i = 0; i < times; i++){
		reading = analogRead(photo1);
		tally = reading + tally;
		delay(10);
	}
	avgRead = (tally) / times;
}

void readColor(){
	for (int i = 1; i <= 3; i++){
		digitalWrite(ledArray[i], HIGH);
		delay(100);
		getReading(5);
		rgbArray[i] = avgRead;
		float greyDiff = whiteArray[i] - blackArray[i];
		rgbArray[i] = (rgbArray[i] - blackArray[i]) / (greyDiff)* 255;
		digitalWrite(ledArray[i], LOW);
		delay(100);
	}
}

int readPotent() {
	int potentVal = analogRead(potent);
	return (potentVal);
}

String printColor(int r, int g, int b) {
	if (r == 0 || b == 0 || g == 0) {
		return("b");
	}
	else if (r > g && r > b) {
		return ("r");
	}
	else if (g > r && g > b) {
		return ("g");
	}
	else if (b > r && b > g) {
		return ("b");
	}
	return ("u");
}

void resetInput() {
	for (int i = 0; i < 16; i++) {
		input[i] = '_';
	}
}

void playMusic() {
	while (true){
		tone(buzzer, LA3, Q);
		delay(1 + Q);
		tone(buzzer, LA3, Q);
		delay(1 + Q);
		tone(buzzer, LA3, Q);
		delay(1 + Q);
		tone(buzzer, F3, E + S);
		delay(1 + E + S);
		tone(buzzer, C4, S);
		delay(1 + S);

		tone(buzzer, LA3, Q);
		delay(1 + Q);
		tone(buzzer, F3, E + S);
		delay(1 + E + S);
		tone(buzzer, C4, S);
		delay(1 + S);
		tone(buzzer, LA3, H);
		delay(1 + H);

		tone(buzzer, E4, Q);
		delay(1 + Q);
		tone(buzzer, E4, Q);
		delay(1 + Q);
		tone(buzzer, E4, Q);
		delay(1 + Q);
		tone(buzzer, F4, E + S);
		delay(1 + E + S);
		tone(buzzer, C4, S);
		delay(1 + S);

		tone(buzzer, Ab3, Q);
		delay(1 + Q);
		tone(buzzer, F3, E + S);
		delay(1 + E + S);
		tone(buzzer, C4, S);
		delay(1 + S);
		tone(buzzer, LA3, H);
		delay(1 + H);

		tone(buzzer, LA4, Q);
		delay(1 + Q);
		tone(buzzer, LA3, E + S);
		delay(1 + E + S);
		tone(buzzer, LA3, S);
		delay(1 + S);
		tone(buzzer, LA4, Q);
		delay(1 + Q);
		tone(buzzer, Ab4, E + S);
		delay(1 + E + S);
		tone(buzzer, G4, S);
		delay(1 + S);

		tone(buzzer, Gb4, S);
		delay(1 + S);
		tone(buzzer, E4, S);
		delay(1 + S);
		tone(buzzer, F4, E);
		delay(1 + E);
		delay(1 + E);
		tone(buzzer, Bb3, E);
		delay(1 + E);
		tone(buzzer, Eb4, Q);
		delay(1 + Q);
		tone(buzzer, D4, E + S);
		delay(1 + E + S);
		tone(buzzer, Db4, S);
		delay(1 + S);
	}
}