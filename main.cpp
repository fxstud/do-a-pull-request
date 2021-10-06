// MODIFIED
#include "mbed.h"

#define PERIOD 1000
#define SERVO_PERIOD 20000
#define STRAIGHT 1
#define BACK 2
#define LEFT 3
#define RIGHT 4

//Diagonal Motor
#define DFR 5
#define DBR 6
#define DFL 7
#define DBL 8

//Motors
#define BR 1
#define BL 2
#define FL 3
#define FR 4

//Wings Servo Values
#define LEFTWINGDOWN 1950
#define LEFTWINGUP 900
#define RIGHTWINGDOWN 900
#define RIGHTWINGUP 1850

//Flag Servo Values
#define FLAGDOWN 1780
#define FLAGUP 800

//Head Servo Values
#define PENISUP 1950
#define PENISDOWN 900

class Counter
{
public:
    Counter(PinName pin) : _interrupt(pin)
    {
        // create the InterruptIn on the pin specified to Counter

        _interrupt.rise(this, &Counter::increment);
        // attach increment function of this counter instance
    }
    void increment()
    {
        _count++;
    }
    int read()
    {
        return _count;
    }
    void reset()
    {
        _count = 0;
    }

private:
    InterruptIn _interrupt;
    volatile int _count;
};

//**********************init methods
void initMotors();
void checkServos();

//**********************Navigation methods

void corrige(int decombien, int dir, bool enableSensors, int corrpwm, bool bHardStop);
void reculetoutdroit(int decombien);
//Powering the engines
void power(int motor, int pulse);
void powerM(int bl, int br, int fl, int fr);
void powerAll(int pulse);
void powerAllSkew(int pulse);

int moymoy(int a, int b, int c, int d);

void clignote(int sec);

void resetCounters();
void updateCounters();

void shutdownM();
void go(int start, int end, int time, int dir);
void diaGo(int pwm, int time, int fb, int lr);
void stop(int start, int end, int time);

void waitstart();
void waitplug();

//***************************Ground Methods
void match();
void teamRight();
void teamLeft();

void homo();
void homoRight();
void homoLeft();
void homoSided(int side);

void dodgeBack();
void dodgeLeft();
void dodgeRight();

void hardStop();

int photo();
void bato();

//***********************Programming methods
void sendToRasp(int a);
void debug(char *array, int truc);
void printCpts();
void endMatchIfNeeded();

//2019
void catchBeer();
void rotate(int angle, int dir);
void activateVent();
void desactivateVent();
void setPenis(int pwm, int speed);
void setPenis(int pwm) { setPenis(pwm, 50); };
void setAileDroite(int pwm, int speed);
void setAileDroite(int pwm) { setAileDroite(pwm, 50); };
void setAileGauche(int pwm, int speed);
void setAileGauche(int pwm) { setAileGauche(pwm, 50); };
void setWing(int side, int pwm) {if(side == LEFT) setAileGauche(pwm); if (side == RIGHT) setAileDroite(pwm);};
void setDrapeau(int pwm, int speed);
void setDrapeau(int pwm) { setDrapeau(pwm, 50); };
void dancecouikcouik();

void manualControl();

int min(int a, int b);
int max(int a, int b);
int min(int a, int b, int c, int d);
int max(int a, int b, int c, int d);


//Define Pin's status and Values
PwmOut myled(LED1);

//BackwardLeft Motor
PwmOut pwmBL(PC_6);

DigitalOut dirBL1(PA_12);
DigitalOut dirBL2(PA_11);
Counter counterBL(PC_4);

//BackWardRight Motor
PwmOut pwmBR(PC_8);
DigitalOut dirBR1(PA_14);
DigitalOut dirBR2(PA_13);
Counter counterBR(PC_2);

//ForwardLeft Motor
PwmOut pwmFL(D10);
DigitalOut dirFL1(D8);
DigitalOut dirFL2(D7);
Counter counterFL(PB_13);

//FowardRight Motor
PwmOut pwmFR(D9);
DigitalOut dirFR1(D4);
DigitalOut dirFR2(D2);
Counter counterFR(PB_1);

PwmOut pwmFlag(A3);
PwmOut pwmAileDroite(PA_15);
PwmOut pwmAileGauche(PB_10); // D6
PwmOut pwmPenis(D3);

//Switches:
DigitalIn startButton(PC_3);
DigitalIn teamSwitch(PB_12);
DigitalIn nonameSwitch(PC_5);

//Globals Variables
int cBL = 0;
int cBR = 0;
int cFL = 0;
int cFR = 0;

int powerBL = 0;
int powerBR = 0;
int powerFL = 0;
int powerFR = 0;
int powerPenis = 0;
int powerFlag = 0;
int powerAileDroite = 0;
int powerAileGauche = 0;

Timer t;

int zeroIfTeamRightOneElse = 1;

int cpt = 0;

Serial pc(USBTX, USBRX);

int shareddist = 0;
int shareddecombien = 0;

