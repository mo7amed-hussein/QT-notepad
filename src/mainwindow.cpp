#include "mainwindow.h"
#include<QTextStream>
#include<QMenu>
#include<QMenuBar>
#include<QMessageBox>
#include<QApplication>
#include<QPrintDialog>
#include<QPrinter>
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
    fileName= "New";
    filePath= "";
    this->setWindowTitle(fileName+" :: NotePad");
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
         this->filePath= filePath;
         fileName= file.fileName();
         this->setWindowTitle(fileName+" :: NotePad");
    }
}

void MainWindow::saveDocument()
{
    if(filePath != "")
    {

        QFile file(filePath);
        if(file.open(QFile::Text | QFile::WriteOnly))
        {
            QTextStream out(&file);
            out<<editor->toPlainText();
            file.close();
        }
    }
    else
    {
        saveDocumentAs();
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
    connect(printAc,&QAction::triggered,this,&MainWindow::print);
    fileMenu->addAction(printAc);
    fileMenu->addSeparator();

    //exit action
    QAction *exitAc = new QAction(tr("Exit"),this);
    exitAc->setShortcut(QKeySequence::Quit);
    connect(exitAc,&QAction::triggered,this,&MainWindow::quitApp);
    fileMenu->addAction(exitAc);


}

void MainWindow::quitApp()
{
    QMessageBox msg(this);
    msg.setText("close Notepad ");
    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    int ret = msg.exec();
    if(ret== QMessageBox::Yes)
    {
        qApp->quit();
    }
}

void MainWindow::print()
{
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog printDig(&printer,this);
    if(printDig.exec() == QDialog::Accepted)
    {
        editor->print(&printer);

    }
}

void MainWindow::saveDocumentAs()
{
    QString newFileName=QFileDialog::getSaveFileName(this,tr("save as"));
        if(newFileName.isEmpty())
            return;
        this->filePath = newFileName;
        this->fileName = newFileName;
        this->setWindowTitle(fileName+" :: NotePad");
        saveDocument();
}
