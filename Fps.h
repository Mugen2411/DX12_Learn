#include <math.h>
#include <chrono>
#include <thread>
#include <windows.h>

class Fps {
  //int start_time_;             //測定開始時刻
  int count_;                 //カウンタ
  float fps_;                 // fps_
  static const int cnt_to_sample = 60;    //平均を取るサンプル数
  static const int ideal_fps_ = 60;  //設定したFPS
  std::chrono::system_clock::time_point start_time_;

 public:
  Fps() {
    count_ = 0;
    fps_ = 0;
  }

  bool Update() {
    if (count_ == 0) {  // 1フレーム目なら時刻を記憶
      start_time_ = std::chrono::system_clock::now();
    }
    if (count_ == cnt_to_sample) {  // 60フレーム目なら平均を計算する
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
        std::chrono::system_clock::now() - start_time_;    // かかった時間
    int64_t waitTime = count_ * 1000 / ideal_fps_ -
                   std::chrono::duration_cast<std::chrono::milliseconds>
                       (tookTime).count();  // 待つべき時間
    if (waitTime > 0) {
      Sleep(waitTime);  //待機
    }
  }
};