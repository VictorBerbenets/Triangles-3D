
#include "display/pipeline.hpp"

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <format>

namespace triangles {
    
std::vector<char> VKTrianglesPipeline::getFileData(std::string_view FilePath) {
  std::ifstream File(FilePath.data(), std::ios::ate | std::ios::binary);
  if (!File.is_open())
    throw std::runtime_error(std::format("failed to open file: {}", FilePath.data())); 

  auto FileSize = static_cast<unsigned>(File.tellg());
  std::vector<char> Data(FileSize);

  File.seekg(0);
  File.read(Data.data(), FileSize);

  File.close();
  return Data;
}
    
void VKTrianglesPipeline::createGraphicsPipeline(std::string_view VertFilePath, std::string_view FragmFilePath) {
  auto VertData = getFileData(VertFilePath);
  auto FragmData = getFileData(FragmFilePath);

  std::cout << "Vertex Shader Code Size = " << VertData.size() << '\n';
  std::cout << "Fragment Shader Code Size = " << FragmData.size() << '\n';
}

} // namespace triangles