int main()
{
    while (true)
    {
        initMotors();
        checkServos();
////        cBL = counterBL.read();
////        cBR = counterBR.read();
////        cFL = counterFL.read();
////        cFR = counterFR.read();
////        wait(0.01);
////    	setDrapeau(FLAGUP);
////	wait(1);
////    	setDrapeau(FLAGDOWN);
////	wait(1);
////	//go(500, 700, 2.5, RIGHT);
////
////    	corrige(12000, RIGHT, true, 500, true);
////	stop(0,0,0);
////        int deltaBL = counterBL.read()-cBL;
////        int deltaBR = counterBR.read()-cBR;
////        int deltaFL = counterFL.read()-cFL;
////        int deltaFR = counterFR.read()-cFR;
////        wait(0.01);
////    	//if (deltaBL>10000) setDrapeau(FLAGUP);
////	//setDrapeau(deltaBR/10);
////	debug("deltaBL",deltaBL);
////	debug("deltaBR",deltaBR);
////	debug("deltaFL",deltaFL);
////	debug("deltaFR",deltaFR);
////	
////	go(400, 700, 3, LEFT);
////	stop(0,0,0);
	
    	//corrige(15000, RIGHT, true, 500, true);
    	//corrige(5000, RIGHT, true, 500, true);
        waitstart();
//         manualControl();
        homo();

        // photo();
        // wait(10);

        // bato();

        // manualControl();

        powerAll(0);
        waitplug();
    }
}

int photo()
{
    pc.printf("%c\n", 'g');
    wait(1);
    for (size_t ii = 0; ii < 1000; ii++)
    {
        char c = '\0';
        if (pc.readable())
        {
            c = pc.getc();
            if (c == 'n' || c == 'N')
            {
                return 1;
            }
            if (c == 's' || c == 'S' || c == 'z')
            {
                return 0;
            }
        }
            
        // if (c == 'z')
        // {
        //     return;
        // }

        wait(0.01);
    }
}

void bato()
{
    corrige(3500, RIGHT, false, 700, true);
    rotate(84, LEFT);
    corrige(2000, BACK, false, 700, true);
    corrige(600, LEFT, false, 700, true);
    corrige(800, BACK, false, 700, true);
    wait(0.4);
    corrige(2000, RIGHT, false, 700, true);
    corrige(22000, STRAIGHT, false, 700, true);
    wait(0.1);
    corrige(500, STRAIGHT, false, 600, true);
    corrige(600, LEFT, false, 700, true);
    corrige(600, STRAIGHT, false, 600, true);
    wait(0.4);
    setPenis(800, 10);
    corrige(6000, RIGHT, false, 700, true);
    wait(0.5);
    setPenis(1800, 10);
    wait(0.5);
    corrige(6000, LEFT, false, 700, true);
}

void manualControl()
{
    int reste = 0;
    char ch = '.';
    char lastch = '.';
    while (true)
    {
        if (pc.readable())
        {
            ch = pc.getc();
            if (ch == lastch)
            {
                reste += 1;
            }
            else
            {
                reste = 0;
            }
            // debug("char", ch);

            if (ch == 'w')
            {
                initMotors();
                wait(0.05);
            }
            if (ch == 'z')
            {
                go(0, 0, 0, STRAIGHT);
                powerAll(500);
                wait(0.05);
            }
            if (ch == 's')
            {
                go(0, 0, 0, BACK);
                powerAll(500);
                wait(0.05);
            }
            if (ch == 'q')
            {
                go(0, 0, 0, LEFT);
                powerAll(500);
                wait(0.05);
            }
            if (ch == 'd')
            {
                go(0, 0, 0, RIGHT);
                powerAll(500);
                wait(0.05);
            }
            if (ch == 'a')
            {
                go(0, 0, 0, STRAIGHT);
                dirBL1 = !dirBL1;
                dirBL2 = !dirBL2;
                dirFL1 = !dirFL1;
                dirFL2 = !dirFL2;
                powerAll(500);
                wait(0.05);
            }
            if (ch == 'e')
            {
                go(0, 0, 0, STRAIGHT);
                dirBR1 = !dirBR1;
                dirBR2 = !dirBR2;
                dirFR1 = !dirFR1;
                dirFR2 = !dirFR2;
                powerAll(500);
                wait(0.05);
            }
            if (ch == 'i')
            {
                powerPenis += 2 * reste;
                powerPenis = min(powerPenis, PENISUP);
                pwmPenis.pulsewidth_us(powerPenis);
            }
            if (ch == 'k')
            {
                powerPenis -= 2 * reste;
                powerPenis = max(powerPenis, PENISDOWN);
                pwmPenis.pulsewidth_us(powerPenis);
            }
            if (ch == 'u')
            {
                powerAileGauche -= 2 * reste;
                powerAileGauche = max(powerAileGauche, LEFTWINGUP);
                pwmAileGauche.pulsewidth_us(powerAileGauche);
            }
            if (ch == 'j')
            {
                powerAileGauche += 2 * reste;
                powerAileGauche = min(powerAileGauche, LEFTWINGDOWN);
                pwmAileGauche.pulsewidth_us(powerAileGauche);
            }
            if (ch == 'o')
            {
                powerAileDroite += 2 * reste;
                powerAileDroite = min(powerAileDroite, RIGHTWINGUP);
                pwmAileDroite.pulsewidth_us(powerAileDroite);
            }
            if (ch == 'l')
            {
                powerAileDroite -= 2 * reste;
                powerAileDroite = max(powerAileDroite, RIGHTWINGDOWN);
                pwmAileDroite.pulsewidth_us(powerAileDroite);
            }
            if (ch == 'p')
            {
                powerFlag -= 2 * reste;
                powerFlag = max(powerFlag, FLAGUP);
                pwmFlag.pulsewidth_us(powerFlag);
            }
            if (ch == 'm')
            {
                powerFlag += 2 * reste;
                powerFlag = min(powerFlag, FLAGDOWN);
                pwmFlag.pulsewidth_us(powerFlag);
            }
            if (ch == '$')
            {
                return;
                wait(0.05);
            }
        }
        else
        {
            powerAll(0);
        }
        lastch = ch;
    }
}

