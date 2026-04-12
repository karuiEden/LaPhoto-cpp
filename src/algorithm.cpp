//
// Created by karui on 7/12/2025.
//

#include "algorithm.h"
#include <cmath>


void process(const std::vector<uint32_t> &srcPixels, std::vector<uint32_t> &dstPixels, const int32_t width,
             const int32_t height, const int32_t thr,
             const int32_t innothr, const std::vector<int32_t> &rules, const std::vector<int32_t> &srcChan,
             const bool msbFromSrc) {
    uint32_t lutB[8], lutG[8], lutR[8];
    for (uint32_t s = 0; s < 8; ++s) {
        lutB[s] = rules[2] >> s & 1u ? 0x000001u : 0u;
        lutG[s] = rules[1] >> s & 1u ? 0x000100u : 0u;
        lutR[s] = rules[0] >> s & 1u ? 0x010000u : 0u;
    }
    std::vector<int32_t> tmpPixels(width * height);
    // set up the first line
    for (int j = 0; j < width; j++) {
        dstPixels[j] = ((srcPixels[j] >> (8 * srcChan[2]) & 0xFF) > thr ? 0x000001 : 0)
                       + ((srcPixels[j] >> (8 * srcChan[1]) & 0xFF) > thr ? 0x000100 : 0)
                       + ((srcPixels[j] >> (8 * srcChan[0]) & 0xFF) > thr ? 0x010000 : 0);
    }
    for (int i = 1; i < height; i++) {
        const auto* prev = &dstPixels[(i - 1) * width];
        auto *cur = &dstPixels[i * width];

        for (int j = 0; j < width; j++) {
            const auto left = j > 0 ? prev[j - 1] : 0;
            const auto center = prev[j];
            const auto right = j + 1 < width ? prev[j + 1] : 0;
            const uint32_t b = (left & 1 ) << 2 | (center & 1) << 1 | right & 1;
            const uint32_t g = (left >> 8 & 1) << 2 | (center >> 8 & 1) << 1 | right >> 8 & 1;
            const uint32_t r = ((left >> 16) & 1) << 2 | ((center >> 16) & 1) << 1 | (right >> 16) & 1;
            cur[j] = lutB[b] | lutG[g] | lutR[r];
        }
        // toggle CA bits if necessary
        for (int j = 0; j < width - 1; j++) {
            const int idx = i * width + j;
            for (int c = 0; c < 3; c++) {
                const int inpixel = srcPixels[idx] >> (8 * srcChan[2 - c]) & 0xFF;
                const int outpixel = ((dstPixels[idx - 1] + dstPixels[idx] + dstPixels[idx + 1]) * 85) >> (8 * c) & 255;
                if (const int d = abs(inpixel - outpixel); d > innothr) {
                    const int mask = 0xFFFFFF - (0xFF << (8 * c));
                    dstPixels[idx] = (dstPixels[idx] & mask) + (inpixel > thr ? 1 << (8 * c) : 0);
                }
            }
        }
    }
    // restore colors
    for (int i = 0; i < width * height; i++) {
        dstPixels[i] = dstPixels[i] * 0xFF + 0xFF000000;
        // toggle output bits if desired
        if (msbFromSrc) {
            dstPixels[i] = dstPixels[i] & (srcPixels[i] | 0xFF7F7F7F);
        }
    }
}
