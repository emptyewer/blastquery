#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "db/dbmanager.h"
#include "imports.h"
#include "utils/io.h"
#include <QCompleter>
#include <QFileDialog>
#include <QMainWindow>
#include <QStringListModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void on_actionChoose_Work_Folder_triggered();
  void populate_completer(QStringList);

  void on_pushButton_clicked();

  void on_searchtext_editingFinished();

  void on_searchtext_returnPressed();

  void on_accession_list_currentIndexChanged(const QString &arg1);

private:
  Ui::MainWindow *ui;
  DbManager db_manager;
  IO io_manager;
  QString work_folder;
  QString old_search_text;
};

#endif // MAINWINDOW_H
