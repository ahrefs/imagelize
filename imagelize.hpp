#ifndef IMAGELIZE_IMAGELIZE_HPP
#define IMAGELIZE_IMAGELIZE_HPP

#include <cstdint>

namespace imagelize
{
  enum class format : uint8_t
  {
    RGBA,
    RGB,
    MONO,
  };

  namespace detail
  {
    [[nodiscard]] inline constexpr size_t channel_count_of_format(format image_format) noexcept
    {
      switch (image_format)
      {
      case format::RGBA: return 4;
      case format::RGB: return 3;
      case format::MONO: return 1;
      }
    }

    template <typename T>
    [[nodiscard]] constexpr inline float div_size() noexcept = delete;

    template <>
    [[nodiscard]] constexpr inline float div_size<float>() noexcept { return 1.0f; }

    template <>
    [[nodiscard]] constexpr inline float div_size<double>() noexcept { return 1.0f; }

    template <>
    [[nodiscard]] constexpr inline float div_size<uint8_t>() noexcept { return 255.0f; }

    template <typename T>
    [[nodiscard]] inline std::vector<float> to_internal_format(const std::vector<T> &source, format image_format, size_t width, size_t height)
    {
      const auto source_channel_count = channel_count_of_format(image_format);

      const auto theoretical_source_element_count = source_channel_count * width * height;
      if (source.size() != theoretical_source_element_count)
        throw std::runtime_error("Element count doesn't match passed count");

      constexpr auto div_factor = 1.0f / div_size<T>();

      const auto pixel_count = source.size() / source_channel_count;
      auto as_internal_format = std::vector<float>(pixel_count * channel_count_of_format(format::RGB));

      const auto write_format_to_internal = [](format source_format, const T* in, float* out){
        switch (source_format)
        {
        case format::RGBA:
          [[fallthrough]];
        case format::RGB:
          for (size_t i = 0; i < 3; i++)
            out[i] = static_cast<float>(in[i]) * div_factor;
          break;
        case format::MONO:
        {
          const auto mono_to_rgb = std::array<float, 3>({0.2125f, 0.7154f, 0.0721f});
          for (size_t i = 0; i < 3; i++)
            out[i] = static_cast<float>(in[0]) * mono_to_rgb[i] * div_factor;
        }
        }
      };

      for (size_t i = 0; i < source.size(); i += source_channel_count)
      {
        const auto internal_format_offset = (i / source_channel_count) * channel_count_of_format(format::RGB);
        write_format_to_internal(image_format, source.data() + i, as_internal_format.data() + internal_format_offset);
      }

      return as_internal_format;
    }
  }

  struct result
  {
    float contrast;
    float brightness;
    float noise;
    float sharpness;
    float blur;
  };

  namespace detail
  {
    [[nodiscard]] inline result analyze(const std::vector<float> &data, size_t width, size_t height)
    {
      return {};
    }
  }

  template <typename T>
  [[nodiscard]] inline result analyze(const std::vector<T> &data, format image_format, size_t width, size_t height)
  {
    const auto data_as_internal_format = detail::to_internal_format(data, image_format, width, height);
    return detail::analyze(data_as_internal_format, width, height);
  }

}  // namespace imagelize

#endif  // IMAGELIZE_IMAGELIZE_HPP
