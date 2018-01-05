#pragma once

#include "gameconfig.h"
#include "random.h"

#include <memory>
#include <iosfwd>
#include <cassert>

#include "ivlib.h"

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

class Channel {
    using T = uint8_t;
public:
    Channel() {}
    Channel(int width, int height) : _w(width), _h(height) { init(); }

    Channel& operator=(Channel&& c) {
        _w = c.width();
        _h = c.height();
        _data.reset(c._data.release());
        c._w = 0;
        c._h = 0;
        return *this;
    }

    int width() const { return _w; }
    int height() const { return _h; }
    size_t getRowBytes() const { return _w * sizeof(T); }
    size_t getIncrement() const { return sizeof(T); }

    T* data() { return _data.get(); }

    void setValue(int x, int y, T value) {
        assert(x < _w);
        assert(y < _h);
        auto i = getRowBytes() * y * getIncrement() + x * getIncrement();
        _data.get()[i] = value;
    }

private:
    void init() {
        if (size() > 0) {
            _data = std::make_unique<T[]>(_w * _h);
            fill(0);
        }
    }

    void fill(T v) {
        std::fill(_data.get(), _data.get() + _w * _h, v);
    }

    size_t size() const {
        return _h * getRowBytes();
    }

private:
    std::unique_ptr<T[]> _data;
    int _w = 0;
    int _h = 0;
};

class AbstractCpuUniverse : public IUniverse {
public:
  AbstractCpuUniverse();
  AbstractCpuUniverse(int width, int height);
  AbstractCpuUniverse(AbstractCpuUniverse &&u);
  AbstractCpuUniverse &operator=(AbstractCpuUniverse &&u);

  ~AbstractCpuUniverse() override;

  int size() const final;

  const uint8_t* render() const final { return texture(); }

  void next() final;

  void add(int x, int y) final;

  int width() const final;
  int height() const final;

protected:
  virtual void next(Channel &src, Channel &dst) const = 0;

  const uint8_t* texture() const;
  void nextLoop(Channel &src, Channel &dst) const;
  void nextLoopOmp(Channel &src, Channel &dst) const;
  void nextIpp(Channel &src, Channel &dst) const;
  void nextIppTbb(Channel &src, Channel &dst) const;
  void nextIppOmp(Channel &src, Channel &dst) const;
  void nextAvx(Channel &src, Channel &dst) const;
  void nextAvxOmp(Channel &src, Channel &dst) const;
  void next(uint8_t *src, int srcStride, uint8_t *dest, int destStride,
            const std::pair<int, int> &roi) const;

  struct Data;
  std::unique_ptr<Data> d;
};

class CpuLoopUniverse : public AbstractCpuUniverse {
public:
  CpuLoopUniverse(int width, int height) : AbstractCpuUniverse(width, height) {}

  std::string name() const { return "CPU Loop"; }

protected:
  void next(Channel &src, Channel &dst) const final {
    nextLoop(src, dst);
  }
};

class CpuLoopOmpUniverse : public AbstractCpuUniverse {
public:
  CpuLoopOmpUniverse(int width, int height)
      : AbstractCpuUniverse(width, height) {}

  std::string name() const { return "CPU Loop with OpenMP"; }

protected:
  void next(Channel &src, Channel &dst) const final {
    nextLoopOmp(src, dst);
  }
};

class CpuIppUniverse : public AbstractCpuUniverse {
public:
  CpuIppUniverse(int width, int height) : AbstractCpuUniverse(width, height) {}

  std::string name() const { return "CPU IPP"; }

protected:
  void next(Channel &src, Channel &dst) const final {
    nextIpp(src, dst);
  }
};

class CpuIppOmpUniverse : public AbstractCpuUniverse {
public:
  CpuIppOmpUniverse(int width, int height)
      : AbstractCpuUniverse(width, height) {}

  std::string name() const { return "CPU IPP with OpenMP"; }

protected:
  void next(Channel &src, Channel &dst) const final {
    nextIppOmp(src, dst);
  }
};

class CpuIppTbbUniverse : public AbstractCpuUniverse {
public:
  CpuIppTbbUniverse(int width, int height)
      : AbstractCpuUniverse(width, height) {}

  std::string name() const { return "CPU IPP with TBB"; }

protected:
  void next(Channel &src, Channel &dst) const final {
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
  void next(Channel &src, Channel &dst) const final;

private:
  uint8_t *sum;
};

class CpuAvxOmpUniverse : public AbstractCpuUniverse {
public:
  CpuAvxOmpUniverse(int width, int height)
      : AbstractCpuUniverse(width, height) {}

  std::string name() const { return "CPU AVX OpenMP"; }

protected:
  void next(Channel &src, Channel &dst) const final {
    nextAvxOmp(src, dst);
  }
};
