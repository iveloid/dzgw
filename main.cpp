#include <poppler/cpp/poppler-document.h>
#include <poppler/cpp/poppler-page.h>
#include <poppler/cpp/poppler-page-renderer.h>
#include <iostream>
#include <fstream>
#include <png.h>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

// 保存为 PNG 文件的辅助函数
void save_as_png(const std::string& filename, const unsigned char* data, int width, int height, int bytes_per_row) {
    FILE* fp = fopen(filename.c_str(), "wb");
    if (!fp) {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return;
    }

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png) {
        std::cerr << "Failed to create PNG write struct." << std::endl;
        fclose(fp);
        return;
    }

    png_infop info = png_create_info_struct(png);
    if (!info) {
        std::cerr << "Failed to create PNG info struct." << std::endl;
        png_destroy_write_struct(&png, nullptr);
        fclose(fp);
        return;
    }

    if (setjmp(png_jmpbuf(png))) {
        std::cerr << "Error during PNG creation." << std::endl;
        png_destroy_write_struct(&png, &info);
        fclose(fp);
        return;
    }

    png_init_io(png, fp);

    png_set_IHDR(
        png, info,
        width, height,
        8, PNG_COLOR_TYPE_RGB,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT
    );

    png_write_info(png, info);

    for (int y = 0; y < height; y++) {
        png_write_row(png, const_cast<png_bytep>(data + y * bytes_per_row));
    }

    png_write_end(png, nullptr);
    png_destroy_write_struct(&png, &info);
    fclose(fp);
}

void process_pdf(const fs::path& pdf_path, const fs::path& output_dir, double dpi) {
    // 打开 PDF 文件
    auto doc = poppler::document::load_from_file(pdf_path.string());
    if (!doc) {
        std::cerr << "Failed to load PDF file: " << pdf_path << std::endl;
        return;
    }

    // 获取第一页
    poppler::page* page = doc->create_page(0);  // 第一页索引为 0
    if (!page) {
        std::cerr << "Failed to load the first page of: " << pdf_path << std::endl;
        return;
    }

    // 设置渲染参数
    int width = static_cast<int>(page->page_rect().width() * dpi / 72.0);
    int height = static_cast<int>(page->page_rect().height() * dpi / 72.0);

    // 创建渲染器
    poppler::page_renderer renderer;
    renderer.set_render_hint(poppler::page_renderer::antialiasing, true);
    renderer.set_render_hint(poppler::page_renderer::text_antialiasing, true);

    // 渲染页面到图像
    poppler::image image = renderer.render_page(page, dpi, dpi);

    // 获取图像数据
    int bytes_per_row = image.bytes_per_row();
    const unsigned char* image_data = reinterpret_cast<const unsigned char*>(image.data());

    // 构造输出文件名
    std::string output_file = output_dir.string() + "/" + pdf_path.stem().string() + ".png";

    // 保存为 PNG 文件
    save_as_png(output_file, image_data, width, height, bytes_per_row);

    std::cout << "Page saved as " << output_file << std::endl;

    // 清理资源
    delete page;
}

int main(int argc, char* argv[]) {
    // if (argc < 3) {
    //     std::cerr << "Usage: " << argv[0] << " <input_directory> <output_directory>" << std::endl;
    //     return 1;
    // }

    std::string input_dir = "/home/ive/input_pdfs";
    std::string output_dir = "/home/ive/output_thumbnails";

    // 确保输出目录存在
    fs::create_directories(output_dir);

    // 遍历输入目录中的所有 PDF 文件
    for (const auto& entry : fs::directory_iterator(input_dir)) {
        if (entry.path().extension() == ".pdf") {
            process_pdf(entry.path(), fs::path(output_dir), 72.0); // 使用 72 DPI
        }
    }

    return 0;
}