void setPenis(int pwm, int speed)
{
    if (pwm < powerPenis)
        for (int ii = powerPenis; ii > max(pwm, PENISDOWN); ii -= speed)
        {
            pwmPenis.pulsewidth_us(ii);
            wait(0.01);
            powerPenis = ii;
        }
    else
        for (int ii = powerPenis; ii < min(pwm, PENISUP); ii += speed)
        {
            pwmPenis.pulsewidth_us(ii);
            wait(0.01);
            powerPenis = ii;
        }
}

void setDrapeau(int pwm, int speed)
{
    if (pwm < powerFlag)
        for (int ii = powerFlag; ii > max(pwm, FLAGUP); ii -= speed)
        {
            pwmFlag.pulsewidth_us(ii);
            wait(0.01);
            powerFlag = ii;
        }
    else
        for (int ii = powerFlag; ii < min(pwm, FLAGDOWN); ii += speed)
        {
            pwmFlag.pulsewidth_us(ii);
            wait(0.01);
            powerFlag = ii;
        }
}

void setAileGauche(int pwm, int speed)
{
    if (pwm < powerAileGauche)
        for (int ii = powerAileGauche; ii > max(pwm, LEFTWINGUP); ii -= speed)
        {
            pwmAileGauche.pulsewidth_us(ii);
            wait(0.01);
            powerAileGauche = ii;
        }
    else
        for (int ii = powerAileGauche; ii < min(pwm, LEFTWINGDOWN); ii += speed)
        {
            pwmAileGauche.pulsewidth_us(ii);
            wait(0.01);
            powerAileGauche = ii;
        }
}

void setAileDroite(int pwm, int speed)
{
    if (pwm < powerAileDroite)
        for (int ii = powerAileDroite; ii > max(pwm, RIGHTWINGDOWN); ii -= speed)
        {
            pwmAileDroite.pulsewidth_us(ii);
            wait(0.01);
            powerAileDroite = ii;
        }
    else
        for (int ii = powerAileDroite; ii < min(pwm, RIGHTWINGUP); ii += speed)
        {
            pwmAileDroite.pulsewidth_us(ii);
            wait(0.01);
            powerAileDroite = ii;
        }
}

void rotate(int angle, int dir)
{
    endMatchIfNeeded();
    if (dir == LEFT)
    {
        dirBL1 = 0;
        dirBL2 = 1;
        dirFL1 = 0;
        dirFL2 = 1;
        dirBR1 = 1;
        dirBR2 = 0;
        dirFR1 = 1;
        dirFR2 = 0;
    }
    if (dir == RIGHT)
    {
        dirBL1 = 1;
        dirBL2 = 0;
        dirFL1 = 1;
        dirFL2 = 0;
        dirBR1 = 0;
        dirBR2 = 1;
        dirFR1 = 0;
        dirFR2 = 1;
    }
    powerAll(600);
    resetCounters();
    int moy = 0;
    // while (moy < 4660) {
    // while (moy < (19825 * angle / 360 - 800)) {
    while (moy < (19825 * angle / 360))
    {
        cBL = counterBL.read();
        cBR = counterBR.read();
        cFL = counterFL.read();
        cFR = counterFR.read();
        wait(0.01);
        moy = moymoy(cBL, cBR, cFL, cFR);
    }
    hardStop();
    wait(1);
}

void sendToRasp(int score)
{
    // pc.printf('%d', score);
}
void debug(char *array, int truc)
{
    pc.printf("%s : %d\n", array, truc);
}

