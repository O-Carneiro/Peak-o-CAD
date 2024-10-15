# Peak-O-CAD: The simple, efficient and original 3D model builder.

### How is docker being setup?
Peak-O-CAD is a OpenGL based software, and therefore uses the GPU
and display-related drivers to run. 

To develop, we opted to use Docker, and it comes with a big throwback:
the docker container must be pierced so that the program can access the GPU and the display.\

Therefore, we are using `xhost` to give docker X server access, so it can manage the 
displays and its windows. It is smart enough to work even if you are in Wayland, don't worry. 

Then, we build it normally, run it with `--net=host` so it able to access the X server directly,
set the `DISPLAY` enviroment variable, and expose the `/dev/dri` devices to the container
granting it access to the GPU.

### System dependencies

`x11-xserver-utils` is needed to run the `xhost` command.



### Building and running the project
Run this once in the session, before running the program.
```
xhost +local:docker 
```
Build
``` 
docker build -t peak-o-cad /path/to/root/dir
```
Run
```
docker run --rm -it --net=host --env DISPLAY=$DISPLAY \
    --device /dev/dri:/dev/dri \
    peak-o-cad
```
### SFML Cmake setup reference:
[GitHub](https://github.com/SFML/cmake-sfml-project?tab=readme-ov-file)
