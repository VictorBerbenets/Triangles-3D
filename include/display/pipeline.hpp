#pragma once

#include <string_view>
#include <vector>

namespace triangles {

class VKTrianglesPipeline {
public:
  VKTrianglesPipeline(std::string_view VertFilePath,
                      std::string_view FragmFilePath) {
    createGraphicsPipeline(VertFilePath, FragmFilePath);
  }

private:
  std::vector<char> getFileData(std::string_view FilePath);

  void createGraphicsPipeline(std::string_view VertFilePath,
                              std::string_view FragmFilePath);
};

} // namespace triangles
