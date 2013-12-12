#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdlib.h>
#include <QDebug>
#include "glwidget.h"
#include <QFileDialog>
#include <QDockWidget>
#include <QListWidget>
#include <QMessageBox>
#include <QtWidgets>
#include <algorithm>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->glwidget->hide();

    displayed_layer = new QLabel;
    displayed_layer->setText("Selected LayerSet:" + QString::number(active_layer_group));
    ui->verticalLayout->addWidget(displayed_layer);
    displayed_layer->hide();
}

void MainWindow::createDockWindows()
{
    layerDock = new QDockWidget(tr("Layers"), this);
    layerDock->setAllowedAreas(Qt::NoDockWidgetArea);

    layerList = new QListWidget(layerDock);
    layerDock->setWidget(layerList);


    layerList->setStyleSheet("QListWidget::indicator::checked{ image: url(:/images/eye-icon-hide.png) }\
                             QListWidget::indicator::unchecked{ image: url(:/images/eye-icon-view.png) }");

    connect(layerList, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(setColor(QModelIndex)));
    connect(layerList, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(stateChanged(QListWidgetItem*)));

    addDockWidget(Qt::RightDockWidgetArea, layerDock);

    layerDock->setFloating(layerList);
    layerDock->setMinimumSize(200,150);
    layerDock->setMaximumSize(200,300);
    layerDock->installEventFilter(this);
    layerDock->setWindowOpacity(0.9);
    layerDock->move(962,149);

    ui->menuView->addAction(layerDock->toggleViewAction());

    sliderDock = new QDockWidget("Timeline", this);
    sliderDock->setAllowedAreas((Qt::BottomDockWidgetArea));
    sliderDock->setFeatures(QDockWidget::NoDockWidgetFeatures);

    sliderDiag = new QToolBar(sliderDock);
    sliderDock->setWidget(sliderDiag);
    ui->menuView->addAction(sliderDock->toggleViewAction());


    slider = new QSlider(Qt::Horizontal, this);

    connect( slider, SIGNAL(valueChanged(int)), SLOT(layerSet(int)) );
    connect(ui->glwidget, SIGNAL(layerSetChanged(int)), slider, SLOT(setValue(int)));

    slider->setTickPosition(QSlider::TicksAbove);
    slider->setTickInterval(1.0);
    slider->setMinimum(1.0);
    slider->setMaximum(layer_groups.size());
    slider->setSingleStep( 1 );
    slider->setPageStep( 1 );

    sliderDiag->addWidget(slider);


    if(ui->glwidget->isHidden()) { // first v3d opened
        QAction *saveVideoAction = new QAction("Save", this);
        saveVideoAction->setShortcut(QKeySequence::fromString("Ctrl+S"));
        ui->menuFile->addAction(saveVideoAction);
        connect(saveVideoAction, SIGNAL(triggered()), this, SLOT(save_graphic()));

        QAction *allLayersColorsAction = new QAction("Apply color changes to all layers", this);
        allLayersColorsAction->setCheckable(true);
        QMenu *colorMenu = new QMenu("Color");
        ui->menuBar->addMenu(colorMenu);
        colorMenu->addAction(allLayersColorsAction);
        connect(allLayersColorsAction, SIGNAL(toggled(bool)), this, SLOT(changeAllLayersColors(bool)));

        menuVideo = ui->menuBar->addMenu("Video");
        makeVideo = new QAction("Generate Video", this);
        menuVideo->addAction(makeVideo);
        connect(makeVideo, SIGNAL(triggered()), this, SLOT(action_makeVideo()));

        QAction *aboutAction = new QAction("About", this);
        QAction *helpAction = new QAction("Help", this);

        QMenu *helpMenu = new QMenu("Help");
        ui->menuBar->addMenu(helpMenu);

        helpMenu->addAction(aboutAction);
        helpMenu->addAction(helpAction);

        connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
    }

    addDockWidget(Qt::BottomDockWidgetArea, sliderDock);
}

void MainWindow::about() {
    QMessageBox::about(this, "3D Multilayer Viewer", "Gabriel <b>Almeida</b> / Victor <b>Maia</b> <br/> ® No rights reserved");
}

