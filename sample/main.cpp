#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <vector>
#include <string>
#include <filesystem>
#include <iostream>

#include <imagelize.hpp>

struct image
{
  std::filesystem::path image_file;
  std::vector<uint8_t> image_data;
  size_t width;
  size_t height;
  imagelize::format format;
};

[[nodiscard]] std::vector<image> load_test_images(const std::filesystem::path &directory)
{
  auto images = std::vector<image>();
  // Recursively go through directory and load images
  for (const auto &entry : std::filesystem::recursive_directory_iterator(directory))
  {
    if (!entry.is_regular_file())
      continue;

    int comp;
    int width;
    int height;
    auto data = stbi_load(entry.path().string().c_str(), &width, &height, &comp, 0);
    if (data == nullptr)
      continue;

    auto as_vec = std::vector<uint8_t>(width * height * comp);
    for (size_t i = 0; i < width * height; i++)
      as_vec[i] = data[i];
    stbi_image_free(data);

    auto format = imagelize::format::RGBA;
    if (comp == 4)
      format = imagelize::format::RGBA;
    else if (comp == 3)
      format = imagelize::format::RGB;
    else if (comp == 1)
      format = imagelize::format::MONO;
    else
      throw std::runtime_error("weird channel count");

    images.push_back(image{
      .image_file = entry.path(),
      .image_data = std::move(as_vec),
      .width = static_cast<size_t>(width),
      .height = static_cast<size_t>(height),
      .format = format
    });
  }

  return images;
}

int main()
{
  const auto images = load_test_images("./test/data/");

  auto results = std::vector<imagelize::result>(images.size());
  std::transform(images.begin(), images.end(), results.begin(), [](const image &img){
    return imagelize::analyze(img.image_data, img.format, img.width, img.height);
  });

  std::for_each(results.begin(), results.end(), [](imagelize::result result){
                  std::cout << "Image: " <<
                    result.contrast.rms << '\n' <<
                    result.contrast.michelson << '\n' <<
                    result.brightness << '\n' <<
                    result.noise << '\n' <<
                    result.blur << '\n' <<
                    result.sharpness << '\n' <<
                    std::endl;

                  });
}