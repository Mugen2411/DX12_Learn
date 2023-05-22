#pragma once

template <class T>
class Singleton {
 public:
  virtual ~Singleton() {}
  static T& GetIns() {
    static T instance;
    return instance;
  }

 protected:
  Singleton() = default;

 private:
  void operator=(const Singleton& obj) = delete;
  Singleton(const Singleton& obj) = delete;
};