void MainWindow::changeAllLayersColors(bool color) {
    if(!color)
        all_layers = false;
    else
        all_layers = true;
}

std::string char_to_hex(char col){
    char alphabet[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    std::stringstream result;
    result << alphabet[(col&0xF0)>>4];
    result << alphabet[col&0x0F];
    return result.str();
};


void MainWindow::save_graphic(){
    std::string result = "";
    for (unsigned int i=0; i<layer_groups.size(); ++i){
        for (unsigned int j=0; j<layer_groups[i].size(); ++j){
            std::stringstream i_str;
            std::stringstream j_str;
            i_str << i;
            j_str << j;

            // QColor color(layer_groups[i][j].r,layer_groups[i][j].g,layer_groups[i][j].b);
            //color.name().remove("#").toStdString();
            // qDebug() << layer_groups[i][j].r << layer_groups[i][j].g << layer_groups[i][j].b;
            //qDebug() << layer_groups[i][j].r / 0x10000 << (layer_groups[i][j].g/0x100)  % 0x100 << layer_groups[i][j].b % 0x100;

            result+=
                std::string("layer")
                +j_str.str()
                +std::string("time")
                +i_str.str()
                +std::string(".dat")
                +";0x"
                +char_to_hex(char(layer_groups[i][j].r*255))
                +char_to_hex(char(layer_groups[i][j].g*255))
                +char_to_hex(char(layer_groups[i][j].b*255))
                ; // COR AQUI! FAZER A FUCKING CONVERSÃO
            result += "\n";
        };

        if( i != layer_groups.size()-1)
            result += "\n";
    };

    QString final_result(QString::fromStdString(result));
    QFile file(fileName);

    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out << final_result;

    file.close();

    //qDebug() << final_result;
};


void MainWindow::action_makeVideo() {

    QString ffmpeg = QString("ffmpeg");

    if( system(ffmpeg.toStdString().c_str()) != 256 ) { // weird way found to find if ffmpeg is up and go
        QMessageBox::information(this, "ffmpeg seems to be not installed", "It looks like you don't have <b>ffmpeg</b> installed!<br/><br/> <b>ffmpeg</b> lib shoud be available to run at path enviroment, please install it in order to get this function working.");
    }  // Let me show my dear user an alert if he seems to miss ffmpeg lib

    videoDialog = new QDialog(this, Qt::Window);

    QGroupBox *pathGroup = new QGroupBox("Video Directory");

    QLabel *saveToLabel = new QLabel("Save to:");
    VideoCfgSaveToLineEdit = new QLineEdit;
    VideoCfgSaveToLineEdit->setMinimumWidth(400);
    QPushButton *choosePath = new QPushButton("Choose");

    QFileInfo fileInfo(fileName);
    VideoCfgSaveToLineEdit->setText(QDir::homePath() + "/" + fileInfo.baseName());

    connect(choosePath, SIGNAL(clicked()), this, SLOT(chooseSavePath()));

    QHBoxLayout *saveToLayout = new QHBoxLayout;
    saveToLayout->addWidget(saveToLabel);
    saveToLayout->addWidget(VideoCfgSaveToLineEdit);
    saveToLayout->addSpacing(15);
    saveToLayout->addWidget(choosePath);

    QVBoxLayout *pathLayout = new QVBoxLayout;
    pathLayout->addLayout(saveToLayout);
    pathGroup->setLayout(pathLayout);

    QGroupBox *videoModeGroup = new QGroupBox("Video ffmpeg-ing Mode:");

    VideoCfgDefaultRadio = new QRadioButton("Default app ffmpeg script");
    VideoCfgCustomRadio = new QRadioButton("Custom ffmpeg script");

    QHBoxLayout *videoModeLayout = new QHBoxLayout;
    videoModeLayout->addWidget(VideoCfgDefaultRadio);
    videoModeLayout->addWidget(VideoCfgCustomRadio);

    videoModeGroup->setLayout(videoModeLayout);

    QGroupBox *settingsDefaultGroup = new QGroupBox("Default video Settings");
    settingsDefaultGroup->setEnabled(false);

    VideoCfgShowIntro = new QCheckBox("Show Intro");
    VideoCfgFadeInIntro = new QCheckBox("Fade-In Intro");
    VideoCfgRemoveJunk = new QCheckBox("Remove junk files");
    VideoCfgOpenFolder = new QCheckBox("Open video folder at end of generation");
    VideoCfgCloseAfterRendering = new QCheckBox("Close this dialog when video get ready");


    VideoCfgShowIntro->setChecked(true);
    VideoCfgFadeInIntro->setChecked(true);
    VideoCfgRemoveJunk->setChecked(true);
    VideoCfgOpenFolder->setChecked(true);
    VideoCfgCloseAfterRendering->setChecked(true);


    QLabel *StartFromLabel = new QLabel("Start from LayerSet:");
    VideoCfgStartFrom = new QSpinBox;

    VideoCfgStartFrom->setMaximumWidth(80);
    VideoCfgStartFrom->setRange(0, layer_groups.size());
    VideoCfgStartFrom->setSingleStep(1);
    VideoCfgStartFrom->setValue(0);

    QLabel *SpeedLabel = new QLabel("Video Speed:");
    VideoCfgSpeed = new QSpinBox;

    VideoCfgSpeed->setMaximumWidth(80);
    VideoCfgSpeed->setRange(1, 250);
    VideoCfgSpeed->setSingleStep(1);
    VideoCfgSpeed->setValue(100);

    QLabel *FPSLabel = new QLabel("FPS:");
    VideoCfgFPS = new QSpinBox;

    VideoCfgFPS->setMaximumWidth(80);
    VideoCfgFPS->setRange(20, 250);
    VideoCfgFPS->setSingleStep(5);
    VideoCfgFPS->setValue(30);

    QLabel *IntroDelayLabel = new QLabel("Delay at Intro for:");
    VideoCfgIntroDelay = new QSpinBox;

    VideoCfgIntroDelay->setMaximumWidth(80);
    VideoCfgIntroDelay->setRange(0, 30);
    VideoCfgIntroDelay->setSingleStep(1);
    VideoCfgIntroDelay->setValue(3);

    QLabel *ImageQualityLabel = new QLabel("Image Quality:");
    VideoCfgImageQuality = new QSpinBox;

    VideoCfgImageQuality->setMaximumWidth(80);
    VideoCfgImageQuality->setRange(20, 100);
    VideoCfgImageQuality->setSingleStep(5);
    VideoCfgImageQuality->setValue(80);

    QVBoxLayout *settingsDefaultHLayout1 = new QVBoxLayout;
    settingsDefaultHLayout1->addWidget(VideoCfgShowIntro);
    settingsDefaultHLayout1->addWidget(VideoCfgFadeInIntro);
    settingsDefaultHLayout1->addWidget(VideoCfgCloseAfterRendering);
    settingsDefaultHLayout1->addWidget(VideoCfgRemoveJunk);
    settingsDefaultHLayout1->addWidget(VideoCfgOpenFolder);

    QVBoxLayout *settingsDefaultHLayout2 = new QVBoxLayout;
    settingsDefaultHLayout2->addWidget(StartFromLabel);
    settingsDefaultHLayout2->addWidget(VideoCfgStartFrom);
    settingsDefaultHLayout2->addWidget(SpeedLabel);
    settingsDefaultHLayout2->addWidget(VideoCfgSpeed);
    settingsDefaultHLayout2->addWidget(FPSLabel);
    settingsDefaultHLayout2->addWidget(VideoCfgFPS);
    settingsDefaultHLayout2->addWidget(IntroDelayLabel);
    settingsDefaultHLayout2->addWidget(VideoCfgIntroDelay);
    settingsDefaultHLayout2->addWidget(ImageQualityLabel);
    settingsDefaultHLayout2->addWidget(VideoCfgImageQuality);

    QHBoxLayout *settingsDefaultLayout = new QHBoxLayout;
    settingsDefaultLayout->addLayout(settingsDefaultHLayout1);
    settingsDefaultLayout->addLayout(settingsDefaultHLayout2);


    settingsDefaultGroup->setLayout(settingsDefaultLayout);



    QGroupBox *settingsCustomGroup = new QGroupBox("Custom video Settings");
    settingsCustomGroup->setEnabled(false);

    QLabel *VideoCfgAlternativeScriptLabel = new QLabel("Choose alternative ffmpeg script (Read documentation for how-to info):");
    VideoCfgAlternativeScript = new QLineEdit;
    VideoCfgAlternativeScript->setMinimumWidth(400);
    QPushButton *chooseScriptPath = new QPushButton("Choose");

    connect(chooseScriptPath, SIGNAL(clicked()), this, SLOT(chooseAlternativeScript()));

    QHBoxLayout *settingsCustomHLayout = new QHBoxLayout;
    settingsCustomHLayout->addWidget(VideoCfgAlternativeScript);
    settingsCustomHLayout->addSpacing(15);
    settingsCustomHLayout->addWidget(chooseScriptPath);

    QVBoxLayout *settingsCustomLayout = new QVBoxLayout;
    settingsCustomLayout->addWidget(VideoCfgAlternativeScriptLabel);
    settingsCustomLayout->addLayout(settingsCustomHLayout);


    settingsCustomGroup->setLayout(settingsCustomLayout);


    connect(VideoCfgDefaultRadio, SIGNAL(toggled(bool)), settingsDefaultGroup, SLOT(setEnabled(bool)));
    connect(VideoCfgCustomRadio, SIGNAL(toggled(bool)), settingsCustomGroup, SLOT(setEnabled(bool)));

    VideoCfgDefaultRadio->setChecked(true);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(grabFrameBuffer()));
    connect(buttonBox, SIGNAL(rejected()), videoDialog, SLOT(hide()));

    QVBoxLayout *videoDialogMainLayout = new QVBoxLayout;
    videoDialogMainLayout->addWidget(pathGroup);
    videoDialogMainLayout->addWidget(videoModeGroup);
    videoDialogMainLayout->addWidget(settingsDefaultGroup);
    videoDialogMainLayout->addWidget(settingsCustomGroup);
    videoDialogMainLayout->addWidget(buttonBox);

    videoDialog->setLayout(videoDialogMainLayout);
    videoDialog->setWindowTitle("Generate video");

    videoDialog->show();

    //videoDialog->exec(); // If u want this as a modal
}


