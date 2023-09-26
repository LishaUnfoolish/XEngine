#include <opencv4/opencv2/opencv.hpp>

int main(int argc, char** argv) {
    // 从参数中获取图片路径
  std::string filename="/xengine/test_bazel/test_opencv/test.jpg";
  if (argc == 2) {
    filename = argv[1];
  }
  cv::Mat img = cv::imread(filename);
  if (img.empty()) {
    printf("Could not load image\n");
    return -1;
  }
  cv::imshow("image", img);
  cv::waitKey(0);
  return 0;
} 