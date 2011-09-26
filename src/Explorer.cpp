
#include "Explorer.h"

ExplorerWidget::ExplorerWidget(MediaPlayer *player, QWidget *parent) :
    QTableView(parent),
    m_player(player)
{
    setObjectName("ExplorerWidget");
    model = new QDirModel(this);
    model->setSorting(QDir::DirsFirst);

    setModel(model);

    QModelIndex index = model->index("d:\\");
    setRootIndex(index);
    setColumnHidden(1, true);
    setColumnHidden(2, true);
    setColumnHidden(3, true);
    this->horizontalHeader()->hide();
    verticalHeader()->hide();
    setSelectionBehavior( QAbstractItemView::SelectRows );
    setSelectionMode(QAbstractItemView::SingleSelection);
    horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
    setShowGrid(false);
    setFocusPolicy(Qt::NoFocus);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QObject::connect(this, SIGNAL(activated(const QModelIndex&)),
                     this,  SLOT(slotSetIndex(QModelIndex)));

    oldIndex = rootIndex();
    selectRow(0);
}

ExplorerWidget::~ExplorerWidget()
{

}

/*virtual*/ void ExplorerWidget::keyPressEvent(QKeyEvent* pe)
{
    if (pe->key() == Qt::Key_Left) {
        qDebug() << "Backspace";
    }
}

void ExplorerWidget::slotSetIndex(const QModelIndex& newIndex)
{
    if (model->isDir(newIndex)) setRootIndex(newIndex);
    else {
        QString filePath = model->filePath(newIndex);
//        m_player->addFile(filePath);
        m_player->setCurrentSource(filePath, true);
    }
    oldIndex = newIndex;
    selectRow(0);
}

void ExplorerWidget::slotKeyLeft()
{
    setRootIndex(model->parent(oldIndex));
    selectRow(oldIndex.row());
    oldIndex = model->parent(oldIndex);
}

void ExplorerWidget::slotKeyRight()
{
    slotSetIndex(currentIndex());
}

void ExplorerWidget::slotKeyUp()
{
    if (currentIndex().row() > 0)
        selectRow(currentIndex().row() - 1);
}

void ExplorerWidget::slotKeyDown()
{
    if (currentIndex().row() < (model->rowCount(rootIndex()) - 1)) {
        qDebug() << model->rowCount(rootIndex());
        selectRow(currentIndex().row() + 1);
    }
}
