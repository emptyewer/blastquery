#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "db/dbmanager.h"
#include "db/dbquery.h"
#include "ext/table.h"
#include "imports.h"
#include "plot/qcustomplot.h"
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
  void initialize_blast_query(QString folder);

private slots:
  void on_actionChoose_Work_Folder_triggered();
  void populate_completer(QStringList);
  void on_searchtext_editingFinished();
  void on_searchtext_returnPressed();
  void on_accession_list_currentIndexChanged(const QString &arg1);
  void on_selection1_currentIndexChanged(int index);
  void resize_tables();
  void update_table_contents();
  void setup_plots();
  void on_selection2_currentIndexChanged(int index);
  void on_selection3_currentIndexChanged(int index);
  void on_save_csv_clicked();

private:
  Ui::MainWindow *ui;
  int orf_start;
  int orf_end;
  QStringList db_files_list;
  DBQuery db_query;
  IO io_manager;
  QString work_folder;
  QString old_search_text;
  QCPBars *plot1_bar;
  QCPBars *plot2_bar;
  QCPBars *plot3_bar;
  QCPBars *plot1_bardark;
  QCPBars *plot2_bardark;
  QCPBars *plot3_bardark;
  QCPBars *ppmplot1_bar_blue;
  QCPBars *ppmplot2_bar_blue;
  QCPBars *ppmplot3_bar_blue;
  QCPBars *ppmplot1_bar_bluedark;
  QCPBars *ppmplot2_bar_bluedark;
  QCPBars *ppmplot3_bar_bluedark;
  QCPBars *ppmplot1_bar_red;
  QCPBars *ppmplot2_bar_red;
  QCPBars *ppmplot3_bar_red;
  QCPBars *ppmplot1_bar_gray;
  QCPBars *ppmplot2_bar_gray;
  QCPBars *ppmplot3_bar_gray;
  QStringList reference_junction_list;
};

#endif // MAINWINDOW_H
