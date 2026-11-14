{
  description = "QMK firmware build";
  inputs = {
    git-hooks = {
      url = "github:cachix/git-hooks.nix";
      inputs.nixpkgs.follows = "nixpkgs";
    };
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  };
  outputs = {
    self,
    nixpkgs,
    git-hooks,
  }: let
    system = "x86_64-linux";
    pkgs = import nixpkgs {inherit system;};
    qmk = pkgs.qmk.override {python3Packages = pkgs.python313Packages;};
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
          avrdude
          (python313.withPackages (ps:
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

          ALLOW_WARNINGS=yes qmk compile -kb keyball/${keyboard} -km ${keymap}
        '';
        installPhase = ''
          mkdir -p $out
          cp *.hex $out/
        '';
      };
  in {
    checks.${system} = {
      pre-commit-check = git-hooks.lib.${system}.run {
        src = ./.;
        hooks = {
          alejandra = {
            enable = true;
            settings.verbosity = "quiet";
          };
          deadnix.enable = true;
          statix.enable = true;
        };
      };
    };
    formatter.${system} = let
      pkgs = nixpkgs.legacyPackages.${system};
      inherit (self.checks.${system}.pre-commit-check.config) package configFile;
    in
      pkgs.writeShellScriptBin "pre-commit-run" ''
        ${pkgs.lib.getExe package} run --all-files --config ${configFile}
      '';
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
    devShells.${system}.default = let
      inherit (self.checks.${system}.pre-commit-check) shellHook enabledPackages;
    in
      pkgs.mkShell {
        buildInputs = enabledPackages;
        inherit shellHook;
        packages = with pkgs; [
          qmk
          python313
          python313Packages.pip
          avrdude
        ];
      };
  };
}
