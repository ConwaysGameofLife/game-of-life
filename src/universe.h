#pragma once

#include "gameconfig.h"
#include "random.h"
#include "ivlib.h"
#include "vec2d.h"

#include <iosfwd>
#include <cassert>


struct Point;

class IUniverse {
public:
  virtual ~IUniverse() {}
  virtual std::string name() const = 0;
  virtual void next() = 0;
  virtual void add(int x, int y) = 0;
  virtual int width() const = 0;
  virtual int height() const = 0;
  virtual int size() const = 0;
  virtual const uint8_t* render() const = 0;

  const static uint8_t ALIVE_COLOR = 0xFF;
};

template <typename T> std::unique_ptr<T> bigBang(int width, int height) {
  std::unique_ptr<T> u = std::make_unique<T>(width, height);
  for (int i = 0; i < width * height * GameConfig::BORN_RATE; ++i) {
    int x = Random::next<int>(width - 1);
    int y = Random::next<int>(height - 1);
    u->add(x, y);
  }
  return u;
}

class AbstractCpuUniverse : public IUniverse {
public:
  AbstractCpuUniverse();
  AbstractCpuUniverse(int width, int height);
  AbstractCpuUniverse &operator=(AbstractCpuUniverse &&rhs);

  ~AbstractCpuUniverse() override;

  int size() const final;

  const uint8_t* render() const final;

  void next() final;

  void add(int x, int y) final;

  int width() const final;
  int height() const final;

protected:
  virtual void next(Vec2d &src, Vec2d &dst) const = 0;

  void nextLoop(Vec2d &src, Vec2d &dst) const;
  void nextLoopOmp(Vec2d &src, Vec2d &dst) const;
  void nextIpp(Vec2d &src, Vec2d &dst) const;
  void nextIppTbb(Vec2d &src, Vec2d &dst) const;
  void nextIppOmp(Vec2d &src, Vec2d &dst) const;
  void nextAvx(Vec2d &src, Vec2d &dst) const;
  void nextAvxOmp(Vec2d &src, Vec2d &dst) const;
  void next(uint8_t *src, int srcStride, uint8_t *dest, int destStride,
            const std::pair<int, int> &roi) const;

  void init(Vec2d &c, int width, int height) const;
  Vec2d &src();
  const Vec2d &src() const;
  Vec2d &dst();
  const Vec2d &dst() const;
  void flip();

  int current_ = 0;
  Vec2d channels_[2];
};

class CpuLoopUniverse : public AbstractCpuUniverse {
public:
  CpuLoopUniverse(int width, int height) : AbstractCpuUniverse(width, height) {}

  std::string name() const { return "CPU Loop"; }

protected:
  void next(Vec2d &src, Vec2d &dst) const final {
    nextLoop(src, dst);
  }
};

class CpuLoopOmpUniverse : public AbstractCpuUniverse {
public:
  CpuLoopOmpUniverse(int width, int height)
      : AbstractCpuUniverse(width, height) {}

  std::string name() const { return "CPU Loop with OpenMP"; }

protected:
  void next(Vec2d &src, Vec2d &dst) const final {
    nextLoopOmp(src, dst);
  }
};

class CpuIppUniverse : public AbstractCpuUniverse {
public:
  CpuIppUniverse(int width, int height) : AbstractCpuUniverse(width, height) {}

  std::string name() const { return "CPU IPP"; }

protected:
  void next(Vec2d &src, Vec2d &dst) const final {
    nextIpp(src, dst);
  }
};

class CpuIppOmpUniverse : public AbstractCpuUniverse {
public:
  CpuIppOmpUniverse(int width, int height)
      : AbstractCpuUniverse(width, height) {}

  std::string name() const { return "CPU IPP with OpenMP"; }

protected:
  void next(Vec2d &src, Vec2d &dst) const final {
    nextIppOmp(src, dst);
  }
};

class CpuIppTbbUniverse : public AbstractCpuUniverse {
public:
  CpuIppTbbUniverse(int width, int height)
      : AbstractCpuUniverse(width, height) {}

  std::string name() const { return "CPU IPP with TBB"; }

protected:
  void next(Vec2d &src, Vec2d &dst) const final {
    nextIppTbb(src, dst);
  }
};

class CpuAvxUniverse : public AbstractCpuUniverse {
public:
  CpuAvxUniverse(int width, int height) : AbstractCpuUniverse(width, height) {
    sum = aligned_malloc<uint8_t>(width - 2, 32);
  }
  ~CpuAvxUniverse() final { aligned_free(sum); }

  std::string name() const { return "CPU AVX"; }

  void next(int width, int height, uint8_t *src, int srcStride, int srcInc,
	  uint8_t *dst, int dstStride, int dstInc) const;

protected:
  void next(Vec2d &src, Vec2d &dst) const final;

private:
  uint8_t *sum;
};

class CpuAvxOmpUniverse : public AbstractCpuUniverse {
public:
  CpuAvxOmpUniverse(int width, int height)
      : AbstractCpuUniverse(width, height) {}

  std::string name() const { return "CPU AVX OpenMP"; }

protected:
  void next(Vec2d &src, Vec2d &dst) const final {
    nextAvxOmp(src, dst);
  }
};
