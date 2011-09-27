
#include "Explorer.h"

ExplorerWidget::ExplorerWidget(MediaPlayer *player, QWidget *parent) :
    QTableView(parent),
    m_player(player)
{
    setObjectName("ExplorerWidget");
    model = new QDirModel(this);
    model->setSorting(QDir::DirsFirst | QDir::Name);
//    QStringList listFilters;
//    listFilters << "*.flv" << "*./";
//    model->setNameFilters(listFilters);
    model->setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);

    model->setLazyChildCount(true);

    setModel(model);

    QModelIndex index = model->index(m_player->homeFilePath);
    setRootIndex(index);
    setColumnHidden(1, true);
    setColumnHidden(2, true);
    setColumnHidden(3, true);
    this->horizontalHeader()->hide();
    verticalHeader()->hide();
    setSelectionBehavior( QAbstractItemView::SelectRows );
    setSelectionMode(QAbstractItemView::SingleSelection);
//    verticalHeader()->setMinimumSectionSize(50);
    horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
//    horizontalHeader()->setLineWidth(50);
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
    if (model->isDir(newIndex)) {
        setRootIndex(newIndex);
        selectRow(0);
    } else {
        QString filePath = model->filePath(newIndex);
        m_player->initPlayList();
        m_player->addFile(filePath);
        m_player->setCurrentSource(filePath, true);
    }
    oldIndex = newIndex;
}

void ExplorerWidget::slotKeyLeft()
{
    if (m_player->sWidget.currentIndex() == 3) {
        setRootIndex(model->parent(oldIndex));
//        qDebug() << model->parent(oldIndex).row();
//        qDebug() << oldIndex.row();
        int row = oldIndex.row();
        if (row == -1) row = 0;
        selectRow(row);
        oldIndex = model->parent(oldIndex);
    } else {
        m_player->pause();
        m_player->sWidget.setCurrentIndex(3);
        oldIndex = model->parent(oldIndex);
    }
}

void ExplorerWidget::slotKeyRight()
{
    if (m_player->sWidget.currentIndex() != 3) return;
    slotSetIndex(currentIndex());
}

void ExplorerWidget::slotKeyUp()
{
    if (m_player->sWidget.currentIndex() != 3) return;
    if (currentIndex().row() > 0)
        selectRow(currentIndex().row() - 1);
}

void ExplorerWidget::slotKeyDown()
{
    if (m_player->sWidget.currentIndex() != 3) return;
    if (currentIndex().row() < (model->rowCount(rootIndex()) - 1)) {
        selectRow(currentIndex().row() + 1);
    }
}