void teamLeft()
{

    corrige(3200, LEFT, true, 900, true);
    go(400, 500, 1, BACK);
    resetCounters();
    // go(400, 500, 0.5, STRAIGHT);
    corrige(8500, STRAIGHT, false, 500, true);
    corrige(3500, RIGHT, true, 900, true);
    //il a poussé le rouge
    corrige(5500, BACK, true, 500, true);
    corrige(12000, STRAIGHT, true, 500, true);
    corrige(1000, RIGHT, true, 900, true);

    corrige(2000, STRAIGHT, true, 500, true);
    rotate(82, RIGHT);

    corrige(10000, STRAIGHT, true, 500, true);
    corrige(2000, RIGHT, false, 900, true);
    corrige(1000, STRAIGHT, false, 500, true);

    go(300, 400, 1, STRAIGHT);
    go(400, 500, 1, LEFT);
    go(300, 400, 1, STRAIGHT);
    resetCounters();
    //calé devant la balance
    corrige(2700, BACK, true, 500, true);

    rotate(52, LEFT);
    corrige(300, RIGHT, false, 900, true);
    corrige(13500, BACK, true, 500, true);
    // il a poussé 4 + rouge
    corrige(5500, STRAIGHT, true, 500, true);
    corrige(4000, RIGHT, true, 900, true);
    rotate(10, RIGHT);
    corrige(7000, BACK, true, 500, true);
    //il a poussé le vert

    corrige(3000, STRAIGHT, true, 500, true);
    rotate(30, LEFT);
    corrige(4000, RIGHT, true, 900, true);
    corrige(3000, BACK, false, 500, true);
    go(400, 500, 1.5, BACK);
    go(500, 600, 3, RIGHT);
    go(200, 300, 1, BACK);
    resetCounters();
    //calé dans l'angle

    go(300, 400, 1, BACK);
    resetCounters();
    //calé dans l'angle

    corrige(1000, LEFT, true, 900, true);

    go(200, 300, 1, BACK);
    resetCounters();
    //VROUMVROUM
    go(0, 0, 0, STRAIGHT);
    powerAll(1100);
    wait(1);
    powerAll(700);
    wait(2);

    //VROUMVROUM
    go(0, 0, 0, BACK);
    powerAll(500);
    wait(2);
    hardStop();

    go(400, 500, 1, RIGHT);
    go(200, 400, 1, BACK);
    go(300, 400, 1, RIGHT);
    go(200, 300, 1, BACK);

    resetCounters();
    // corrige(9000, LEFT, true, 900, true);
    // go(400, 500, 1, BACK);
    // resetCounters();

    // ONLY LEFT
    // go(400, 500, 1, BACK);
    corrige(950, LEFT, true, 900, true);
    corrige(580, STRAIGHT, true, 300, true);

    activateVent();
    setPenis(1800, 5);
    setPenis(900, 5);
    go(400, 500, 1, RIGHT);
    go(400, 500, 1, BACK);
    //collé au coin

    corrige(8000, LEFT, true, 900, true);
    corrige(17000, STRAIGHT, true, 500, true);
    go(400, 500, 1, RIGHT);
    setPenis(2100, 30);
    desactivateVent();
    setPenis(900, 30);
    //dans la balance

    for (size_t ii = 1; ii <= 6; ii++)
    {
        go(400, 500, 1, RIGHT);
        go(200, 250, 1, STRAIGHT);
        corrige(950, BACK, true, 500, true);
        wait(0.7);
        corrige(700, LEFT, true, 800, true);
        corrige(4000 + ii * 1405, BACK, false, 350, true);
        go(400, 500, 1, RIGHT);
        corrige(1000, LEFT, true, 900, true);

        activateVent();
        setPenis(1820, 3);
        setPenis(900, 3);
        corrige(4000 + ii * 1400, STRAIGHT, false, 500, true);
        go(400, 500, 1, RIGHT);
        // go(300, 400, 1, STRAIGHT);
        // go(400, 500, 1, RIGHT);
        setPenis(2100, 40);
        desactivateVent();
        wait(1);
        setPenis(900, 30);
        go(300, 400, 1, STRAIGHT);
    }
}

void teamRight()
{
    corrige(3200, RIGHT, true, 900, true);
    go(400, 500, 1, BACK);
    resetCounters();
    // go(400, 500, 0.5, STRAIGHT);
    corrige(8500, STRAIGHT, false, 500, true);
    corrige(3500, LEFT, true, 900, true);
    //il a poussé le rouge
    corrige(5500, BACK, true, 500, true);
    corrige(10000, STRAIGHT, true, 500, true);
    corrige(1000, LEFT, true, 900, true);

    corrige(2000, STRAIGHT, true, 500, true);
    rotate(85, LEFT);

    corrige(8000, STRAIGHT, true, 500, true);
    corrige(2000, LEFT, false, 900, true);
    corrige(1000, STRAIGHT, false, 500, true);

    go(300, 400, 1, STRAIGHT);
    go(400, 500, 1, RIGHT);
    go(300, 400, 1, STRAIGHT);
    resetCounters();
    //calé devant la balance
    corrige(2700, BACK, true, 500, true);

    rotate(62, RIGHT);
    corrige(600, LEFT, false, 900, true);
    corrige(16500, BACK, true, 500, true);
    // il a poussé 4 + rouge
    corrige(7500, STRAIGHT, true, 500, true);
    corrige(5000, LEFT, true, 900, true);
    rotate(10, LEFT);
    corrige(8000, BACK, true, 500, true);
    //il a poussé le vert

    corrige(3000, STRAIGHT, true, 500, true);
    rotate(60, RIGHT);
    corrige(4000, LEFT, true, 900, true);
    corrige(3000, BACK, false, 500, true);
    go(400, 500, 3, BACK);
    go(500, 600, 3, LEFT);
    go(200, 300, 1, BACK);
    resetCounters();
    //calé dans l'angle

    corrige(1000, RIGHT, true, 900, true);

    go(300, 400, 1, BACK);
    resetCounters();

    //VROUMVROUM
    go(0, 0, 0, STRAIGHT);
    powerAll(1100);
    wait(1);
    powerAll(700);
    wait(2);

    //VROUMVROUM
    go(0, 0, 0, BACK);
    powerAll(500);
    wait(2);
    hardStop();

    go(400, 500, 1, LEFT);
    go(300, 400, 1, BACK);
    go(400, 500, 1, LEFT);
    resetCounters();
    corrige(9000, RIGHT, true, 900, true);
    go(400, 500, 1, BACK);
    resetCounters();

    // ONLY RIGHT
    corrige(2500, STRAIGHT, true, 500, true);
    rotate(175, LEFT);
    corrige(12000, BACK, true, 500, true);
    go(400, 500, 1, RIGHT);
    go(400, 500, 1, BACK);

    //calé balance à droite

    for (size_t ii = 1; ii <= 6; ii++)
    {
        corrige(1000, STRAIGHT, true, 500, true);
        corrige(1350, LEFT, true, 800, true);
        corrige(ii * 1400, STRAIGHT, false, 400, true);

        activateVent();
        setPenis(1820, 3);
        setPenis(900, 3);
        corrige(ii * 1400, BACK, false, 500, true);
        go(400, 500, 1, RIGHT);
        go(300, 400, 1, BACK);
        go(400, 500, 1, RIGHT);
        rotate(58, RIGHT);
        setPenis(2100, 40);
        desactivateVent();
        wait(1);
        setPenis(900, 30);
        rotate(45, LEFT);
        go(400, 500, 1, RIGHT);
        go(200, 250, 1, BACK);
    }
    // corrige(3000, STRAIGHT, true, 500, true);
    // go(700, 800, 1, RIGHT);
    // corrige(2000, LEFT, true, 900, true);
    // rotate(80, LEFT);
    // go(400, 500, 1, BACK);
    // go(400, 500, 1, LEFT);
    // go(300, 400, 1, BACK);
    //
    // resetCounters();
    // corrige(13000, STRAIGHT, true, 500, true);
    // corrige(3000, STRAIGHT, false, 500, true);
    // rotate(85, LEFT);
    // go(700, 800, 1.5, RIGHT);
    // corrige(2500, STRAIGHT, false, 500, true);
    // setPenis(1250, 5);
    // corrige(2000, STRAIGHT, true, 500, true);
    // setPenis(800, 20);
    // corrige(19000, BACK, true, 500, true);
    // //END
}

