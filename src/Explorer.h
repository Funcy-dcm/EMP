#ifndef EXPLORER_H
#define EXPLORER_H

#include <QtGui/QTableView>
#include <QtGui/QHeaderView>
#include <QtGui/QDirModel>

class ExplorerWidget : public QTableView
{
    Q_OBJECT
public:
    ExplorerWidget(QWidget *parent = 0);
    ~ExplorerWidget();

protected:

public slots:

private slots:

private:
    QDirModel *model;

};

#endif // EXPLORER_H
