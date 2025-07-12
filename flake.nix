{
  description = "The set of dependencies to build the project with Vulkan API";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
    flake-parts.url = "github:hercules-ci/flake-parts";
    treefmt-nix = {
      url = "github:numtide/treefmt-nix";
      inputs.nixpkgs.follows = "nixpkgs";
    };
    nixGL.url = "github:nix-community/nixGL";
  };

  outputs =
    { flake-parts, treefmt-nix, ... } @ inputs:
    flake-parts.lib.mkFlake { inherit inputs; } {
      imports = [ treefmt-nix.flakeModule ];

      systems = [
        "x86_64-linux"
        "aarch64-linux"
      ];

      perSystem =
        { pkgs, ... }:
        let
          VulkanPackages = pkgs.vulkan-headers;
        in
        rec {
          packages = rec {
            graphicTriangles = pkgs.callPackage ./. { stdenv = VulkanPackages.stdenv; };
            default = graphicTriangles;
          };

          devShells.default = (pkgs.mkShell.override { stdenv = VulkanPackages.stdenv; }) {
            nativeBuildInputs = packages.graphicTriangles.nativeBuildInputs;
            buildInputs = with pkgs; [
                glm
                glfw-wayland
                vulkan-loader
                vulkan-validation-layers
                vulkan-headers
            ];
          };
        };
    };
}
