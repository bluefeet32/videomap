# videomap
Orienteering map overlay for videos

Takes a gps route, exported as xml from Quickroute, a video recorded while orienteering and a map image and overlays the map on the video frame by frame

### Dependencies
Requires quickroute to export an xml and jpg file of the route
```
OpenCV
```

### Compile
```
./build.sh
```
Currently only simply does:
```
g++ -o readImage readImage.cpp `pkg-config --cflags --libs opencv`
```

### Usage
First do your normal quickroute set up, dragging the route to the correct places. Once that is done, export the map as a jpg with tran
./readImage /path/to/route/xml /path/to/map/image /path/to/video offset

The offset is the number of seconds the gps recording starts after the video recording

While the program is running, the video is displayed on the screen in real time. The following controls are available:
```
p - pause
- - zoom out of map
= - zoom into map
up arrow - increase size of map region
down arrow - decrease size of map region
left arrow - decrease playback speed
right arrow - increase playback speed
ESC - stop execution and gracefully exit
```

### TODOs
The output is currently massive (>4x the size of the input)
Add a small example set of files

### Notes
There are some extra files for testing the xml, video and image reading parts seperately. Instructions for compiling them