void match()
{
    if (zeroIfTeamRightOneElse)
    {
        teamRight();
    }
    else
    {
        teamLeft();
    }
}
void homo()
{
    if (zeroIfTeamRightOneElse)
    {
        // homoSided(RIGHT);
        homoRight();
    }
    else
    {
        // homoSided(LEFT);
        homoLeft();
    }
}
void homoRight()
{
    go(0, 0, 0, BACK);
    wait(1);
    powerM(200, 600, 600, 200);
    wait(4);
    powerAll(0);
    diaGo(500, 1, BACK, LEFT);
//    diaGo(500, 4, BACK, LEFT);
  wait(1);
  // go(300, 500, 0.5, BACK);
  resetCounters(); //calé dans le port N
  corrige(7800, RIGHT, true, 500, true);
  go(300, 500, .5, BACK);
  resetCounters();
  wait(1);
  corrige(5000, STRAIGHT, true, 500, true);
  wait(1);
  corrige(2000, LEFT, true, 500, true);
  corrige(2000, RIGHT, true, 500, true);
  wait(1);
//  rotate(4, RIGHT);
//  wait(1);
  corrige(7000, STRAIGHT, true, 500, true);
  wait(1);
  corrige(2500, LEFT, true, 500, true);
  corrige(2500, RIGHT, true, 500, true);
  wait(1);
//  rotate(4, RIGHT);
//  wait(1);
  corrige(9000, STRAIGHT, true, 500, true);
  wait(1);
  corrige(4000, LEFT, true, 500, true);
  wait(1);
  diaGo(600, 5, STRAIGHT, LEFT);
  resetCounters();
  wait(1);
  setPenis(PENISDOWN);
  wait(1);
//   diaGo(1000, 4, STRAIGHT, RIGHT);
  go(500, 800, 0, RIGHT);
  wait(1);
  powerM(500, 800, 800, 500);
  wait(2);
  powerAll(0);
  setPenis(PENISUP);
  wait(0.2);
  wait(10);
  int cote = photo();
  wait(0.5);
  go(400, 500, 0.7, STRAIGHT);
  resetCounters();
  wait(0.5);

  if(cote == 1){
    corrige(20000, BACK, true, 300, true);
    diaGo(400, 1, BACK, LEFT);
    rotate(80, RIGHT);
    go(300, 500, 4, BACK);
  }
  else {
    corrige(6000, LEFT, true, 500, true);
    diaGo(500, 2, STRAIGHT, LEFT);
    corrige(5000, BACK, true, 400, true);
  }

  //GO NORD

}
void homoLeft()
{
    go(0, 0, 0, BACK);
    wait(1);
    powerM(600, 200, 200, 600);
    wait(4);
    powerAll(0);
    // diaGo(500, 4, BACK, RIGHT);

    wait(1);
    // go(300, 500, 0.5, BACK);
    resetCounters(); //calé dans le port N
    corrige(8500, LEFT, true, 500, true);
    go(300, 500, 1, BACK);
    resetCounters();
    wait(1);
    corrige(5000, STRAIGHT, true, 500, true);
    wait(1);
    corrige(2000, RIGHT, true, 500, true);
    corrige(2000, LEFT, true, 500, true);
       rotate(4, RIGHT);
    wait(.5);
    corrige(7000, STRAIGHT, true, 500, true);
    wait(1);
    corrige(2500, RIGHT, true, 500, true);
    corrige(2500, LEFT, true, 500, true);
       rotate(4, RIGHT);
    wait(.5);
    corrige(9000, STRAIGHT, true, 500, true);
    wait(1);
    corrige(4000, RIGHT, true, 500, true);
    wait(1);
    diaGo(600, 4, STRAIGHT, RIGHT);
    go(400, 500, 1, RIGHT);
    diaGo(800, 2, STRAIGHT, RIGHT);
    resetCounters();
    wait(1);
    setPenis(PENISDOWN);
    wait(1);
    //   diaGo(1000, 4, STRAIGHT, RIGHT);
    go(500, 800, 0, LEFT);
    wait(1);
    powerM(800, 500, 500, 800);
    wait(2);
    powerAll(0);
    setPenis(PENISUP);
    wait(0.2);
    wait(10);
    int cote = photo();
    wait(0.5);
    go(400, 500, 0.7, STRAIGHT);
    resetCounters();
    wait(0.5);

    if(cote == 1){
        corrige(22000, BACK, true, 300, true);
        diaGo(400, 1, BACK, RIGHT);
        rotate(60, LEFT);
        go(300, 500, 4, BACK);
    }
    else {
        corrige(6000, RIGHT, true, 500, true);
        diaGo(500, 2, STRAIGHT, RIGHT);
        corrige(5000, BACK, true, 400, true);
    }
}

