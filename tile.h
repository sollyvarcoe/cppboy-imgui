#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdint.h>
#include <vector>
namespace cppboy {

class Tile {
public:
  Tile(uint8_t *location);

  friend std::ostream &operator<<(std::ostream &out, Tile const &tile) {
    size_t count{0};
    for (const auto &pixel : tile.pixels) {
      if (count == 8) {
        out << "\n";
        count = 0;
      }

      out << +pixel << " ";

      count++;
    }
    return out;
  }

  sf::Sprite toSprite();

private:
  uint8_t size_bytes{16};
  uint8_t width_px{8};
  uint8_t height_px{8};
  sf::Texture texture{};
  std::vector<uint32_t> pixels = std::vector<uint32_t>(width_px * height_px);
  void set_pixel(uint8_t x, uint8_t y, uint8_t value);
};

} // namespace cppboy