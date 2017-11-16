#include "mainwindow.h"
#include<QTextStream>
#include<QMenu>
#include<QMenuBar>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    //create editor
    editor = new QTextEdit;
    //fill window with our editor
    this->setCentralWidget(editor);
    //resize notepad to 600X800
    this->resize(800,600);

    createStatusBar();
    createMenuBar();
    connect(editor,&QTextEdit::cursorPositionChanged,this,&MainWindow::retCursorPos);

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

void MainWindow::openDocument()
{
    QString filePath= QFileDialog::getOpenFileName(this,"select text file ",QDir::currentPath(),"All files (*.*);;");
    if(filePath != "")
    {
        createNewDoc();
        QFile file(filePath);
         file.open(QIODevice::Text | QIODevice::ReadOnly);
         QTextStream in(&file);
         while (! in.atEnd()) {
            editor->append(in.readLine());
         }
         fileName= filePath;
    }
}

void MainWindow::saveDocument()
{
    QFile file(fileName);
    if(file.open(QFile::Text | QFile::WriteOnly))
    {
        QTextStream out(&file);
        out<<editor->toPlainText();
        file.close();
    }

}

void MainWindow::retCursorPos()
{
    QTextCursor cur = editor->textCursor();
    updateStatusBar(cur.blockNumber(),cur.columnNumber());
}

void MainWindow::createMenuBar()
{
    //create file menu
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    //new action
    QAction *newAc = new QAction(tr("New"),this);
    newAc->setShortcut(QKeySequence::New);
    connect(newAc,&QAction::triggered,this,&MainWindow::createNewDoc);
    fileMenu->addAction(newAc);

    //open action
    QAction *openAc = new QAction(tr("Open"),this);
    openAc->setShortcut(QKeySequence::Open);
    connect(openAc,&QAction::triggered,this,&MainWindow::openDocument);
    fileMenu->addAction(openAc);

    //save action
    QAction *saveAc = new QAction(tr("Save"),this);
    saveAc->setShortcut(QKeySequence::Save);
    connect(saveAc,&QAction::triggered,this,&MainWindow::saveDocument);
    fileMenu->addAction(saveAc);

    //save as action
    QAction *saveasAc = new QAction(tr("SaveAs"),this);
    saveasAc->setShortcut(QKeySequence::SaveAs);
    connect(saveasAc,&QAction::triggered,this,&MainWindow::saveDocument);
    fileMenu->addAction(saveasAc);
    fileMenu->addSeparator();

    //print action
    QAction *printAc = new QAction(tr("Print"),this);
    printAc->setShortcut(QKeySequence::Print);
 //   connect(saveasAc,&QAction::triggered,this,&MainWindow::saveDocument);
    fileMenu->addAction(printAc);
    fileMenu->addSeparator();

    //exit action
    QAction *exitAc = new QAction(tr("Exit"),this);
    exitAc->setShortcut(QKeySequence::Quit);
//    connect(saveasAc,&QAction::triggered,this,&MainWindow::saveDocument);
    fileMenu->addAction(exitAc);


}
