{ pkgs, stdenv, ... }:
stdenv.mkDerivation {
  src = ./.;
  pname = "triangles in 3D";
  version = "0.1.0";
  nativeBuildInputs = with pkgs; [
    ninja
    cmake
    valgrind
    clang-tools
    clang
  ];
}
