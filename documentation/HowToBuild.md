
# How to Build by yourself:

## Clone the Repository

You should clone with 
```shell
$ git clone --recursive https://github.com/Eng1n33r/flipperzero-firmware.git
```

## Build with Docker

### Prerequisites

1. Install [Docker Engine and Docker Compose](https://www.docker.com/get-started)
2. Prepare the container:

 ```sh
 docker-compose up -d
 ```

### Compile everything for development

```sh
docker-compose exec dev ./fbt
```

### Compile everything for release + get updater package to update from microSD card

```sh
docker-compose exec dev ./fbt --with-updater COMPACT=1 DEBUG=0 updater_package
```

Check `dist/` for build outputs.

Use **`flipper-z-{target}-full-{suffix}.dfu`** to flash your device.

If compilation fails, make sure all submodules are all initialized. Either clone with `--recursive` or use `git submodule update --init --recursive`.

# Build on macOS

Check out `documentation/fbt.md` for details on building and flashing firmware. 

## macOS Prerequisites

Make sure you have [brew](https://brew.sh) and install all the dependencies:
```sh
brew bundle --verbose
```

Install Python packages required by assets build scripts: `pip3 install -r scripts/requirements.txt`

### Compile everything for development

```sh
./fbt
```

### Compile everything for release + get updater package to update from microSD card

```sh
./fbt --with-updater COMPACT=1 DEBUG=0 updater_package
```

Check `dist/` for build outputs.

Use **`flipper-z-{target}-full-{suffix}.dfu`** to flash your device.