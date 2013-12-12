#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include "vector3d.h"
#include "layer.h"
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <vector>
#include <iostream>
#include <assert.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

typedef Vector3D vec;
extern std::vector<std::vector<Layer<double> > > layer_groups;
extern Layer<double>* selected_layer;
extern double rotate_y;
extern double rotate_x;
extern vec center_pos;
extern int active_layer_group;
extern std::list<int> active_layers;
extern bool show_axis;
extern bool show_grid;
extern bool show_wave;
extern double cam_radius;

class GLWidget : public QGLWidget {
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = 0);
    ~GLWidget(){};

    void initializeGL();
    void paintGL();
    void resizeGL(int,int);

	void draw_quad(vec a, vec b, vec c, vec d);
	void draw_grid(double x,double y,double dx,double dy,int w,int h,double(*z)(const vec&),vec(*col)(const vec&));
	void draw_line(const vec & a, const vec & b);
	void draw_axis();
	void draw_layers();

	void open_layer(std::string file, double r, double g, double b);
    void open_graphic(std::string file="");
	//double wavy_z(const vec & pos);
	//vec wavy_col(const vec & pos);

signals:
    void layerSetChanged(int layer);

protected:
    void wheelEvent(QWheelEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
	unsigned int width;
	unsigned int height;
    QPoint lastPos;
};

double guide_z(const vec & pos);
vec guide_col(const vec & pos);

double layer_z(const vec& pos);
vec layer_col(const vec & pos);

#endif // GLWIDGET_H
