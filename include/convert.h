//
// Created by karui on 7/28/2025.
//

#ifndef CONVERT_H
#define CONVERT_H
#include <opencv2/opencv.hpp>
#include <vector>
#include <cstdint>

std::optional<std::vector<int32_t>> mat_to_argb_vector(const cv::Mat& src_mat);

cv::Mat argb_vector_to_mat(const std::vector<int32_t>& argb_pixels, const int width, const int height);

#endif //CONVERT_H
