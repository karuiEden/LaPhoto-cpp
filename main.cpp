#include <opencv2/opencv.hpp>
#include <vector>

#include "include/algorithm.h"
#include "include/channel_mixer.h"
#include "include/convert.h"
using namespace std;


int main(const int32_t argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <image_path>" << "<output_path>" << std::endl;
        return -1;
    }
    const string image_path = argv[1];
    const string output_path = argc == 3 ? argv[2] : "../output/";
    cv::Mat image = cv::imread(image_path, cv::IMREAD_UNCHANGED); // Загрузка с альфой, если есть
    if (image.empty()) {
        std::cerr << "Could not open or find the image." << std::endl;
        return -1;
    }
    ChannelMix b = {1.0, 1.0, 1.0, 1.0};
    ChannelMix g = {1.0, 1.0, 1.0, 1.0};
    ChannelMix r = {1.0, 1.0, 1.0, 1.0};
    ChannelMix gray = {-0.7, 2.0, -0.3, 0.0};
    auto mix_image_opt = channel_mixer(image, r, g, b, gray, true);
    image.release();
    if (!mix_image_opt) {
        std::cerr << "Couldn't mixer of channel." << std::endl;
        return -1;
    }
    auto& mix_image = mix_image_opt.value();
    mix_image.convertTo(mix_image, CV_8UC4);
    cv::imwrite("../output/rose_gray.JPG", mix_image);
    // mix_image.release();
    const int width = mix_image.cols;
    const int height = mix_image.rows;

    const auto src_pixels_argb_opt = mat_to_argb_vector(mix_image);
    if (!src_pixels_argb_opt) {
        std::cerr << "Could not convert image to ARGB." << std::endl;
        return -1;
    }

    const auto& src_pixels_argb = src_pixels_argb_opt.value();


    // Пример параметров (замените на ваши реальные)
    const int32_t thr = 78;
    const int32_t innothr = 50;
    const std::vector rules = {30,110,30}; // Ваши правила
    const std::vector srcChan = {2, 1, 0}; // R, G, B
    const bool msbFromSrc = true;

    std::vector<int32_t> dst_pixels_argb(width * height);
    // Вызов вашей функции process
    for (int i = 0; i < 10; i++) {
        auto res = dst_pixels_argb;
        process(src_pixels_argb, res, width, height, thr, innothr, rules, srcChan, msbFromSrc);
    }

    std::chrono::microseconds time{};
    for (int i = 0; i < 20; ++i) {
        std::vector<int32_t> res(dst_pixels_argb.size());
        auto start = std::chrono::high_resolution_clock::now();
        process(src_pixels_argb, res, width, height, thr, innothr, rules, srcChan, msbFromSrc);
        auto end = std::chrono::high_resolution_clock::now();
        time += std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        if (i == 19) {
            dst_pixels_argb = res;
        }
    }
    std::cout << "Time: " << time / 20  << std::endl;

    // Конвертация обратно в cv::Mat для сохранения
    cv::Mat output_image = argb_vector_to_mat(dst_pixels_argb, width, height);
    cv::imwrite(output_path, output_image); // Сохранение в PNG (поддерживает альфу)
    output_image.release();
    return 0;
}
