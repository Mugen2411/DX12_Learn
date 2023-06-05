#include <math.h>
#include <chrono>
#include <thread>
#include <windows.h>

class Fps {
  //int start_time_;             //����J�n����
  int count_;                 //�J�E���^
  float fps_;                 // fps_
  static const int cnt_to_sample = 60;    //���ς����T���v����
  static const int ideal_fps_ = 60;  //�ݒ肵��FPS
  std::chrono::system_clock::time_point start_time_;

 public:
  Fps() {
    count_ = 0;
    fps_ = 0;
  }

  bool Update() {
    if (count_ == 0) {  // 1�t���[���ڂȂ玞�����L��
      start_time_ = std::chrono::system_clock::now();
    }
    if (count_ == cnt_to_sample) {  // 60�t���[���ڂȂ畽�ς��v�Z����
      auto t = std::chrono::system_clock::now();
      fps_ = 1000.f / (std::chrono::duration_cast<std::chrono::milliseconds>(t - start_time_).count() / (float)cnt_to_sample);
      count_ = 0;
      start_time_ = t;
    }
    count_++;
    return true;
  }

  void Draw() {
  }

  void Wait() {
    auto tookTime =
        std::chrono::system_clock::now() - start_time_;    // ������������
    int64_t waitTime = count_ * 1000 / ideal_fps_ -
                   std::chrono::duration_cast<std::chrono::milliseconds>
                       (tookTime).count();  // �҂ׂ�����
    if (waitTime > 0) {
      Sleep(waitTime);  //�ҋ@
    }
  }
};