#include "tile.h"

namespace cppboy {

// Renderer expects pixels in RGBA format
uint32_t colour_map[4]{0x000000FF, 0xD2D7D3FF, 0x6C7A89FF, 0xFFFFFFFF};

uint8_t extract_bit(uint8_t byte, uint8_t location) {
  return (byte & (0b10000000 >> location)) > 0 ? 1 : 0;
}

void Tile::set_pixel(uint8_t x, uint8_t y, uint8_t value) {

  size_t location = x + width_px * y;
  pixels[location] = colour_map[value];
}

Tile::Tile(uint8_t *location) {
  texture.create(8, 8);
  for (uint8_t y = 0; y < height_px; y++) {

    uint8_t low_byte{location[y * 2]};
    uint8_t high_byte{location[y * 2 + 1]};

    for (uint8_t x = 0; x < width_px; x++) {

      uint8_t low_pixel{extract_bit(low_byte, x)};
      uint8_t high_pixel{extract_bit(high_byte, x)};

      set_pixel(x, y, high_pixel * 2 + low_pixel);
    }
  }
}

sf::Sprite Tile::toSprite() {
  texture.update(reinterpret_cast<uint8_t *>(pixels.data()));
  sf::Sprite sprite{};
  sprite.setTexture(texture);
  sprite.setScale(sf::Vector2f(2.f, 2.f));
  return sprite;
}

} // namespace cppboy