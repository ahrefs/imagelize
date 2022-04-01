#ifndef IMAGELIZE_IMAGELIZE_HPP
#define IMAGELIZE_IMAGELIZE_HPP

#include <cstdint>
#include <array>
#include <vector>
#include <numeric>

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

    [[nodiscard]] float rgb_to_mono(const float *rgb)
    {
      const auto weights = std::array<float, 3>({0.2125f, 0.7154f, 0.0721f});
      auto res = 0.0f;
      for (size_t i = 0; i < 3; i++) // 3 = rgb
        res += rgb[i] * weights[i];
      return res;
    }

    [[nodiscard]] std::array<float, 3> mono_to_rgb(float mono)
    {
      const auto weights = std::array<float, 3>({0.2125f, 0.7154f, 0.0721f});
      auto res = std::array<float, 3>();
      for (size_t i = 0; i < 3; i++)
        res[i] = mono * weights[i];
      return res;
    }

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
          const auto rgb = mono_to_rgb(static_cast<float>(in[0]) * div_factor);
          for (size_t i = 0; i < 3; i++)
            out[i] = rgb[i];
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

  struct contrast_result
  {
    float rms;
    float michelson;
  };

  struct result
  {
    contrast_result contrast;
    float brightness;
    float noise;
    float sharpness;
    float blur;
  };

  namespace detail
  {
    [[nodiscard]] inline float rms_contrast(const std::vector<float> &brightness)
    {
      const auto summed = std::accumulate(brightness.begin(), brightness.end(), 0.0f);
      const auto average_brightness = summed / static_cast<float>(brightness.size());

      auto sum_part = 0.0f;
      for (const auto pixel : brightness)
        sum_part += std::pow(pixel - average_brightness, 2.0f);

      const auto inv_count = 1.0f / static_cast<float>(brightness.size());
      return std::sqrt(inv_count * sum_part);
    }

    [[nodiscard]] inline float michelson_contrast(const std::vector<float> &brightness)
    {
      auto [min_it, max_it] = std::minmax(brightness.begin(), brightness.end());
      const auto min = *min_it;
      const auto max = *max_it;
      const auto contrast = (max - min) / (min + max);
      return std::isnan(contrast) ? 0.0f : contrast;
    }

    [[nodiscard]] inline float average_brightness(const std::vector<float> &brightness)
    {
      return std::accumulate(brightness.begin(), brightness.end(), 0.0f) / static_cast<float>(brightness.size());
    }

    [[nodiscard]] inline float sharpness(const std::vector<float> &brightness, size_t width, size_t height)
    {
      const auto vertical_kernel = std::array<float, 9>({
        1.0f, 2.0f, 1.0f,
        0.0f, 0.0f, 0.0f,
        -1.0f, -2.0f, -1.0f});

      const auto horizontal_kernel = std::array<float, 9>({
        -1.0f, 0.0f, 1.0f,
        -2.0f, 0.0f, 2.0f,
        -1.0f, 0.0f, 1.0f
      });

      auto access_pixel = [&brightness, width, height](size_t x, size_t y) -> float {
        const auto idx = width * y + x;
        return brightness[idx];
      };

      // Todo: Finish implementing sobel

      return 0.0f;
    }

    [[nodiscard]] inline result analyze(const std::vector<float> &data, size_t width, size_t height)
    {
      constexpr auto channel_count = channel_count_of_format(format::RGB);
      auto brightness = std::vector<float>(data.size() / channel_count);
      for (size_t i = 0; i < data.size(); i += channel_count)
        brightness[i / channel_count] = rgb_to_mono(data.data() + i);

      auto contrast = contrast_result();
      contrast.rms = rms_contrast(brightness);
      contrast.michelson = michelson_contrast(brightness);

      return {
        .contrast = contrast,
        .brightness = average_brightness(brightness),

      };
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
