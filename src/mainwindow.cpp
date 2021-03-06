#include "mainwindow.h"
#include<QTextStream>
#include<QMenu>
#include<QMenuBar>
#include<QMessageBox>
#include<QApplication>
#include<QPrintDialog>
#include<QPrinter>
#include<QDateTime>
#include<QInputDialog>
#include<QFontDialog>
#include<QClipboard>
#include<QMimeData>
#include<QDebug>
#include<QCloseEvent>

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
    connect(editor,&QTextEdit::copyAvailable,copyAc,&QAction::setEnabled);
    connect(editor,&QTextEdit::copyAvailable,cutAc,&QAction::setEnabled);
    connect(editor,&QTextEdit::copyAvailable,delAc,&QAction::setEnabled);

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

    //create file menu
    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));

    //undo action
    undoAc = new QAction(tr("Undo"),this);
    undoAc->setShortcut(QKeySequence::Undo);
    undoAc->setEnabled(false);
    connect(undoAc,&QAction::triggered,this,&MainWindow::undo);
    editMenu->addAction(undoAc);

    editMenu->addSeparator();

    //cut action
    cutAc = new QAction(tr("Cut"),this);
    cutAc->setShortcut(QKeySequence::Cut);
    cutAc->setEnabled(false);
    connect(cutAc,&QAction::triggered,this,&MainWindow::cut);
    editMenu->addAction(cutAc);

    //copy action
    copyAc = new QAction(tr("Copy"),this);
    copyAc->setShortcut(QKeySequence::Copy);
    copyAc->setEnabled(false);
    connect(copyAc,&QAction::triggered,this,&MainWindow::copy);
    editMenu->addAction(copyAc);

    //paste action
    pasteAc = new QAction(tr("Paste"),this);
    pasteAc->setShortcut(QKeySequence::Paste);
    pasteAc->setEnabled(false);
    connect(pasteAc,&QAction::triggered,this,&MainWindow::paste);
    editMenu->addAction(pasteAc);

    //delete action
    delAc = new QAction(tr("Delete"),this);
    delAc->setShortcut(QKeySequence::Delete);
    delAc->setEnabled(false);
    connect(delAc,&QAction::triggered,this,&MainWindow::deleteSelection);
    editMenu->addAction(delAc);

    editMenu->addSeparator();

    //go to line action
    QAction * lineAc = new QAction(tr("Go To .."),this);
    connect(lineAc,&QAction::triggered,this,&MainWindow::gotoLine);
    editMenu->addAction(lineAc);

    editMenu->addSeparator();

    //selectall action
    QAction *selectAc = new QAction(tr("SelectAll"),this);
    selectAc->setShortcut(QKeySequence::SelectAll);
    connect(selectAc,&QAction::triggered,this,&MainWindow::selectAll);
    editMenu->addAction(selectAc);

    //time/date action
    QAction *timeAc = new QAction(tr("Time/Date"),this);
    timeAc->setShortcut(QKeySequence::Refresh);
    connect(timeAc,&QAction::triggered,this,&MainWindow::echoTime);
    editMenu->addAction(timeAc);

    //create formst menu
    QMenu *formatMenu = menuBar()->addMenu(tr("&Format"));
    //word wrap action
    QAction *wrapAc = new QAction(tr("Word Wrap"),this);
    wrapAc->setCheckable(true);
    wrapAc->setChecked(false);
    connect(wrapAc,&QAction::triggered,this,&MainWindow::wordwrap);
    formatMenu->addAction(wrapAc);

    //font action
    QAction *fontAc = new QAction(tr("Font"),this);
    connect(fontAc,&QAction::triggered,this,&MainWindow::fontdialog);
    formatMenu->addAction(fontAc);

    //create view menu
    QMenu *viewMenu = menuBar()->addMenu(tr("&View"));
    //status bar action
    QAction *statusAc = new QAction(tr("Status Bar"),this);
    statusAc->setCheckable(true);
    statusAc->setChecked(true);
    connect(statusAc,&QAction::triggered,this,&MainWindow::toggleStatusBar);
    viewMenu->addAction(statusAc);

    //create help menu
    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    QAction *aboutAc = new QAction(tr("About"),this);
    connect(aboutAc,&QAction::triggered,this,&MainWindow::aboutPad);
    helpMenu->addAction(aboutAc);

    QAction *aboutqtAc = new QAction(tr("About"),this);
    connect(aboutqtAc,&QAction::triggered,qApp,&QApplication::aboutQt);
    helpMenu->addAction(aboutqtAc);

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

void MainWindow::undo()
{
    editor->undo();
}

void MainWindow::cut()
{
    editor->cut();
}

void MainWindow::copy()
{
    editor->copy();
}

void MainWindow::paste()
{
    editor->paste();
}

void MainWindow::echoTime()
{
    QDateTime time = QDateTime::currentDateTimeUtc();
    QString str = time.toString("dd/mm/yyyy - HH:mm:ss A");
    editor->append(str);
}

void MainWindow::selectAll()
{
    editor->selectAll();
}

void MainWindow::gotoLine()
{
    bool ok;
    QString line = QInputDialog::getText(this,"Go To Line","Line Number",QLineEdit::Normal,"1",&ok);
    if(ok && !line.isEmpty())
    {
        //....
    }
}

void MainWindow::wordwrap()
{
    QTextOption::WrapMode old=editor->wordWrapMode();
    if( old != QTextOption::NoWrap )
    {
        editor->setWordWrapMode(QTextOption::WordWrap);
    }
    else
    {
      editor->setWordWrapMode(QTextOption::NoWrap);
    }

}

void MainWindow::fontdialog()
{
    QFont curFont = editor->font();
    bool ok;
    QFont newFont = QFontDialog::getFont(&ok,curFont,this);
    if(ok)
    {
        padFont= newFont;
        editor->setFont(padFont);
    }
}

void MainWindow::toggleStatusBar()
{
    if(this->statusBar()->isVisible())
    {
        this->statusBar()->setVisible(false);
    }
    else
    {
        this->statusBar()->setVisible(true);
    }
}

void MainWindow::aboutPad()
{
    QMessageBox::about(this,"about Notepad","<b>Notepad</b> was developed by <p> Mohamed Hussein <p> Nov,2017");
}

bool MainWindow::event(QEvent * event)
{
   if(event->type() == QEvent::Clipboard)
   {
       checkClipboard();
   }
 return QMainWindow::event(event);
}

void MainWindow::checkClipboard()
{
    qDebug()<<"clipboard";
    const QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();
    if(mimeData->hasHtml()|mimeData->hasText()|mimeData->hasUrls())
    {
        pasteAc->setEnabled(true);
    }
    else
    {
       pasteAc->setEnabled(false);
    }
}

void MainWindow::closeEvent(QCloseEvent * event)
{
    quitApp();
    event->ignore();
}

void MainWindow::deleteSelection()
{
    editor->textCursor().removeSelectedText();
}
