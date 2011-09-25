#include "Explorer.h"

ExplorerWidget::ExplorerWidget(QWidget *parent)
    : QTableView(parent)
{
    setObjectName("ExplorerWidget");
    model = new QDirModel(this);
    model->setSorting(QDir::DirsFirst);

    setModel(model);

    QModelIndex index = model->index("e:\\");
    setRootIndex(index);
    setColumnHidden(1, true);
    setColumnHidden(2, true);
    setColumnHidden(3, true);
    this->horizontalHeader()->hide();
    verticalHeader()->hide();
    setSelectionBehavior( QAbstractItemView::SelectRows );
    setSelectionMode(QAbstractItemView::SingleSelection);
    horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
    setFocusPolicy(Qt::NoFocus);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QObject::connect(this, SIGNAL(activated(const QModelIndex&)),
                     this,  SLOT(setRootIndex(const QModelIndex&)));

}

ExplorerWidget::~ExplorerWidget()
{

}
