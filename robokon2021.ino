#include "sbus.hpp"
#include <SoftwareSerial.h>
// #include <algorithm>
float s_d[10];
SoftwareSerial motor[4] = {
    SoftwareSerial(3, 2),
    SoftwareSerial(6, 5),
    SoftwareSerial(8, 7),
    SoftwareSerial(12, 11)};

void printDIRData(DIR Dir)
{
    Serial.print(Dir.lx);
    Serial.print("  ");
    Serial.print(Dir.ly);
    Serial.print("  ");
    Serial.print(Dir.rx);
    Serial.print("  ");
    Serial.print(Dir.ry);
    Serial.print("   ");
    Serial.print(Dir.A);
    Serial.print("   ");
    Serial.print(Dir.B);
    Serial.print("   ");
    Serial.print(Dir.C);
    Serial.print("   ");
    Serial.println(Dir.D);
    // omunioutput(Dir);
}

SBUS Sbus;
// DATA data;

void setup()
{
    Serial.begin(115200);
    Serial1.begin(100000);
    motor[0].begin(9600);
    motor[1].begin(9600);
    motor[2].begin(9600);
    motor[3].begin(9600);
    // pinMode(52, OUTPUT);
    // digitalWrite(52, HIGH);
    Sbus.emergencyStop();

    TCCR1B = (TCCR1B & 0b11111000) | 2;
    TCCR3B &= B11111000;
    TCCR3B |= B00000010; // rを1に設定 fc/(2*255*1)Hzになる
    TCCR4B &= B11111000;
    TCCR4B |= B00000010; // rを1に設定 fc/(2*255*1)Hzになる
}

void loop()
{
    Sbus.loop_do(do_something);
    // omunioutput();
}
void do_something()
{
    DIR Dir;
    DIR *pDir;

    Dir = Sbus.get_Dir();
    Sbus.SBUSfailSafe(Dir.A);

    printDIRData(Dir);
}

void omunioutput(DIR Dir)
{
    float sita = kakudo(Dir);
    kaiten(sita, Dir);
}

void kaiten(const float &sita, DIR Dir)
{
    // t=右上
    // s=右下
    float migiue = -(Dir.ly + Dir.lx) / 2;    //-127~127
    float migisita = -(-Dir.lx + Dir.ly) / 2; //-127~127
    float hidariue = -migisita;
    float hidarisita = -migiue;

    migiue += sita;
    migisita += sita;
    hidariue += sita;
    hidarisita += sita;

    migiue = marumeru(migiue);
    migisita = marumeru(migisita);
    hidariue = marumeru(hidariue);
    hidarisita = marumeru(hidarisita);

    migiue = map(migiue, -127, 127, 1, 127);
    migisita = map(migisita, -127, 127, 1, 127);
    hidariue = map(hidariue, -127, 127, 128, 255);
    hidarisita = map(hidarisita, -127, 127, 128, 255);

    //↖ ↗ ↙ ↘
    // Serial.print(migiue);
    // Serial.print("  ");
    // Serial.print(migisita);
    // Serial.print("  ");
    // Serial.print(hidariue);
    // Serial.print("  ");
    // Serial.println(hidarisita);

    motor[0].write((int)migiue);
    motor[0].write((int)hidariue);
    motor[1].write((int)migisita);
    motor[1].write((int)hidarisita);
}

int marumeru(float value)
{
    value = min(max(value, -127), 127);
    return value;
}

float kakudo(DIR Dir)
{
    return map(Dir.rx, -255, 255, -128, 128);
}