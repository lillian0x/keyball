{
  description = "QMK firmware build";
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  };
  outputs = {
    self,
    nixpkgs,
  }: let
    system = "x86_64-linux";
    pkgs = import nixpkgs {inherit system;};
    buildFirmware = {
      keyboard,
      keymap,
    }:
      pkgs.stdenv.mkDerivation {
        pname = "qmk-firmware-${keyboard}-${keymap}";
        version = "0.22.14";
        src = pkgs.fetchgit {
          url = "https://github.com/qmk/qmk_firmware.git";
          rev = "0.22.14";
          sha256 = "sha256-0xezGGrzu6UlKFLsAIBBCzkXehKTPsAtymcZa0y6u2Y=";
          fetchSubmodules = true;
        };
        nativeBuildInputs = with pkgs; [
          qmk
          gcc
          avrdude
          (python3.withPackages (ps:
            with ps; [
              appdirs
              argcomplete
              colorama
              dotty-dict
              hidapi
              hjson
              jsonschema
              milc
              pyusb
              pygments
              pillow
            ]))
        ];
        buildPhase = ''
          export HOME=$PWD

          chmod -R +w .

          ln -s ${./qmk_firmware/keyboards/keyball} keyboards/keyball

          qmk compile -kb keyball/${keyboard} -km ${keymap}
        '';
        installPhase = ''
          mkdir -p $out
          cp *.hex $out/
        '';
      };
  in {
    packages.${system} = {
      keyball44-dvorak = buildFirmware {
        keyboard = "keyball44";
        keymap = "dvorak";
      };
      keyball44-default = buildFirmware {
        keyboard = "keyball44";
        keymap = "default";
      };
    };
    defaultPackage.${system} =
      self.packages.${system}.keyball44-dvorak;
    devShells.${system}.default = pkgs.mkShell {
      packages = with pkgs; [
        qmk
        git
        python3
        python3Packages.pip
        gcc
        avrdude
      ];
      shellHook = ''
        echo "QMK dev environment ready"
      '';
    };
  };
}
