{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };
  outputs =
    {
      self,
      nixpkgs,
      flake-utils,
    }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = import nixpkgs { inherit system; };

        mkFlash =
          name: cmd:
          pkgs.writeShellScriptBin name ''
            set -euo pipefail
            echo ">>> ${cmd}"
            exec ${cmd} "$@"
          '';

        flashScripts = [
          (mkFlash "flash-kyria-rev3-liatris"
            "qmk flash -kb splitkb/kyria/rev3 -km teelahti -e CONVERT_TO=liatris"
          )
          (mkFlash "flash-kyria-rev1" "qmk flash -kb splitkb/kyria/rev1 -km teelahti")
          (mkFlash "flash-kyria-rev4-halcyon" "qmk flash -kb splitkb/halcyon/kyria/rev4 -km teelahti")
          (mkFlash "flash-kaly42-left" "qmk flash -kb kaly/kaly42 -km teelahti -bl dfu-util-split-left")
          (mkFlash "flash-kaly42-right" "qmk flash -kb kaly/kaly42 -km teelahti -bl dfu-util-split-right")
          (mkFlash "flash-onekey-spacebar" "qmk flash -kb handwired/onekey/rp2040 -km default_spacebar")
          (mkFlash "flash-ploopy-nano" "qmk flash -kb ploopyco/trackball_nano -km teelahti")
        ];
      in
      {
        devShells.default =
          with pkgs;
          mkShell {
            buildInputs = [
              qmk
              gnumake
              dos2unix
            ]
            ++ flashScripts;

            shellHook = ''
              echo ""
              echo "QMK flash commands available:"
              echo "  flash-kyria-rev3-liatris   Kyria rev3 + Liatris RP2040 (home)"
              echo "  flash-kyria-rev1           Kyria rev1 AVR (work)"
              echo "  flash-kyria-rev4-halcyon   Halcyon Kyria rev4"
              echo "  flash-kaly42-left          Kaly42 left half"
              echo "  flash-kaly42-right         Kaly42 right half"
              echo "  flash-onekey-spacebar      Handwired XIAO RP2040 spacebar"
              echo "  flash-ploopy-nano          Ploopyco Trackball Nano"
              echo ""
            '';
          };
      }
    );
}
