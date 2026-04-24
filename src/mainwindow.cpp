#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);
    connect(m_ui->actionLoad, &QAction::triggered, this, &MainWindow::loadAudioSlot);
    connect(m_ui->pushButton, &QPushButton::pressed, this, &MainWindow::onDebug);
    m_file = nullptr;
}

MainWindow::~MainWindow()
{
    delete m_ui;
    delete m_file;
}

void MainWindow::loadAudioSlot(){

    //prompt the user to open a file
    QFileDialog *d = new QFileDialog(this);
    d->exec();

    //get the result of the dialog
    QStringList r = d->selectedFiles();

    //if they don't select anything, don't do anything
    if(r.isEmpty()) return;

    //set up the audio format
    QAudioFormat fmt;
    fmt.setChannelCount(2);
    fmt.setSampleFormat(QAudioFormat::Int32);
    fmt.setSampleRate(48000);

    //read the file and create a new AudioFile object
    m_file = new AudioFile(r[0], fmt, this);

    //start decoding the audio file
    m_file->startDecode();

}

void MainWindow::onDebug()
{
    if(!m_file) {
        qDebug()<<"No file loaded";
        return;
    }
    qDebug()<<m_file->isDecoding();
}

