# videomap
Orienteering map overlay for videos

-Takes a gps route, exported as xml from Quickroute, a video recorded while orienteering and a map image and overlays the map on the video frame by frame

###Dependencies
Requires quickroute to export an xml and jpg file of the route
```
OpenCV
```

###Compile
g++ -o readImage readImage.cpp `pkg-config --cflags --libs opencv`

###Usage
./readImage /path/to/route/xml /path/to/map/image /path/to/video offset

The offset is the number of seconds the gps recording starts after the video recording


