{ pkgs, stdenv, ... }:
stdenv.mkDerivation {
  src = ./.;
  pname = "triangles in 3D";
  version = "0.1.0";
  nativeBuildInputs = with pkgs; [
    cmake
    lit
    filecheck
  ];
  buildInputs = with pkgs; [
    valgrind
    vulkan-loader
    vulkan-headers
    vulkan-tools
    spirv-tools
    clang-tools
    glfw
    glade
    glm
  ];
}
