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
signals:

public slots:
    void updateStatusBar(int l,int c);
    void retCursorPos();
private:
    QTextEdit * editor;//text editor
    QLabel *cursorPosition;//editor cursor position label
    QString fileName;
};

#endif // MAINWINDOW_H
