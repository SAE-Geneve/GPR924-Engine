#ifndef GPR924_ENGINE_OBSERVER_H
#define GPR924_ENGINE_OBSERVER_H

#include <span>
#include <stdexcept>
#include <vector>

namespace common {
template <typename T>
class ObserverSubject {
 public:
  static void AddObserver(T* observer) {
    auto it = std::ranges::find(observers_, nullptr);
    if (it == observers_.end()) {
      observers_.push_back(observer);
    } else {
      *it = observer;
    }
  }
  static void RemoveObserver(T* observer) {
    auto it = std::ranges::find(observers_, observer);
    if (it != observers_.end()) {
      *it = nullptr;
    } else {
      throw std::runtime_error("Observer does not exist");
    }
  }
  static std::span<T*> GetObservers() { return observers_; }

 private:
  inline static std::vector<T*> observers_;
};
}  // namespace common
#endif  // GPR924_ENGINE_OBSERVER_H
