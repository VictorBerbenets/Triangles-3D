{ pkgs, stdenv, ... }:
stdenv.mkDerivation {
  src = ./.;
  pname = "triangles in 3D";
  version = "0.1.0";
  nativeBuildInputs = with pkgs; [
    ninja
    cmake
    clang-tools
    clang
  ];
  buildInputs = with pkgs; [
    valgrind
    vulkan-loader
    vulkan-headers
    vulkan-tools
    spirv-tools
    glfw
    glade
    glm
  ];
}