void MainWindow::grabFrameBuffer()
{
    QFileInfo fileInfo(fileName);
    QFileInfo saveTofileInfo(VideoCfgSaveToLineEdit->text());

    // FIX: Time 0 not being screenshoted by some reason..
    // TODO: Run ffmpeg at screenshot`s folder and create video at parent path √
    // TODO: Get settings dialog done to set video options and alternative ffmpeg script √

    for( unsigned i = 0; i <= layer_groups.size()-1; i++) {
        active_layer_group = i;
        ui->glwidget->updateGL();

        QImage image = ui->glwidget->grabFrameBuffer();
        QDir save_dir;
        save_dir.mkdir(saveTofileInfo.filePath() + "/");
        image.save(saveTofileInfo.filePath() + "/img" + QString::number(i) + ".jpg", 0, VideoCfgImageQuality->value());
    }

     QImage *video_splash = new QImage(":/images/bg.jpg");
     video_splash->scaled(ui->glwidget->grabFrameBuffer().size());

     QString intro_file = saveTofileInfo.filePath() + "/intro.png";
     video_splash->save(intro_file);

     if(VideoCfgDefaultRadio->isChecked()) {

         QString extension = "mpg";
         QString file1 = saveTofileInfo.filePath() + "/" + "part1" + "." + extension;
         QString file2 = saveTofileInfo.filePath() + "/" + "intro" + "." + extension;
         QString file3 = saveTofileInfo.filePath() + "/" + fileInfo.baseName() + "." + extension;


        QString scale = QString("scale=%1:-1").arg(QString::number(ui->glwidget->grabFrameBuffer().width()));

        QString fadeIn;
        if(VideoCfgFadeInIntro->isChecked()) {
            int sec = VideoCfgIntroDelay->value()*10;
            fadeIn = QString(",fade=in:0:%1 [out]").arg(QString::number(sec));
        }

        QString effects = QString("-vf '%1%2' ").arg(scale,fadeIn);

        if(VideoCfgShowIntro->isChecked()) {
            QString ffmpeg_step1 = QString("ffmpeg -y -r %1 -start_number %2 -i %3/img%d.jpg -r %4 %5").arg(QString::number(VideoCfgSpeed->value()),QString::number(VideoCfgStartFrom->value()),saveTofileInfo.filePath(),QString::number(VideoCfgFPS->value()),file1);
            system(ffmpeg_step1.toStdString().c_str());

            QString ffmpeg_step2 = QString("ffmpeg -y -loop 1 -f image2 -i %1 -t %2 %3 %4").arg(intro_file,QString::number(VideoCfgIntroDelay->value()),effects,file2);
            system(ffmpeg_step2.toStdString().c_str());


            QString ffmpeg_step3 = QString("ffmpeg -y -i 'concat:%1|%2' -c copy %3").arg(file2,file1,file3);
            system(ffmpeg_step3.toStdString().c_str());
        } else {
            QString ffmpeg_step1 = QString("ffmpeg -y -r %1 -start_number %2 -i %3/img%d.jpg -r %4 %5").arg(QString::number(VideoCfgSpeed->value()),QString::number(VideoCfgStartFrom->value()),saveTofileInfo.filePath(),QString::number(VideoCfgFPS->value()),file3);
            system(ffmpeg_step1.toStdString().c_str());
        }

        if(VideoCfgRemoveJunk->isChecked()) {
            QFile rm1(file1);
            QFile rm2(file2);
            QFile rm3(intro_file);
            rm1.remove();
            rm2.remove();
            rm3.remove();

            for( unsigned i = 0; i <= layer_groups.size()-1; i++) {
                QFile file(saveTofileInfo.filePath() + "/img" + QString::number(i) + ".jpg");
                file.remove();
            }
        }

        QString openFolder = QString("open %1").arg(saveTofileInfo.filePath());
        if(VideoCfgOpenFolder->isChecked())
            system(openFolder.toStdString().c_str());

        if(VideoCfgCloseAfterRendering->isChecked())
            videoDialog->hide();
     } else {

         QFile file(VideoCfgAlternativeScript->text());
         if(!file.open(QIODevice::ReadOnly)) {
             QMessageBox::information(0, "error", file.errorString());
         }

         QTextStream in(&file);
         QString script = in.readAll();

        file.close();

        QFileInfo scriptInfo(VideoCfgAlternativeScript->text());

        QString script_path(scriptInfo.absolutePath());
        QString imgs_path(saveTofileInfo.filePath() + "/img%d.jpg");
        QString save_to_path(saveTofileInfo.filePath());

        QString ffmpeg_step1 = QString(script).arg(imgs_path,save_to_path,script_path,intro_file);
        system(ffmpeg_step1.toStdString().c_str());

        videoDialog->hide();

        QString openFolder = QString("open %1").arg(saveTofileInfo.filePath());
        system(openFolder.toStdString().c_str());
     }

   // ffmpeg is so weird........... as c++, everything so boring, deal with that
}

