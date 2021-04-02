#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QStandardPaths>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QStringList list{"item 1", "item 2", "item 3", "item 4", "item 5", "item 6"};
    ui->listWidget->addItems(list);
    qputenv("GST_DEBUG", "*:3");
    gst_init(nullptr, nullptr);
    playbin = gst_element_factory_make("playbin", "playbin");
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButtonPlay_clicked()
{
    auto uri = gst_filename_to_uri(ui->lineEdit->text().toLocal8Bit(), nullptr);
    g_object_set(playbin, "uri", uri, nullptr);
    gst_element_set_state(playbin, GST_STATE_PLAYING);
}

void MainWindow::on_pushButton_clicked()
{
    gst_element_set_state(playbin, GST_STATE_NULL);
}

void MainWindow::on_pushButtonBrowse_clicked()
{
    auto fileName = QFileDialog::getOpenFileName(this,
       "Open Audio File", QStandardPaths::locate(QStandardPaths::MusicLocation, "", QStandardPaths::LocateDirectory), "Audio Files (*.mp3 *.ogg *.wav)");
    ui->lineEdit->setText(fileName);
}
