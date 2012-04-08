#include "Explorer.h"

ExplorerWidget::ExplorerWidget(MediaPlayer *player, QWidget *parent) :
  QTableView(parent),
  m_player(player)
{
  setObjectName("ExplorerWidget");
  model = new QDirModel(this);
  model->setSorting(QDir::DirsFirst | QDir::IgnoreCase);
  model->setFilter(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files);
  model->setLazyChildCount(true);

  setModel(model);

  QModelIndex index = model->index(m_player->homeFilePath);
  setRootIndex(index);
  setColumnHidden(1, true);
  setColumnHidden(2, true);
  setColumnHidden(3, true);
  horizontalHeader()->hide();
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

  selectRow(0);
  oldIndex = currentIndex();
  onFullScreen(false);
  QString strF = QString(EXTENSIONS_VIDEO) + QString(EXTENSIONS_AUDIO);
  strFilters = strF.split(" *", QString::SkipEmptyParts);
  setFilters();
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
    for (int i = 0; i < model->rowCount(rootIndex()); i++) {
      if (isRowHidden(i)) showRow(i);
    }
    setRootIndex(newIndex);
    selectRow(0);
  } else {
    if (!m_player->isFullScreen())
      m_player->controlPanel->show();
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
    for (int i = 0; i < model->rowCount(rootIndex()); i++) {
      if (isRowHidden(i)) showRow(i);
    }
    setRootIndex(model->parent(oldIndex));
  } else {
    m_player->controlPanel->hide();
    m_player->pause();
    m_player->sWidget.setCurrentIndex(3);
  }

  int row = oldIndex.row();
  if (row == -1) row = 0;
  selectRow(row-1);
  setFilters();
  selectRow(row);
  oldIndex = model->parent(oldIndex);
}

void ExplorerWidget::slotKeyRight()
{
  if (m_player->sWidget.currentIndex() != 3) return;
  slotSetIndex(currentIndex());
  setFilters();
}

void ExplorerWidget::slotKeyUp()
{
  if (m_player->sWidget.currentIndex() != 3) return;
  bool ok = false;
  int row = currentIndex().row();
  if (row > 0) {
    for (int i = row-1; i >= 0; --i) {
      if (!isRowHidden(i)) {
        selectRow(i);
        ok = true;
        break;
      }
    }
  }
  if (!ok) {
    row = model->rowCount(rootIndex())-1;
    for (int i = row; i >= 0; --i) {
      if (!isRowHidden(i)) {
        selectRow(i); break;
      }
    }
  }
  //    if (currentIndex().row() > 0)
  //        selectRow(currentIndex().row() - 1);
  //    else selectRow(model->rowCount(rootIndex())-1);
}

void ExplorerWidget::slotKeyDown()
{
  if (m_player->sWidget.currentIndex() != 3) return;
  bool ok = false;
  int row = currentIndex().row();
  if (row < (model->rowCount(rootIndex()) - 1)) {
    for (int i = row+1; i < model->rowCount(rootIndex()); i++) {
      if (!isRowHidden(i)) {
        selectRow(i);
        ok = true;
        break;
      }
    }
  }
  if (!ok) {
    row = 0;
    for (int i = row; i < model->rowCount(rootIndex()); i++) {
      if (!isRowHidden(i)) {
        selectRow(i); break;
      }
    }
  }
  //    if (currentIndex().row() < (model->rowCount(rootIndex()) - 1)) {
  //        selectRow(currentIndex().row() + 1);
  //    } else selectRow(0);
  //    int  current = verticalScrollBar()->value();
  //    verticalScrollBar()->setValue(++current);
}

void ExplorerWidget::onFullScreen(bool on)
{
  QFont font;
  font = this->font();
  int fontSize;
  int itemSise;
  if (on) {
    fontSize = 48;
    itemSise = fontSize + 24;
    setIconSize(QSize(48, 48));
  } else {
    fontSize = 24;
    itemSise = fontSize + 10;
    setIconSize(QSize(16, 16));
  }
  font.setPixelSize(fontSize);
  setFont(font);
  verticalHeader()->setDefaultSectionSize(itemSise);

  int row = currentIndex().row();
  selectRow(row+1);
  selectRow(row);
}

void ExplorerWidget::setFilters()
{
  bool ok;
  QModelIndex index;
  for (int i = 0; i < model->rowCount(rootIndex()); i++) {
    index = model->index(i, 0 ,rootIndex());
    QString str1 = model->fileName(index);
    if (!model->isDir(index)) {
      QString str = model->fileName(index);
      ok = false;
      foreach (QString strFilter, strFilters) {
        if (str.contains(strFilter, Qt::CaseInsensitive)) {
          ok = true; break;
        }
      }
      if (!ok) {
        hideRow(i);
      }
    } else {
      if (model->fileName(index) == "..") {

      }
    }
  }
}
