#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<QFileDialog>
#include <QMainWindow>
#include <QTextEdit>
#include<QLabel>
#include<QStatusBar>
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    void createMenuBar();
    void createStatusBar();
    void createNewDoc();
    void openDocument();
    void saveDocument();
    void saveDocumentAs();
    void paste();
    void echoTime();
    void selectAll();
    void gotoLine();
protected:

   virtual bool event(QEvent * event);
   virtual void closeEvent(QCloseEvent * event);
signals:

public slots:
    void updateStatusBar(int l,int c);
    void retCursorPos();
    void quitApp();
    void print();
    void undo();
    void cut();
    void copy();
    void wordwrap();
    void fontdialog();
    void toggleStatusBar();
    void aboutPad();
    void checkClipboard();
    void deleteSelection();
private:
    QTextEdit * editor;//text editor
    QLabel *cursorPosition;//editor cursor position label
    QString fileName;
    QString filePath;

    QAction *cutAc;
    QAction *copyAc;
    QAction *pasteAc;
    QAction *undoAc;
    QAction *delAc;
    QFont padFont;
};

#endif // MAINWINDOW_H
