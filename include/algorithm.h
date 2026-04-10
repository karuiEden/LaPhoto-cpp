//
// Created by karui on 7/12/2025.
//

#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <cmath>
#include <cstdint>
#include <vector>

void process(const std::vector<int32_t> &srcPixels, std::vector<int32_t> &dstPixels, const int32_t width, const int32_t height, const int32_t thr,
    const int32_t innothr, const std::vector<int32_t> &rules, const std::vector<int32_t> &srcChan, const bool msbFromSrc);

#endif //ALGORITHM_H
