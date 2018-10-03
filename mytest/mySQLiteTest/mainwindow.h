#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void dbshow(QString form,QString Qfind);
    void dbnew (QString form,QString xingming,QString xuehao,QString chengji);
    void dbopen(QString dbname);
    void dbopen(QString dbname,QString password);
    void dbUpdate(QString form,QString xingming,QString xuehao,QString chengji);
    void dbDelete(QString form,QString ID);


private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void table_view_clicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
