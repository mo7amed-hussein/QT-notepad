#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include<QLabel>
#include<QStatusBar>
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);

    void createStatusBar();

signals:

public slots:
    void updateStatusBar(int l,int c);
private:
    QTextEdit * editor;//text editor
    QLabel *cursorPosition;//editor cursor position label
};

#endif // MAINWINDOW_H