void MainWindow::chooseSavePath() {
    QFileInfo fileInfo(fileName);
    QString saveTofileName = QFileDialog::getExistingDirectory(this, "Save video to", QDir::homePath() + fileInfo.baseName() + ".avi");

    if(!saveTofileName.isEmpty())
    {
        VideoCfgSaveToLineEdit->setText(saveTofileName);
    }

}

void MainWindow::chooseAlternativeScript() {
    QFileInfo fileInfo(fileName);
    QString alternativeScript = QFileDialog::getOpenFileName(this, "Open alternative script", QDir::homePath());

    if(!alternativeScript.isEmpty())
    {
        VideoCfgAlternativeScript->setText(alternativeScript);
    }

}


void MainWindow::layerSet(int activeSet)
{
    active_layer_group = activeSet-1;
    displayed_layer->setText("Selected LayerSet: " + QString::number(activeSet)); // not being shown
    sliderDock->setWindowTitle("LayerSet: " + QString::number(activeSet));
    ui->glwidget->updateGL();
}

void MainWindow::stateChanged(QListWidgetItem *cur_item)
{

    if(cur_item->checkState()) {
            active_layers.push_back(layerList->row(cur_item));
    }
     else {
            active_layers.remove(layerList->row(cur_item));
    }

    ui->glwidget->updateGL();
}

