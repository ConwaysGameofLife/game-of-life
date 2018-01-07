#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include <cassert>

class Vec2d {
    using T = uint8_t;
public:
    Vec2d() {}
    Vec2d(int width, int height) : _w(width), _h(height) { init(); }

    Vec2d& operator=(Vec2d&& c) {
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
    const T* data() const { return _data.get(); }

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

    friend void swap(Vec2d& lhs, Vec2d& rhs) {
        using std::swap;
        swap(lhs._data, rhs._data);
        swap(lhs._w, rhs._w);
        swap(lhs._h, rhs._w);
    }
};