void corrige(int decombien, int dir, bool enableSensors, int corrpwm, bool bHardStop)
{
    endMatchIfNeeded();
    int pwm = 700;
    switch (dir)
    {
    case 1: //STRAIGHT
        pwm = corrpwm;
        dirBL1 = 1;
        dirBL2 = 0;
        dirBR1 = 1;
        dirBR2 = 0;
        dirFL1 = 1;
        dirFL2 = 0;
        dirFR1 = 1;
        dirFR2 = 0;
        break;
    case 2: //BACK
        pwm = corrpwm;
        dirBL1 = 0;
        dirBL2 = 1;
        dirBR1 = 0;
        dirBR2 = 1;
        dirFL1 = 0;
        dirFL2 = 1;
        dirFR1 = 0;
        dirFR2 = 1;
        break;
    case 3: //LEFT
        pwm = corrpwm;
        dirBL1 = 1;
        dirBL2 = 0;
        dirBR1 = 0;
        dirBR2 = 1;
        dirFL1 = 0;
        dirFL2 = 1;
        dirFR1 = 1;
        dirFR2 = 0;
        break;
    case 4: //RIGHT
        pwm = corrpwm;
        dirBL1 = 0;
        dirBL2 = 1;
        dirBR1 = 1;
        dirBR2 = 0;
        dirFL1 = 1;
        dirFL2 = 0;
        dirFR1 = 0;
        dirFR2 = 1;
        break;
    default: //STRAIGHT
        pwm = 250;
        dirBL1 = 1;
        dirBL2 = 0;
        dirBR1 = 1;
        dirBR2 = 0;
        dirFL1 = 1;
        dirFL2 = 0;
        dirFR1 = 1;
        dirFR2 = 0;
    }
    resetCounters();
    int dist = 0;
    bool stopped = false;
    //if (dir==4) {powerAllSkew(pwm);} else {powerAll(pwm);}
    int obstacleFront = 0;
    int obstacleBack = 0;
    int obstacleLeft = 0;
    int obstacleRight = 0;
    while (dist < decombien)
    {
        endMatchIfNeeded();
        char c = '\0';
        if (pc.readable())
            c = pc.getc();

        if (c == 'f' && enableSensors && dir == STRAIGHT)
        {
            // debug("f found", 0);
            obstacleFront++;
        }
        else
        {
            obstacleFront = 0;
        }

        // debug("c", 0);
        // debug("enableSensors", 1);

        if (c == 'b' && enableSensors && dir == BACK)
        {
            // debug("b found", 0);
            obstacleBack++;
        }
        else
        {
            obstacleBack = 0;
        }

        if (c == 'l' && enableSensors && dir == LEFT)
        {
            // debug("l found", 0);
            obstacleLeft++;
        }
        else
        {
            obstacleLeft = 0;
        }

        if (c == 'r' && enableSensors && dir == RIGHT)
        {
            // debug("r found", 0);
            obstacleRight++;
        }
        else
        {
            obstacleRight = 0;
        }

        if (obstacleFront > 0 || obstacleBack > 0 || obstacleLeft > 0 || obstacleRight > 0)
        {
            // debug("obstacle detected", 0);
            if (!stopped)
            {
                hardStop();
                stopped = true;
            }

            wait(0.5);
        }
        else
        {
            powerAll(corrpwm);
            stopped = false;
        }

        cBL = counterBL.read();
        cBR = counterBR.read();
        cFL = counterFL.read();
        cFR = counterFR.read();
        wait(0.01);
        int moy = moymoy(cBL, cBR, cFL, cFR);
        if (cFL > moy)
            power(FL, powerFL -= 1);
        if (cFL < moy)
            power(FL, powerFL += 1);
        if (cFR > moy)
            power(FR, powerFR -= 1);
        if (cFR < moy)
            power(FR, powerFR += 1);
        if (cBL > moy)
            power(BL, powerBL -= 1);
        if (cBL < moy)
            power(BL, powerBL += 1);
        if (cBR > moy)
            power(BR, powerBR -= 1);
        if (cBR < moy)
            power(BR, powerBR += 1);

        dist = min(cBL, cBR, cFL, cFR);
    }
    if (bHardStop)
    {
        hardStop();
    }
}


