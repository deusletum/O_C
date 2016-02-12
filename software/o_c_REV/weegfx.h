// Copyright (c) 2016 Patrick Dowling
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef WEEGFX_H_
#define WEEGFX_H_

#include <stdint.h>
#include <string.h>

namespace weegfx {

typedef int_fast16_t coord_t;

// Quick & dirty graphics for 128 x 64 framebuffer with vertical pixels.
// - Writes to provided framebuffer
// - Interface pseudo-compatible with u8glib
// - Makes some assumptions based on fixed size and pixel orientation
class Graphics {
public:

  static const uint8_t kWidth = 128;
  static const uint8_t kHeight = 64;
  static const size_t kFrameSize = kWidth * kHeight / 8;

  void Init();

  void Begin(uint8_t *frame, bool clear_frame);
  void End();

  // Pseudo-compatible functions with u8lib
  void setDefaultBackgroundColor() { };
  void setDefaultForegroundColor() { };
  void setFont(const void *) { };

  // All Drawing is with foreground color except where otherwise noted

  inline void setPixel(coord_t x, coord_t y) __attribute__((always_inline));

  void drawRect(coord_t x, coord_t y, coord_t w, coord_t h);
  void clearRect(coord_t x, coord_t y, coord_t w, coord_t h);
  void invertRect(coord_t x, coord_t y, coord_t w, coord_t h);
  void drawFrame(coord_t x, coord_t y, coord_t w, coord_t h);

  void drawHLine(coord_t x, coord_t y, coord_t w);
  void drawVLine(coord_t x, coord_t y, coord_t h);

  void drawLine(coord_t x1, coord_t y1, coord_t x2, coord_t y2);

  void drawBitmap8(coord_t x, coord_t y, coord_t w, const uint8_t *data);

  // Beware: No clipping
  void drawCircle(coord_t center_x, coord_t center_y, coord_t r);

  void setPrintPos(coord_t x, coord_t y);

  void print(char);
  void print(int);
  void print(int, size_t width);
  void print(uint16_t, size_t width);
  void print(long);

  void pretty_print(int);
  void pretty_print(int, size_t width);

  void print(const char *);
  void drawStr(coord_t x, coord_t y, const char *str);

private:
  uint8_t *frame_;

  inline uint8_t *get_frame_ptr(const coord_t x, const coord_t y) __attribute__((always_inline));

  coord_t text_x_;
  coord_t text_y_;
};

inline void Graphics::setPixel(coord_t x, coord_t y) {
  *(get_frame_ptr(x, y)) |= (0x1 << (y & 0x7));
}

inline void Graphics::setPrintPos(coord_t x, coord_t y) {
  text_x_ = x;
  text_y_ = y;
}

inline uint8_t *Graphics::get_frame_ptr(const coord_t x, const coord_t  y) {
  return frame_ + ((y >>3) << 7) + x;
}

};

#endif // WEEGFX_H_
