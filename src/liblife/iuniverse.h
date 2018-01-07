#pragma once

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
