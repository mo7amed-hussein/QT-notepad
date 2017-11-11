#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    //create editor
    editor = new QTextEdit;
    //fill window with our editor
    this->setCentralWidget(editor);
    //resize notepad to 600X800
    this->resize(800,600);

    createStatusBar();



}

void MainWindow::createStatusBar()
{
    //statusbar widget
    cursorPosition = new QLabel;
    cursorPosition->setText("Ln: 0 , Col: 0 ");
    cursorPosition->setAlignment(Qt::AlignHCenter);
    cursorPosition->setFrameStyle(QFrame::NoFrame);
    this->statusBar()->addPermanentWidget(cursorPosition,0);
    statusBar()->adjustSize();
}

void MainWindow::updateStatusBar(int l,int c)
{
    QString str = tr("Ln: %1 , Col: %2 ").arg(l).arg(c);
    cursorPosition->setText(str);
}

void MainWindow::createNewDoc()
{
    fileName= "";
    editor->clear();
}
