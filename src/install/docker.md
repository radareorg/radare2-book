## Docker containers

Docker is a software that use OS-level virtualization to deliver software in packages called images and containers when running them.
Containers bundle their own software and are isolated from the host system, this prevents conflicts with other installed software.

There are several implamentations to run Docker images but radare2 images only have been tested with `docker`, `podman` and `nerdctl`.

### Available images

There are 2 mantained docker images, one for the official releases and one targeted to be build locally from GIT.
Both can be used similarly.

#### Stable version

This docker image can be found in [Docker Hub](https://hub.docker.com/r/radare/radare2) and contain the latest radare2 stable version.
This image is based on **Ubuntu** and the same [radare2 snap](https://snapcraft.io/radare2) build.
The Dockerfile used to build it can be found in [this dedicated repository](https://github.com/radareorg/radare2-snap).

The resulting build includes the following projects:

* [radare2](https://github.com/radareorg/radare2)
* [r2ghidra](https://github.com/radareorg/r2ghidra)
* [r2frida](https://github.com/nowsecure/r2frida) (only in supported platforms)
* [r2dec](https://github.com/wargio/r2dec-js)
* [r2yara](https://github.com/radareorg/r2yara)
* [r2pipe](https://pypi.org/project/r2pipe/) (for Python)

To use this docker image you can use either:
```sh
docker run -ti radare/radare2
podman run -ti docker.io/radare/radare2
nerdctl run -ti radare/radare2
```

To use the docker image as one shot so it removes everything inside the container on exit just add `--rm` as follows:
```sh
docker run --rm -ti radare/radare2
```

Another example to use for debugging inside the docker:
```sh
docker run --tty --interactive --privileged --cap-add=SYS_PTRACE --security-opt seccomp=unconfined --security-opt apparmor=unconfined radare/radare2
```

#### GIT version

Alternatively there is a version from radare2 GIT aimed to be build locally, also called **r2docker**.
This will build an image using **Debian** with radare2 from GIT with latest changes.
The Dockerfile to build can be found inside the `dist/docker` directory in the [radare2](https://github.com/radareorg/radare2) source tree.

To build this other image run the following lines:

```sh
git clone https://github.com/radareorg/radare2.git
cd radare2
make -C dist/docker
```

This will build an image with the following plugins by default:

* [r2ghidra](https://github.com/radareorg/r2ghidra)
* [r2frida](https://github.com/nowsecure/r2frida)
* [r2dec](https://github.com/wargio/r2dec-js)

It is possible to specify more packages using the `R2PM` make variable:

```sh
make -C dist/docker R2PM=radius2
```

Also, you can select the architecture (`amd64` / `arm64`) to compile the image by using the `ARCH` make variable.

This Dockerfile also used by Remnux distribution from SANS, and is available on the [Docker Hub](https://hub.docker.com/r/remnux/radare2), but it might not contain latest changes.


### Run a container as r2web server

By default both images are intended to be used in a interactive terminal.

But both can also be launched directly to use the radare2 web UI.

The do so it can be launched using the following command:
```sh
docker run -p 9090:9090 radare/radare2 r2 -c '=h' -
```

Or the following docker-compose structure:
```yaml
version: "3.8"
services:
  radare2:
    image: radare/radare2
    command: r2 -c '=h' -
    network_mode: bridge
    ports:
      - "9090:9090"
```

Or if debugging functionality is required:
```yaml
version: "3.8"
services:
  radare2:
    image: radare/radare2
    command: r2 -c '=h' -
    network_mode: bridge
    ports:
      - "9090:9090"
    privileged: true
    cap_add:
      - SYS_PTRACE
    security_opt:
      - "seccomp=unconfined"
      - "apparmor=unconfined"
```
