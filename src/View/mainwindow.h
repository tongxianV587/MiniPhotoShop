#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../Constants.h"
#include "../Common/DataStructure.h"
#include <QMainWindow>
#include "../Common/Observer.h"
#include "../Common/BaseCommand.h"
#include "../View/imagewidget.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow, public Observer {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

    void setAddLineCommand(const shared_ptr<BaseCommand> &addLineCommand);
    void setAddEllipseCommand(const shared_ptr<BaseCommand> &addEllipseCommand);

    void setNewCanvasCommand(const shared_ptr<BaseCommand> &newCanvasCommand);
    void setPenUpdateCommand(const shared_ptr<BaseCommand> &penUpdateCommand);
    void SetPen(const Pen* pen);
    void SetBrush(const Brush* brush);
    void SetDisplayImage(const QImage* displayImage);
    virtual void update(Params params);

private:
    Ui::MainWindow *ui;
    const Pen* pen;
    const Brush* brush;
    const QImage* displayImage;
    int state;
     shared_ptr<BaseCommand> newCanvasCommand,penUpdateCommand;
 public slots:
     void menuTriggered(QAction*);   //响应菜单栏事件
     void StateChanged();
     void ButtonForeColorPressed();
     void ButtonBackColorPressed();
};

#endif // MAINWINDOW_H
