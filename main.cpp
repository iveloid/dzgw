#include <poppler/cpp/poppler-document.h>
#include <iostream>

int main() {
    auto doc = poppler::document::load_from_file("test.pdf");
    if (!doc) {
        std::cerr << "Failed to load PDF file." << std::endl;
        return 1;
    }
    std::cout << "PDF loaded successfully." << std::endl;
    return 0;
}

/*#include <iostream>
#include <vector>
#include <filesystem>
#include <poppler/document.h>
#include <poppler/page.h>
#include <poppler/page-renderer.h>
#include <opencv2/opencv.hpp>

namespace fs = std::filesystem;

// 提取 PDF 第一页为 OpenCV 图像
cv::Mat extract_pdf_thumbnail(const std::string& pdf_path, double dpi = 72.0) {
    // 加载 PDF 文件
    auto doc = poppler::document::load_from_file(pdf_path);
    if (!doc || doc->pages() == 0) {
        throw std::runtime_error("Failed to load PDF: " + pdf_path);
    }

    // 渲染第一页
    poppler::page_renderer renderer;
    renderer.set_render_hint(poppler::page_renderer::antialiasing, true);
    renderer.set_render_hint(poppler::page_renderer::text_antialiasing, true);

    poppler::page* page = doc->create_page(0);
    poppler::image image = renderer.render_page(page, dpi, dpi);
    delete page;

    // 转换为 OpenCV 的 Mat 格式 (BGR)
    cv::Mat cv_image(image.height(), image.width(), CV_8UC3);
    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            poppler::image::pixel_data pixel = image.pixel(x, y);
            cv_image.at<cv::Vec3b>(y, x) = cv::Vec3b(pixel.blue, pixel.green, pixel.red);
        }
    }

    return cv_image;
}

// 合并缩略图为一个图像网格
cv::Mat merge_thumbnails(const std::vector<cv::Mat>& thumbnails, int cols = 4, int thumbnail_size = 200) {
    if (thumbnails.empty()) {
        throw std::runtime_error("No thumbnails to merge!");
    }

    // 计算网格布局
    int rows = (thumbnails.size() + cols - 1) / cols;
    cv::Mat grid(rows * thumbnail_size, cols * thumbnail_size, CV_8UC3, cv::Scalar(255, 255, 255));

    for (size_t i = 0; i < thumbnails.size(); ++i) {
        // 调整缩略图大小
        cv::Mat resized;
        cv::resize(thumbnails[i], resized, cv::Size(thumbnail_size, thumbnail_size));

        // 计算位置
        int x = (i % cols) * thumbnail_size;
        int y = (i / cols) * thumbnail_size;

        // 将缩略图复制到网格中
        resized.copyTo(grid(cv::Rect(x, y, thumbnail_size, thumbnail_size)));
    }

    return grid;
}

int main() {
    try {
        std::vector<cv::Mat> thumbnails;
        std::string pdf_dir = "pdf_files";  // PDF 文件所在目录

        // 遍历目录中的 PDF 文件
        for (const auto& entry : fs::directory_iterator(pdf_dir)) {
            if (entry.path().extension() == ".pdf") {
                std::string pdf_path = entry.path().string();
                std::cout << "Processing: " << pdf_path << std::endl;
                cv::Mat thumbnail = extract_pdf_thumbnail(pdf_path, 150.0);  // 150 DPI
                thumbnails.push_back(thumbnail);
            }
        }

        // 合并缩略图
        if (!thumbnails.empty()) {
            cv::Mat grid = merge_thumbnails(thumbnails, 4, 200);
            cv::imwrite("thumbnail_grid.jpg", grid);
            std::cout << "Saved thumbnail_grid.jpg" << std::endl;
        } else {
            std::cerr << "No PDF files found in directory: " << pdf_dir << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}*/