#ifndef _SBUS_h
#define _SBUS_h

#include <Arduino.h>
#include<SoftwareSerial.h>

const int emergancyStopPin = 38;

typedef struct
{
  float lx, ly, rx, ry;
  float FL, FR, BL, BR;
  float A, B, C, D;
} DIR;

class SBUS
{
private:
  void data_to_val_();
  void read_sentence_();
  void center_range();
  void data_encode_();

public:
  volatile bool SBUSlost = true; //trueで通信途絶により非常停止
  volatile bool emergencyStopState = false;
  int data_[26];
  int val_[18];
  long interval_;
  int count_;
  int send_data_[10];

  SBUS()
  {
    count_ = 0; //オブジェクト読み出し時の初期化
  }
  _SBUS()
  {
  }
  void loop_do(void (*move_func)());
  void print_send_data();
  DIR get_Dir()
  {
    DIR d;
    d.lx = send_data_[3];
    d.ly = send_data_[1];
    d.rx = send_data_[0];
    d.ry = send_data_[2];
    d.A = send_data_[4];
    d.B = send_data_[5];
    d.C = send_data_[6];
    d.D = send_data_[7];
    return d;
  }

  void count_init()
  {
    count_ = 0;
  }
  int get_count()
  {
    return count_;
  }
  int safe(int getData);
  int button(int num);
  void SBUSfailSafe(float A);
  void emergencyStop();
  void cancelEmergencyStop();
  void open();
};

#endif
