#include "ThirdParty/imgui/imgui-SFML.h"
#include "ThirdParty/imgui/imgui.h"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include <fstream>
#include <iostream>
#include <vector>

#include "tile.h"
#include <stdint.h>

namespace cartridge {
constexpr int entrypoint{0x0100};
constexpr int logo{0x0104};
constexpr int title{0x0134};
constexpr int manufacturer_code{0x0100};
constexpr int cgb_flag{0x0100};
constexpr int new_licensee_code{0x0144};
constexpr int sgb_flag{0x0146};
constexpr int cartridge_type{0x0147};
constexpr int rom_size{0x0148};
constexpr int ram_size{0x0149};
constexpr int destination_code{0x014A};
constexpr int old_licensee_code{0x014B};
constexpr int version_number{0x014C};
constexpr int header_checksum{0x014D};
constexpr int global_checksum{0x014E};
} // namespace cartridge

int main() {

  std::vector<uint8_t> tile_data{0x3C, 0x7E, 0x42, 0x42, 0x42, 0x42,
                                 0x42, 0x42, 0x7E, 0x5E, 0x7E, 0x0A,
                                 0x7C, 0x56, 0x38, 0x7C};
  uint8_t *ptr = tile_data.data();
  cppboy::Tile tile{ptr};

  sf::Image image = tile.toSprite().getTexture()->copyToImage();
  std::cout << image.saveToFile(
                   "/home/sollyvarcoe/Projects/cppboy/data/Test.png")
            << std::endl;

  std::vector<cppboy::Tile> tiles{};

  std::cout << tile << std::endl;

  std::ifstream cartridge_file(
      "/home/sollyvarcoe/Projects/cppboy/data/tetris.gb",
      std::ios::binary | std::ios::in);

  size_t length{0};
  if (cartridge_file) {

    cartridge_file.seekg(0, std::ios::end);
    length = cartridge_file.tellg();
    cartridge_file.seekg(0, std::ios::beg);
  } else {
    throw std::runtime_error("Cannot open rom");
  }

  char *buffer = (char *)malloc(sizeof(char) * length);
  cartridge_file.read(buffer, length);
  uint8_t *cartridge_bytes = (uint8_t *)buffer;

  for (size_t i = 0; i < length - 1; i += 16) {
    tiles.emplace_back(cartridge_bytes + i);
  }

  std::cout << length << " " << cartridge::title << std::endl;
  std::string name(&buffer[cartridge::title], 8);
  std::cout << "HERE";
  std::cout << name << std::endl;
  std::cout << name << std::endl;

  sf::RenderWindow window(sf::VideoMode(1000, 1000), "");
  window.setVerticalSyncEnabled(true);
  ImGui::SFML::Init(window);

  sf::Color bgColor;

  float color[3] = {0.f, 0.f, 0.f};

  // let's use char array as buffer, see next part
  // for instructions on using std::string with ImGui
  char windowTitle[255] = "ImGui + SFML = <3";

  window.setTitle(windowTitle);
  window
      .resetGLStates(); // call it if you only draw ImGui. Otherwise not needed.
  sf::Clock deltaClock;
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      ImGui::SFML::ProcessEvent(event);

      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    ImGui::SFML::Update(window, deltaClock.restart());

    ImGui::Begin("Sample window"); // begin window

    // Background color edit
    if (ImGui::ColorEdit3("Background color", color)) {
      // this code gets called if color value changes, so
      // the background color is upgraded automatically!
      bgColor.r = static_cast<sf::Uint8>(color[0] * 255.f);
      bgColor.g = static_cast<sf::Uint8>(color[1] * 255.f);
      bgColor.b = static_cast<sf::Uint8>(color[2] * 255.f);
    }

    // Window title text edit
    ImGui::InputText("Window title", windowTitle, 255);

    if (ImGui::Button("Update window title")) {
      // this code gets if user clicks on the button
      // yes, you could have written if(ImGui::InputText(...))
      // but I do this to show how buttons work :)
      window.setTitle(windowTitle);
    }
    ImGui::End(); // end window
    ImGui::SetNextWindowSize(ImVec2(640.0f, 480.0f));
    ImGui::Begin("Sprites");

    ImGui::Image(tile.toSprite());
    // ImGui::GetWindowDrawList()->AddImage(
    //     unsigned int user_texture_id, const ImVec2 &p_min, const ImVec2
    //     &p_max);

    window.clear(bgColor); // fill background with color
    window.draw(tile.toSprite());

    int x = 0;
    int y = 0;
    for (auto &tile : tiles) {
      ImGui::SameLine();
      if (x > 600) {
        ImGui::NewLine();
        y += 16;
        if (y == 64)
          break;
        x = 0;
      }

      sf::Sprite sprite = tile.toSprite();
      sprite.setPosition(x, y);
      ImGui::Image(sprite);
      x += 16;
    }
    ImGui::End();
    ImGui::SFML::Render(window);
    window.display();
  }

  ImGui::SFML::Shutdown();
}