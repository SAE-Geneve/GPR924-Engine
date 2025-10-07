#ifndef GPR924_ENGINE_SYSTEM_H
#define GPR924_ENGINE_SYSTEM_H

namespace common {
class SystemInterface
{
public:
  virtual ~SystemInterface() = default;
  virtual void Begin() = 0;
  virtual void End() = 0;
  virtual void Update(float dt) = 0;
};
}
#endif  // GPR924_ENGINE_SYSTEM_H
