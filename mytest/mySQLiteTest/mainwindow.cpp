#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/qsqlquerymodel.h>
#include <QSqlQuery>
#include <QSqlQueryModel>

QSqlQuery query;
QSqlDatabase db;
QSqlQueryModel *model;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(ui->tableView,SIGNAL(clicked(QModelIndex)),this,SLOT(table_view_clicked(QModelIndex)));//tableView单击事件
}
void MainWindow::table_view_clicked(const QModelIndex &index)//tableView单击事件的绑定
{

    int curRow  = index.row();
    QModelIndex indextemp   = model->index(curRow,1);//遍历行的列
    QString datatemp        = model->data(indextemp).toString();
    ui->lineEdit_5->setText(datatemp);

    indextemp   = model->index(curRow,2);
    datatemp    = model->data(indextemp).toString();
    ui->lineEdit_6->setText(datatemp);




}
MainWindow::~MainWindow()
{
    db.close();
    delete ui;

}

void MainWindow::on_pushButton_2_clicked()//打开数据库
{
    dbopen("mydb",ui->lineEdit_4->text());
    //dbopen("mydb");
}

void MainWindow::on_pushButton_clicked()//添加
{

    dbnew("students",ui->lineEdit->text(),ui->lineEdit_2->text(),ui->lineEdit_3->text());
}

void MainWindow::on_pushButton_3_clicked()//选中删除
{
    int _row=ui->tableView->currentIndex().row();
    if(_row==-1){
        return;
    }
    QModelIndex index = model->index(_row,0);
    QString ID = model->data(index).toString();
     qDebug() << ID;
    dbDelete("students",ID);

}

void MainWindow::on_pushButton_4_clicked()//修改
{
    int _row=ui->tableView->currentIndex().row();
    if(_row==-1){
     return;
    }
    QModelIndex index = model->index(_row,0);
    QString ID = model->data(index).toString();//得到选中列表的ID

    dbUpdate("students",ID,ui->lineEdit_5->text(),ui->lineEdit_6->text());

}
void MainWindow::on_pushButton_5_clicked()//查询
{
    dbshow("students",ui->lineEdit_7->text());
}
void MainWindow::dbDelete(QString form,QString ID){ //DB删除

    query.prepare(QString("delete from "+form+" where xingming = ?"));
    query.addBindValue(ID);
    if(! query.exec())
    {
          qDebug() << query.lastError();
    }
    else
    {
          qDebug() << "inserted Wang!";

    }
}
void MainWindow::dbUpdate(QString form,QString xingming,QString xuehao,QString chengji)//修改更新内容
{


    QString update_sql = "update "+form+" set xuehao = :xuehao,chengji = :chengji where xingming = :xingming";
    query.prepare(update_sql);
    query.bindValue(":xuehao", xuehao);
    query.bindValue(":chengji", chengji);
    query.bindValue(":xingming",xingming);

    if(!query.exec())
    {
        qDebug() << query.lastError();
    }
    else
    {
        qDebug() << "updated!";
    }
}
void MainWindow::dbnew(QString form,QString xingming,QString xuehao,QString chengji)//添加数据（表单名+内容）
{
    bool query_OK= query.exec("create table "+form+"(xingming vchar primary key,xuehao vchar ,chengji vchar )");//新建表单
    if(query_OK)
    {
        qDebug()<< "新建form成功!";
    }
    else
    {
        qDebug()<< "新建form失败!"  << db.lastError();

    }
    QString insert_buf;
    insert_buf=QObject::tr(" values('%1','%2','%3')").arg(xingming,xuehao,chengji);
    insert_buf="insert into "+form+insert_buf;
    query_OK= query.exec(insert_buf);
    if(query_OK)
    {
        qDebug()<< "添加数据成功!";
    }
    else
    {
        qDebug()<< "添加数据失败!"  << db.lastError();

    }
}

void MainWindow::dbshow(QString form,QString Qfind)//查询显示数据到tableView（表单名，查询内容）
{


    model = new QSqlQueryModel(this);

    if(Qfind.isEmpty()){
        Qfind="SELECT * FROM "+form;
    }else{
        Qfind="SELECT * FROM "+form+" WHERE (xingming='"+Qfind+"')"+
                                        "OR (xuehao  ='"+Qfind+"')"+
                                        "OR (chengji ='"+Qfind+"')";
    }

    model->setQuery(Qfind);
    model->setHeaderData(0,Qt::Horizontal,tr("姓名"));
    model->setHeaderData(1,Qt::Horizontal,tr("学号"));
    model->setHeaderData(2,Qt::Horizontal,tr("成绩"));
    ui->tableView->setModel(model);
}
void MainWindow::dbopen(QString dbname){//打开数据库

    db=QSqlDatabase::addDatabase("QSQLITE");
    dbname=dbname+".db";
    db.setDatabaseName(dbname);
    bool ok=db.open();
    if(ok)
    {
       QMessageBox::information(this,"Information","数据库打开成功");
    }
    else
    {
       QMessageBox::information(this,"Information","数据库打开失败");
    }
    query = QSqlQuery(db);
}
void MainWindow::dbopen(QString dbname,QString password){//打开数据库

    db=QSqlDatabase::addDatabase("SQLITECIPHER");
    dbname=dbname+".db";
    db.setDatabaseName(dbname);
    db.setPassword(password);
    //db.setConnectOptions("QSQLITE_CREATE_KEY");
    bool ok=db.open();
    if(ok)
    {
       QMessageBox::information(this,"Information","数据库打开成功");
    }
    else
    {
       QMessageBox::information(this,"Information","数据库打开失败");
    }
    query = QSqlQuery(db);
}