void MainWindow::setColor(QModelIndex index) {
    const QColor color = QColorDialog::getColor(Qt::green, this, "Select Color");
    if(color.isValid()) {

        for (unsigned int i=0; i<layer_groups.size(); ++i){
			int from_j = all_layers ? 0 : index.row(); 
			int to_j = all_layers ? layer_groups[i].size() : index.row() + 1;
			for (int j=from_j; j<to_j; ++j){
				layer_groups[i][j].r = color.red() / 255.0;
				layer_groups[i][j].g = color.green() / 255.0;
				layer_groups[i][j].b = color.blue() / 255.0;
			}
        }
    }
}



bool MainWindow::eventFilter( QObject* object, QEvent* event )
{
  if( object == layerDock )
  {
      if( event->type() == QEvent::Move)
        {
          /*
         layerDock->setWindowOpacity(0.8);
         QMessageBox::about(this, "UHU", "OH EAH");
         QString str;
         layerList->addItem(str.setNum(layerDock->pos().x()));
         */
        }
  }

  return false;
}


void MainWindow::keyPressEvent(QKeyEvent *event){
	if (event->key() == Qt::Key_Z) 		show_axis = !show_axis;
	if (event->key() == Qt::Key_X) 		show_grid = !show_grid;
	if (event->key() == Qt::Key_C) 		show_wave = !show_wave;
    if (event->key() == Qt::Key_J) 		cam_radius += 0.2;
    if (event->key() == Qt::Key_K) 		cam_radius -= 0.2;
    if (event->key() == Qt::Key_U) 		slider->setValue(slider->value()-1);
    if (event->key() == Qt::Key_I) 		slider->setValue(slider->value()+1);
    if (event->key() == Qt::Key_Right) 	rotate_y += 5;
    if (event->key() == Qt::Key_Left) 	rotate_y -= 5;
    if (event->key() == Qt::Key_Down) 	rotate_x += 5;
    if (event->key() == Qt::Key_Up) 	rotate_x -= 5; 
    if (event->key() == Qt::Key_S) 		center_pos.y += 0.2;
    if (event->key() == Qt::Key_W) 		center_pos.y -= 0.2;
    if (event->key() == Qt::Key_A) 		center_pos.x += 0.2;
    if (event->key() == Qt::Key_D) 		center_pos.x -= 0.2;
    if(event->key() >= Qt::Key_1 && event->key() <= Qt::Key_9) {
        for( unsigned i = 0; i < layer_groups[active_layer_group].size(); i++) {
           if(QString::number(i+1) == QKeySequence(event->key()).toString(QKeySequence::NativeText)) {
               if(layerList->item(i)->checkState())
                   layerList->item(i)->setCheckState(Qt::Unchecked);
               else
                   layerList->item(i)->setCheckState(Qt::Checked);
           }
        }
    }

    ui->glwidget->updateGL();
}


MainWindow::~MainWindow(){
    delete ui;
}


void MainWindow::on_actionOpen_triggered()
{

    fileName = QFileDialog::getOpenFileName(this,"Choose a .v3d file",QDir::homePath()+"/*.v3d","3D Multilayer-Viewer(*.v3d)");

    if(!fileName.isEmpty())
    {

        ui->glwidget->open_graphic(fileName.toStdString());

        QFileInfo fileInfo(fileName);
        QString only_fileName(fileInfo.fileName());
        this->setWindowTitle("3D Mulilayer Viewer - " + only_fileName);

            if(!ui->glwidget->isHidden())
        {
            delete layerDock;
            delete sliderDock;
        }

        createDockWindows();

        QString str;
        for(unsigned int i = 0; i< layer_groups[0].size(); i++)
        {
            item = new QListWidgetItem("Layer "+ str.setNum(i+1), layerList);
            item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
            item->setCheckState(Qt::Checked);
        }

        ui->glwidget->show();
        ui->splash_screen->hide();
        // displayed_layer->show(); // being shown way fancier at slider windowTitle ;)

    }
}
