//
// Created by karui on 7/28/2025.
//

#include "convert.h"
#include <opencv2/opencv.hpp>
#include <vector>

std::optional<std::vector<int32_t>> mat_to_argb_vector(const cv::Mat& src_mat) {
    if (src_mat.empty()) {
        return std::nullopt;
    }
    std::vector<int32_t> argb_pixels(src_mat.rows * src_mat.cols);
    for (int y = 0; y < src_mat.rows; ++y) {
        for (int x = 0; x < src_mat.cols; ++x) {
            // Пиксели в OpenCV Mat CV_8UC4 обычно B G R A
            const auto& pixel = src_mat.at<cv::Vec4b>(y, x);
            uint8_t b = pixel[0];
            uint8_t g = pixel[1];
            uint8_t r = pixel[2];
            uint8_t a = pixel[3];
            uint32_t argb_val = (static_cast<uint32_t>(a) << 24) |
                                (static_cast<uint32_t>(r) << 16) |
                                (static_cast<uint32_t>(g) << 8) |
                                (static_cast<uint32_t>(b));

            argb_pixels[y * src_mat.cols + x] = static_cast<int32_t>(argb_val);
        }
    }
    return argb_pixels;
}

// Вспомогательная функция для конвертации std::vector (ARGB) обратно в cv::Mat (BGRA)
cv::Mat argb_vector_to_mat(const std::vector<int32_t>& argb_pixels, const int width, const int height) {
    cv::Mat dst_mat(height, width, CV_8UC4); // CV_8UC4 для BGRA
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            // Здесь dstPixels[i] может быть int32_t, поэтому используем uint32_t для извлечения битов
            const auto argb_val = static_cast<uint32_t>(argb_pixels[y * width + x]);

            const uint8_t a = (argb_val >> 24) & 0xFF;
            const uint8_t r = (argb_val >> 16) & 0xFF;
            const uint8_t g = (argb_val >> 8) & 0xFF;
            const uint8_t b = (argb_val >> 0) & 0xFF;

            const cv::Vec4b pixel(b, g, r, a); // Порядок BGRA для OpenCV Mat
            dst_mat.at<cv::Vec4b>(y, x) = pixel;
        }
    }
    return dst_mat;
}
