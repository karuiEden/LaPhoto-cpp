//
// Created by karui on 7/12/2025.
//

#include "algorithm.h"
#include <cmath>



void process(const std::vector<int32_t> &srcPixels, std::vector<int32_t> &dstPixels, const int32_t width, const int32_t height, const int32_t thr,
    const int32_t innothr, const std::vector<int32_t> &rules, const std::vector<int32_t> &srcChan, const bool msbFromSrc) {
    // set up the first line
    for (int j=0; j<width; j++) {
        dstPixels[j]=((srcPixels[j] >> (8 * srcChan[2]) & 0xFF) > thr ? 0x000001 : 0)
                    +((srcPixels[j] >> (8 * srcChan[1]) & 0xFF) > thr ? 0x000100 : 0)
                    +((srcPixels[j] >> (8 * srcChan[0]) & 0xFF) > thr ? 0x010000:0);
    }
    for (int i=1; i<height; i++) {
        // apply cellular automaton rule
        int32_t p = dstPixels[(i-1) * width] * 3;
        for (int j=0; j<width; j++) {
            p = p<<1 & 0x070707;
            if (j + 1 < width) {
                p = p + dstPixels[(i-1) * width + j + 1];
            }
            dstPixels[i * width + j]=((1 << (p & 7) & rules[2]) > 0 ? 0x000001 : 0)
                                +((1 << (p >> 8 & 7) & rules[1]) > 0 ? 0x000100 : 0)
                                +((1 << (p>>16 & 7) & rules[0]) > 0 ? 0x010000 : 0);
        }
        // toggle CA bits if necessary
        for (int j = 0; j < width - 1; j++) {
            const int idx = i * width + j;
            for (int c = 0; c < 3; c++) {
                const int inpixel=srcPixels[idx] >> (8 * srcChan[2 - c]) & 0xFF;
                const int outpixel=((dstPixels[idx - 1] + dstPixels[idx] + dstPixels[idx + 1]) * 85) >> (8 * c) & 255;
                if (const int d = abs(inpixel - outpixel); d > innothr) {
                    const int mask = 0xFFFFFF - (0xFF << (8 * c));
                    dstPixels[idx]=(dstPixels[idx] & mask) + (inpixel > thr ? 1 << (8 * c) : 0);
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
