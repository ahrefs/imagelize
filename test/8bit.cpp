#include <catch2/catch_all.hpp>

#include <imagelize.hpp>

TEST_CASE("8 bit image from memory", "[imagelize.basic.mono]")
{
  constexpr auto WIDTH = 512;
  constexpr auto HEIGHT = 512;

  auto buffer = std::vector<uint8_t>(WIDTH * HEIGHT);

  REQUIRE_NOTHROW(imagelize::analyze(buffer, imagelize::format::MONO, WIDTH, HEIGHT));
}

TEST_CASE("8 bit RGB image from memory", "[imagelize.basic.rgb]")
{
  constexpr auto WIDTH = 512;
  constexpr auto HEIGHT = 512;

  auto buffer = std::vector<uint8_t>(WIDTH * HEIGHT * 3);

  REQUIRE_NOTHROW(imagelize::analyze(buffer, imagelize::format::RGB, WIDTH, HEIGHT));
}

TEST_CASE("8 bit RGBA image from memory", "[imagelize.basic.rgba]")
{
  constexpr auto WIDTH = 512;
  constexpr auto HEIGHT = 512;

  auto buffer = std::vector<uint8_t>(WIDTH * HEIGHT * 4);

  REQUIRE_NOTHROW(imagelize::analyze(buffer, imagelize::format::RGBA, WIDTH, HEIGHT));
}

TEST_CASE("8 bit image from memory with wrong size", "[imagelize.basic.mono.fail]")
{
  constexpr auto WIDTH = 512;
  constexpr auto HEIGHT = 512;

  constexpr auto WRONG_WIDTH = 512;
  constexpr auto WRONG_HEIGHT = 513;

  auto buffer = std::vector<uint8_t>(WRONG_WIDTH * WRONG_HEIGHT);

  REQUIRE_THROWS(imagelize::analyze(buffer, imagelize::format::MONO, WIDTH, HEIGHT));
}

TEST_CASE("8 bit RGB image from memory with wrong size", "[imagelize.basic.rgb.fail]")
{
  constexpr auto WIDTH = 512;
  constexpr auto HEIGHT = 512;

  constexpr auto WRONG_WIDTH = 512;
  constexpr auto WRONG_HEIGHT = 513;

  auto buffer = std::vector<uint8_t>(WRONG_WIDTH * WRONG_HEIGHT * 3);

  REQUIRE_THROWS(imagelize::analyze(buffer, imagelize::format::RGB, WIDTH, HEIGHT));
}

TEST_CASE("8 bit RGBA image from memory with wrong size", "[imagelize.basic.rgba.fail]")
{
  constexpr auto WIDTH = 512;
  constexpr auto HEIGHT = 512;

  constexpr auto WRONG_WIDTH = 512;
  constexpr auto WRONG_HEIGHT = 513;

  auto buffer = std::vector<uint8_t>(WRONG_WIDTH * WRONG_HEIGHT * 4);

  REQUIRE_THROWS(imagelize::analyze(buffer, imagelize::format::RGBA, WIDTH, HEIGHT));
}
