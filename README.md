
### 3D Multilayer Viewer

A simple C++ QT-based 3D viewer for height maps.

---

#### Example usage:

1. Compile and run it using QtCreator 5+
2. File > Open > /example/example.v3d

The program will then read the data on the files and display it on the screen. You should see something similar to this:

![print](https://raw.github.com/viclib/3d_multilayer_viewer/master/example/example.png)

Observe the files at `/example` to understand how to include your own layers.

---

#### Controls

Key | Effect
--- | ---
Ctrl+O | Open new file(.v3d)
Ctrl+S | Save current file
Arrow Up/Arrow Down | Increase/decrease camera inclination
Arrow Left/Arrow Right | Rotate camera
W | Move forward
A | Move left
S | Move back
D | Move right
Z | Toggle axis
X | Toggle guides
U/I | Back/Advance LayerSet
Keys 1 to 9 | Hide/Show Layer N
HorizontalPad | Change Slider position
J/K/Wheel/VerticalPad | Zoom in/out

#### Generate Video Custom Script

-> Create a text file with own ffmpeg commands, accordingly to this syntax:

Pattern | Equivalent
--- | ---
%1 | Images path(location where images used to generate video are stored)
%2 | Save to path(path selected at user interface)
%3 | Script path(to be able to use relative paths - e.g.: %3/audio.mp3 would load the file at same path of script)
%4 | Intro file(path to app logo image)

#### Dependecies

* OpenGL
* ffmpeg

-> ffmpeg library needs to be installed at user path enviroment to proper 'Generate Video' functionality