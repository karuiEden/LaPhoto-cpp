//
// Created by karui on 7/28/2025.
//

#ifndef CHANNEL_MIXER_H
#define CHANNEL_MIXER_H

#include <opencv2/opencv.hpp>
#include <vector>
#include <cstdint>
// Apply for options of Channel Mixer for every channel
typedef struct ChannelMix {
    double red;
    double green;
    double blue;
    double constant;
}ChannelMix;

std::optional<cv::Mat> channel_mixer(const cv::Mat& input, const ChannelMix& r_mixer, const ChannelMix& g_mixer, const ChannelMix& b_mixer, const ChannelMix& gray_mixer, bool monochrome);

#endif //CHANNEL_MIXER_H