//blink for n seconds
void clignote(int n)
{
    for (int i = 0; i < n; i++)
    {
        myled = 1;
        wait(0.5);
        myled = 0;
        wait(0.5);
    }
    myled = 1;
    wait(0.5);
}

void initMotors()
{
    //4 motors initialization
    pwmBL.period_us(PERIOD);
    pwmBR.period_us(PERIOD);
    pwmFL.period_us(PERIOD);
    pwmFR.period_us(PERIOD);

    //this is initialization for STRAIGHT
    dirBL1 = 1;
    dirBL2 = 0;
    pwmBL.pulsewidth_us(0);
    dirBR1 = 1;
    dirBR2 = 0;
    pwmBR.pulsewidth_us(0);
    dirFL1 = 1;
    dirFL2 = 0;
    pwmFL.pulsewidth_us(0);
    dirFR1 = 1;
    dirFR2 = 0;
    pwmFR.pulsewidth_us(0);

    //IMPORTANT
    // period of servo are always 20 ms
    //pulse of servo are in [800;2200]
    pwmPenis.period_us(SERVO_PERIOD);
    pwmPenis.pulsewidth_us(PENISUP);
    powerPenis = PENISUP;

    pwmAileDroite.period_us(SERVO_PERIOD);
    pwmAileDroite.pulsewidth_us(RIGHTWINGUP);
    powerAileDroite = RIGHTWINGUP;

    pwmAileGauche.period_us(SERVO_PERIOD);
    pwmAileGauche.pulsewidth_us(LEFTWINGUP);
    powerAileGauche = LEFTWINGUP;

    pwmFlag.period_us(SERVO_PERIOD);
    pwmFlag.pulsewidth_us(FLAGDOWN);
    powerFlag = FLAGDOWN;

    startButton.mode(PullUp);
    teamSwitch.mode(PullUp);

    wait(0.1);
    if (teamSwitch)
    {
        zeroIfTeamRightOneElse = 0;
    }
    else
    {
        zeroIfTeamRightOneElse = 1;
    }
}

void checkServos()
{
    setPenis(PENISDOWN);
    setPenis(PENISUP);
    setAileDroite(RIGHTWINGDOWN);
    setAileDroite(RIGHTWINGUP);
    setAileGauche(LEFTWINGDOWN);
    setAileGauche(LEFTWINGUP);
    setDrapeau(FLAGUP);
    setDrapeau(FLAGDOWN);
}

void resetCounters()
{
    counterBL.reset();
    cBL = 0;
    counterBR.reset();
    cBR = 0;
    counterFL.reset();
    cFL = 0;
    counterFR.reset();
    cFR = 0;
}

void power(int motor, int pulse)
{
    if (pulse > 1200)
        pulse = 1200;
    if (pulse < 0)
        pulse = 0;
    if (motor == 1)
    {
        pwmBR.pulsewidth_us(pulse);
        powerBR = pulse;
        return;
    }
    if (motor == 2)
    {
        pwmBL.pulsewidth_us(pulse);
        powerBL = pulse;
        return;
    }
    if (motor == 3)
    {
        pwmFL.pulsewidth_us(pulse);
        powerFL = pulse;
        return;
    }
    if (motor == 4)
    {
        pwmFR.pulsewidth_us(pulse);
        powerFR = pulse;
        return;
    }
}

void powerM(int bl, int br, int fl, int fr)
{
    power(BL, bl);
    power(BR, br);
    power(FL, fl);
    power(FR, fr);
}

void powerAllSkew(int pulse)
{
    powerM(pulse, (int)(0.9*pulse), pulse, pulse);
}
void powerAll(int pulse)
{
    powerM(pulse, pulse, pulse, pulse);
}

void shutdownM()
{
    powerM(0, 0, 0, 0);
}

