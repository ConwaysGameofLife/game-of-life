#pragma once

#include "gameconfig.h"
#include "random.h"
#include "ivlib.h"
#include "vec2d.h"
#include "iuniverse.h"

#include <iosfwd>
#include <cassert>


struct Point;

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
