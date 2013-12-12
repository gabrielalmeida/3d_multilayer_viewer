
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
Z | Toggle axis
X | Toggle guides
U/I | Back/Advance LayerSet
Keys 1 to 9 | Hide/Show Layer N
HorizontalPad | Change Slider position
J/K/Wheel/VerticalPad | Zoom in/out
