#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QListWidget>
#include <QColorDialog>
#include <QDialogButtonBox>
#include <QLabel>
#include <QRadioButton>
#include <QCheckBox>
#include <QSpinBox>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
	void keyPressEvent(QKeyEvent* event);
    bool eventFilter( QObject* object, QEvent* event );

private slots:
    void on_actionOpen_triggered();
    void action_makeVideo();
    void setColor(QModelIndex index);
    void stateChanged(QListWidgetItem *cur_item);
    void layerSet(int activeSet);
    void chooseSavePath();
    void chooseAlternativeScript();
    void grabFrameBuffer();
    void save_graphic();
    void changeAllLayersColors(bool color);
    void about();
    void openHelp();
private:

    QMenu *menuVideo;
    QAction *makeVideo;

    std::string ConvertRGBtoHex(int r, int g, int b);

    void createDockWindows();
    bool all_layers;

    Ui::MainWindow *ui;

    QDialog *videoDialog;

    QLineEdit *VideoCfgSaveToLineEdit;
    QLineEdit *VideoCfgAlternativeScript;

    QRadioButton *VideoCfgDefaultRadio;
    QRadioButton *VideoCfgCustomRadio;

    QCheckBox *VideoCfgShowIntro;
    QCheckBox *VideoCfgFadeInIntro;
    QCheckBox *VideoCfgCloseAfterRendering;
    QCheckBox *VideoCfgRemoveJunk;
    QCheckBox *VideoCfgOpenFolder;

    QSpinBox *VideoCfgStartFrom;
    QSpinBox *VideoCfgSpeed;
    QSpinBox *VideoCfgFPS;
    QSpinBox *VideoCfgIntroDelay;
    QSpinBox *VideoCfgImageQuality;

    QLabel *displayed_layer;

    QDockWidget *layerDock;
    QDockWidget *sliderDock;
    QToolBar *sliderDiag;
    QListWidget *layerList;
    QListWidgetItem *item;
    QSlider *slider;
    QString fileName;
};

extern double rotate_y;
extern double rotate_x;
extern bool show_axis;
extern bool show_grid;
extern bool show_wave;
extern double cam_radius;

#endif // MAINWINDOW_H
