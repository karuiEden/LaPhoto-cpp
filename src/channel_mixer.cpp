//
// Created by karui on 7/28/2025.
//
#include "channel_mixer.h"

#include <opencv2/opencv.hpp>
#include <vector>
#include <cstdint>

//Make mixer of channels. If input is empty, function returns nullopt.
std::optional<cv::Mat> channel_mixer(const cv::Mat &input, const ChannelMix &r_mixer, const ChannelMix &g_mixer,
                                     const ChannelMix &b_mixer, const ChannelMix &gray_mixer, bool monochrome) {
    if (input.empty()) {
        return std::nullopt;
    }
    std::vector<cv::Mat> channels;
    cv::split(input, channels);
    cv::Mat R, G, B;
    channels[0].convertTo(B, CV_32F);
    channels[1].convertTo(G, CV_32F);
    channels[2].convertTo(R, CV_32F);
    cv::Mat output;
    if (monochrome) {
        cv::Mat gray = R * gray_mixer.red + G * gray_mixer.green + B * gray_mixer.blue + gray_mixer.constant;
        gray = cv::min(cv::max(0, gray), 255);
        gray.convertTo(output, CV_8U);
        cv::cvtColor(gray, output, cv::COLOR_GRAY2BGRA);
    } else {
        cv::Mat outputRed = R * r_mixer.red + G * r_mixer.green + B * r_mixer.blue + r_mixer.constant;
        cv::Mat outputGreen = R * g_mixer.red + G * g_mixer.green + B * g_mixer.blue + g_mixer.constant;
        cv::Mat outputBlue = R * b_mixer.red + G * b_mixer.green + B * b_mixer.blue + b_mixer.constant;
        cv::Mat UB, UG, UR;
        outputBlue.convertTo(UB, CV_8U);
        outputGreen.convertTo(UG, CV_8U);
        outputRed.convertTo(UR, CV_8U);
        std::vector outputBGR = {UB, UG, UR};
        cv::merge(outputBGR, output);
        cv::cvtColor(output, output, cv::COLOR_BGR2BGRA);
    }
    return output;
}