void diaGo(int pwm, int time, int fb, int lr)
{
    endMatchIfNeeded();
    int nbPas = 20;
    if (fb == STRAIGHT && lr == LEFT)
    {
        dirBL1 = 1;
        dirBL2 = 0;
        dirBR1 = 0;
        dirBR2 = 0;
        dirFL1 = 0;
        dirFL2 = 0;
        dirFR1 = 1;
        dirFR2 = 0;
    }
    if (fb == STRAIGHT && lr == RIGHT)
    {
        dirBL1 = 0;
        dirBL2 = 0;
        dirBR1 = 1;
        dirBR2 = 0;
        dirFL1 = 1;
        dirFL2 = 0;
        dirFR1 = 0;
        dirFR2 = 0;
    }
    if (fb == BACK && lr == LEFT)
    {
        dirBL1 = 0;
        dirBL2 = 0;
        dirBR1 = 0;
        dirBR2 = 1;
        dirFL1 = 0;
        dirFL2 = 1;
        dirFR1 = 0;
        dirFR2 = 0;
    }
    if (fb == BACK && lr == RIGHT)
    {
        dirBL1 = 0;
        dirBL2 = 1;
        dirBR1 = 0;
        dirBR2 = 0;
        dirFL1 = 0;
        dirFL2 = 0;
        dirFR1 = 0;
        dirFR2 = 1;
    }

    //robustesse
    if (time < 0)
    {
        powerAll(pwm);
        return;
    }

    //histoire de donner un peu de jus aux moteurs pour les lancer
    powerAll(500);
    wait(0.05);

    powerAll(pwm);;
    float tt = 1.0 * time;
    wait(tt);
    shutdownM();
}
void go(int start, int end, int time, int dir)
{
    // BR=999, BL=998, FR=997, FL=996
    endMatchIfNeeded();
    int nbPas = 20;

    //   1=Straight   2=Back   3=Left   4=Right
    if (dir == 1)
    {
        dirBL1 = 1;
        dirBL2 = 0;
        dirBR1 = 1;
        dirBR2 = 0;
        dirFL1 = 1;
        dirFL2 = 0;
        dirFR1 = 1;
        dirFR2 = 0;
    }

    if (dir == 2)
    {
        dirBL1 = 0;
        dirBL2 = 1;
        dirBR1 = 0;
        dirBR2 = 1;
        dirFL1 = 0;
        dirFL2 = 1;
        dirFR1 = 0;
        dirFR2 = 1;
    }
    if (dir == 3)
    {
        dirBL1 = 1;
        dirBL2 = 0;
        dirBR1 = 0;
        dirBR2 = 1;
        dirFL1 = 0;
        dirFL2 = 1;
        dirFR1 = 1;
        dirFR2 = 0;
    }
    if (dir == 4)
    {
        dirBL1 = 0;
        dirBL2 = 1;
        dirBR1 = 1;
        dirBR2 = 0;
        dirFL1 = 1;
        dirFL2 = 0;
        dirFR1 = 0;
        dirFR2 = 1;
    }

    //robustesse
    if (time == 0)
    {
        powerAll(end);
        return;
    }
    start = min(start, end);
    end = max(start, end);

    //histoire de donner un peu de jus aux moteurs pour les lancer
    powerAll(500);
    wait(0.05);

    int power = start;
    while (power < end)
    {
        endMatchIfNeeded();
        powerAll(power += (end - start) / nbPas);
        float tt = 1.0 * time / nbPas;
        wait(tt);
    }
}

void stop(int start, int end, int time)
{
    //decelerate from power 'start' to 'end' in 'time' seconds
    int nbPas = 20;

    //robustesse
    start = max(start, end);
    end = min(start, end);

    int power = start;
    while (power > end)
    {
        endMatchIfNeeded();
        powerAll(power += (end - start) / nbPas);
        float tt = 1.0 * time / nbPas;
        wait(tt);
    }

    hardStop();
}

void waitstart()
{
    int cpt = 0;
    while (cpt < 3)
    {
        endMatchIfNeeded();
        if (!startButton)
            cpt++;
        else
            cpt = 0;
        wait(0.1);
    }
    t.start();
}

void waitplug()
{
    int cpt = 0;
    while (cpt < 3)
    {
        endMatchIfNeeded();
        if (startButton)
            cpt++;
        else
            cpt = 0;
        wait(0.1);
    }
}

void hardStop()
{
    powerAll(300);
    //fragment de rotation arriere pour éviter toute inertie
    dirBL1 = !dirBL1;
    dirBL2 = !dirBL2;
    dirBR1 = !dirBR1;
    dirBR2 = !dirBR2;
    dirFL1 = !dirFL1;
    dirFL2 = !dirFL2;
    dirFR1 = !dirFR1;
    dirFR2 = !dirFR2;

    //plus le robot est lourd, plus il faut augmenter le temps de ce wait
    wait(0.08);
    dirBL1 = !dirBL1;
    dirBL2 = !dirBL2;
    dirBR1 = !dirBR1;
    dirBR2 = !dirBR2;
    dirFL1 = !dirFL1;
    dirFL2 = !dirFL2;
    dirFR1 = !dirFR1;
    dirFR2 = !dirFR2;
    shutdownM();
}

void endMatchIfNeeded()
{
    float tttt;
    if(t){
        tttt = t.read();
        wait(0.01);

    } else {
      return;
    }
    if (tttt > 97)
    {
        debug("T", tttt);
        initMotors();
        setDrapeau(FLAGUP);
        wait(10000000);
    }
}

//HELPERS
int abs(int n)
{
    if (n >= 0)
        return n;
    else
        return -n;
}
int min(int a, int b, int c, int d)
{
    return min(min(a, b), min(c, d));
}
int max(int a, int b, int c, int d)
{
    return max(max(a, b), max(c, d));
}
int min(int a, int b)
{
    if (a < b)
        return a;
    return b;
}
int max(int a, int b)
{
    if (a > b)
        return a;
    return b;
}
int moymoy(int bl, int br, int fl, int fr)
{
    return (bl + br + fl + fr) / 4;
}

//DEBUG
void printCpts()
{
    debug("BR", counterBR.read());
    debug("BL", counterBL.read());
    debug("FR", counterFR.read());
    debug("FL", counterFL.read());
    wait(0.1);
}

//BEST ACTIONS
void dancecouikcouik()
{
    for (int jj = 0; jj < 8; jj++)
    {
        rotate(120, LEFT);
        powerAll(0);
        int i = 0;
        while (i < 7)
        {
            i++;
            pwmPenis.pulsewidth_us(1500);
            wait(0.09);
            pwmPenis.pulsewidth_us(900);
            wait(0.09);
        }
    }
}
