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

    //attempt to open the file
    QFile file = QFile(r[0]);
    file.open(QIODevice::ReadOnly);

    //notify the user if the file cannot be opened
    if(!file.isOpen()){
        QMessageBox::critical(this, "Error", "Unable to open file!");
        return;
    }

    //read the file and create a new AudioFile object
    m_file = new AudioFile(new QByteArray(file.readAll()), r[0], this);

    //observe the niceties (close the file)
    file.close();

    //start decoding the audio file
    m_file->startDecode();

}
