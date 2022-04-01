#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <vector>
#include <string>
#include <filesystem>

struct image
{
  std::filesystem::path image_file;
  std::vector<uint8_t> image_data;
};

[[nodiscard]] std::vector<

int main()
{

}