#include "glwidget.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QWheelEvent>
#include <sstream>

std::vector<std::vector<Layer<double> > > layer_groups;
Layer<double>* selected_layer;
double rotate_y;
double rotate_x;
vec center_pos;
int active_layer_group;
std::list<int> active_layers;
bool show_axis;
bool show_grid;
bool show_wave;
double cam_radius;

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent)
{
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    if( qApp->keyboardModifiers() & Qt::ShiftModifier || event->modifiers() & Qt::ShiftModifier)
    {
        if(event->orientation() == Qt::Vertical)
        {
            center_pos.y += static_cast<double>(event->delta()) / 1000;
            updateGL();
        } else if(event->orientation() == Qt::Horizontal) {
            center_pos.x += static_cast<double>(event->delta()) / 1000;
            updateGL();
        }
    } else {

        if(event->orientation() == Qt::Vertical)
        {
            cam_radius += static_cast<double>(event->delta()) / 1000;
            updateGL();
        } else if(event->orientation() == Qt::Horizontal) {
            int newPos = active_layer_group + static_cast<int>(event->delta()) / 50;

            if( newPos >= 0 && newPos <= static_cast<int>(layer_groups.size()-1) ) {
                active_layer_group += static_cast<int>(event->delta()) / 50;
                updateGL();
                emit layerSetChanged(active_layer_group+1);
            }
        }
    }
}


void GLWidget::mousePressEvent(QMouseEvent *event)
{
 lastPos = event->pos();
}


void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
     int dx = event->x() - lastPos.x();
     int dy = event->y() - lastPos.y();

     if (event->buttons() & Qt::LeftButton) {
         rotate_x += dy;
         rotate_y += dx;
     } else if (event->buttons() & Qt::LeftButton) {
         rotate_x += dy;
         rotate_y += dx;
     }

    lastPos = event->pos();
    updateGL();
}

void GLWidget::initializeGL(){
	rotate_y=0;
	rotate_x=0;
	active_layer_group=0;
    show_axis=true;
	show_grid=true;
	show_wave=false;
	cam_radius=2.0;
	center_pos=vec(0,0,0);

    glClearColor(255,255,255,1.0);
	glEnable(GL_DEPTH_TEST | GL_LINE_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void GLWidget::paintGL(){
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluPerspective(60.0f,(GLfloat)width/(GLfloat)height,0.01f,650.0f);
	gluLookAt(
		0.0, cam_radius, cam_radius,
		0.0, 0.0, 0.0,
		0.0, 0.0, 1.0);

	glViewport(0, 0, width, height);
	glRotatef( rotate_x, 1.0, 0.0, 0.0 );
	glRotatef( rotate_y, 0.0, 0.0, 1.0 );

	draw_layers();
	if (show_grid)
        draw_grid(-0.5,-0.5,0.1,0.1,11,11,&guide_z,&guide_col);
	if (show_axis)
		draw_axis();

	glFlush();
}

void GLWidget::resizeGL(int width_, int height_){
    width = width_;
    height = height_;

    /*if ( height_ == 0 )
        height_ = 1;

    glViewport( 0, 0, width_, height_ );

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    gluPerspective(60.0f,(GLfloat)width/(GLfloat)height,0.01f,650.0f);

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    */
}

void GLWidget::open_graphic(std::string file){
	layer_groups.clear();

    std::string line;
	std::ifstream infile(file.c_str());

	while (std::getline(infile, line)){
		std::istringstream iss(line);
		if (line.length()==0 || layer_groups.size()==0){
			std::vector<Layer<double> > layers;
			layer_groups.push_back(layers);
			if (line.length()==0) continue;
		};

		std::string layer_file;
		std::getline(iss,layer_file,';');
		layer_file = file.substr(0,file.rfind("/"))+"/"+layer_file;
		std::string color_string;
		std::getline(iss,color_string,',');
        int rgb = (int)strtol(color_string.c_str(), NULL, 16);
        int r = double((rgb&0xFF0000)>>16)/255.0;
        int g = double((rgb&0x00FF00)>>8)/255.0;
        int b = double(rgb&0x0000FF)/255.0;
		open_layer(layer_file,r,g,b);
	}
}
	

void GLWidget::open_layer(std::string file, double r, double g, double b){
    Layer<double> layer(file, r, g, b);
    layer_groups[layer_groups.size()-1].push_back(layer);
	updateGL();
}

void GLWidget::draw_quad(vec a, vec b, vec c, vec d){
	glBegin(GL_POLYGON);
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);
	glVertex3f(c.x, c.y, c.z);
	glVertex3f(d.x, d.y, d.z);
	glEnd();
};

void GLWidget::draw_grid(double x,double y,double dx,double dy,int w,int h,double(*z)(const vec&),vec(*col)(const vec&)){
	x += center_pos.x;
	y += center_pos.y;
	for (int i=0; i<w - 1; i++){
		for (int j=0; j<h - 1; j++){
			vec color(col(vec(i,j,0)));
			glColor3f(color.x, color.y, color.z);
			int I = i+1; 
			int J = j+1;
			float pi = x+i*dx;
			float pj = y+j*dy;
			float pI = x+I*dx;
			float pJ = y+J*dy;
			draw_quad(
				vec(pi,pj,z(vec(i,j))),
				vec(pI,pj,z(vec(I,j))),
				vec(pI,pJ,z(vec(I,J))),
				vec(pi,pJ,z(vec(i,J))));
		};
	};
}

void GLWidget::draw_line(const vec & a, const vec & b){
	glBegin(GL_LINES);
	glVertex3d(a.x,a.y,a.z);
	glVertex3d(b.x,b.y,b.z);
	glEnd();
}

void GLWidget::draw_axis(){
	glColor3f(1.0,0.0,0.0); draw_line(vec(0.0,0.0,0.0),vec(1.0,0.0,0.0));
	glColor3f(0.0,1.0,0.0); draw_line(vec(0.0,0.0,0.0),vec(0.0,1.0,0.0));
	glColor3f(0.0,0.0,1.0); draw_line(vec(0.0,0.0,0.0),vec(0.0,0.0,1.0));
}

double layer_z(const vec& pos){
	return selected_layer->z(pos.x,pos.y);
}
vec layer_col(const vec & pos){
	double c=0.5+double(pos.x)*0.05 + double(int(pos.x+pos.y)%2)*0.1;
	return vec(selected_layer->r*c,selected_layer->g*c,selected_layer->b*c);
}

double guide_z(const vec & pos){
    return 0;
}
vec guide_col(const vec & pos){ 
	double add=int(pos.x+pos.y)%2*0.2; 
	return vec(0.5+add,0.5+add,0.5+add); 
}

void GLWidget::draw_layers(){
	for (unsigned int i=0; i<layer_groups[active_layer_group].size(); ++i){
		selected_layer = &layer_groups[active_layer_group][i];

        QList<int> layer_list;
        layer_list = layer_list.fromStdList(active_layers);

        if(!layer_list.contains(i))
            continue;

        draw_grid(
            layer_groups[active_layer_group][i].x,
            layer_groups[active_layer_group][i].y,
            layer_groups[active_layer_group][i].dx,
            layer_groups[active_layer_group][i].dy,
            layer_groups[active_layer_group][i].dimx,
            layer_groups[active_layer_group][i].dimy,
            layer_z,
            layer_col);
	}
}
