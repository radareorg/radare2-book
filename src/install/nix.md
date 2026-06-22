## Nix

radare2 ships a [Nix](https://nixos.org) flake for reproducible builds and development environments on Linux and macOS. The packaging lives in `dist/nix/` and a redirect shim in the repository root ties it to the lock file.

### Running radare2

Run radare2 directly without installing it:

```sh
nix run github:radareorg/radare2
```

Open an interactive shell with `r2` and related tools in `PATH`:

```sh
nix shell github:radareorg/radare2
```

Build the package into the Nix store:

```sh
nix build github:radareorg/radare2
```

The resulting `result/bin/radare2` is a symlink to the built binary.

### Development Shell

Clone the repository and enter the dev shell to get all build dependencies (meson, ninja, capstone, zlib, etc.) without installing them system-wide:

```sh
git clone https://github.com/radareorg/radare2
cd radare2
nix develop
```

Inside the dev shell, build radare2 normally:

```sh
meson setup build
ninja -C build
```

You can also build directly from local source with Nix:

```sh
nix build
```

### Overlays

The flake exposes an overlay so you can add radare2 to your own Nix configuration:

```nix
# In your flake.nix inputs:
inputs.radare2.url = "github:radareorg/radare2";

# In your outputs:
nixpkgs.overlays = [ radare2.overlays.default ];
```

After applying the overlay, `pkgs.radare2` is available in your package set.

### Formatting

Run `nix fmt` to format `.nix` files in the repository using [nixfmt](https://github.com/NixOS/nixfmt) (RFC 166 style).
