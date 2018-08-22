#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  io_manager = IO();
  db_query = DBQuery();
  old_search_text = "";
  setup_plots();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::initialize_blast_query(QString folder) {
  work_folder = folder;
  if (!work_folder.isEmpty() && !work_folder.isNull()) {
    ui->path_label->setText("Work folder path : " + work_folder);
    // Full path of db files
    db_files_list = io_manager.get_blast_query_db_list(work_folder);
    if (!db_files_list.isEmpty()) {
      foreach (QString file, db_files_list) {
        DbManager::initialize_db(file, file);
      }
      QStringList file_names_list;
      file_names_list.append("---");
      foreach (QString file, db_files_list) {
        QFileInfo fi(file);
        file_names_list.append(fi.fileName());
      }
      ui->selection1->addItems(file_names_list);
      ui->selection2->addItems(file_names_list);
      ui->selection3->addItems(file_names_list);
      populate_completer(db_query.get_gene_accession_list(db_files_list.at(0)));
    } else {
      QMessageBox::warning(this, tr("Blast Query ++"),
                           tr("No blast query databases found in work folder "
                              "path. Please select a valid work folder."));
    }
  }
  resize_tables();
}

void MainWindow::on_actionChoose_Work_Folder_triggered() {
  // Uncomment the below 3 lines for final version
  QString folder = QFileDialog::getExistingDirectory(
      this, tr("Open Directory"), QDir::home().dirName(),
      QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
  initialize_blast_query(folder);
}

void MainWindow::resize_tables() {
  ui->list_table1->horizontalHeader()->setSectionResizeMode(
      QHeaderView::ResizeToContents);
  ui->list_table2->horizontalHeader()->setSectionResizeMode(
      QHeaderView::ResizeToContents);
  ui->list_table3->horizontalHeader()->setSectionResizeMode(
      QHeaderView::ResizeToContents);
}

void MainWindow::populate_completer(QStringList list) {
  QCompleter *completer = new QCompleter(list, this);
  completer->setCaseSensitivity(Qt::CaseInsensitive);
  ui->searchtext->setCompleter(completer);
}

void MainWindow::on_searchtext_editingFinished() {
  on_searchtext_returnPressed();
}

void MainWindow::on_searchtext_returnPressed() {
  QString search_text = ui->searchtext->text();
  if (old_search_text != search_text) {
    QStringListModel *model = new QStringListModel();
    QStringList accession_list =
        db_query.get_accession_list_for_gene(db_files_list.at(0), search_text);
    model->setStringList(accession_list);
    ui->accession_list->setModel(model);
    update_table_contents();
    old_search_text = search_text;
  }
}

void MainWindow::on_accession_list_currentIndexChanged(const QString &arg1) {
  QStringList gene_info =
      db_query.get_info_for_accession_number(db_files_list.at(0), arg1);
  if (!gene_info.isEmpty()) {
    ui->gene_textbox->setHtml(io_manager.create_gene_html_text(gene_info));
    update_table_contents();
  }
}

void MainWindow::update_table_contents() {

  on_selection1_currentIndexChanged(ui->selection1->currentIndex());
  on_selection2_currentIndexChanged(ui->selection2->currentIndex());
  on_selection3_currentIndexChanged(ui->selection3->currentIndex());
}

void MainWindow::on_selection1_currentIndexChanged(int index) {
  if (index > 0) {
    QVector<QStringList> result = db_query.get_junction_list(
        db_files_list.at(index - 1), ui->searchtext->text(),
        ui->accession_list->currentText());
    ui->list_table1->setRowCount(result.length());
    ui->list_table1->setColumnCount(5);
    reference_junction_list.clear();
    for (int i = 0; i < result.length(); i++) {
      reference_junction_list.append(result.at(i).at(0));
      for (int j = 0; j < result.at(i).length(); j++) {
        if (j == 0 || j == 1 || j == 2) {
          ui->list_table1->setItem(
              i, j, new NumberTableWidgetItem(result.at(i).at(j)));
        } else {
          ui->list_table1->setItem(
              i, j, new StringTableWidgetItem(result.at(i).at(j)));
        }
      }
    }
    QVector<double> ticks1;
    QVector<double> ticks2;
    QVector<double> stats =
        db_query.get_stats(db_files_list.at(index - 1), ui->searchtext->text(),
                           ui->accession_list->currentText());
    ticks1 << 1 << 2 << 3 << 4 << 5;
    plot1_bar->setData(ticks1, stats.mid(0, 5));
    ticks2 << 6 << 7 << 8;
    plot1_bardark->setData(ticks2, stats.mid(5, 3));
    ui->plot1->yAxis->rescale(true);
    ui->plot1->replot();
  } else {
    ui->list_table1->setRowCount(0);
    ui->list_table1->clearContents();
  }
  ui->list_table1->horizontalHeader()->setSectionResizeMode(
      QHeaderView::ResizeToContents);
}

void MainWindow::on_selection2_currentIndexChanged(int index) {
  if (index > 0) {
    // Populate the List
    QVector<QStringList> result = db_query.get_junction_list(
        db_files_list.at(index - 1), ui->searchtext->text(),
        ui->accession_list->currentText());
    ui->list_table2->setRowCount(result.length());
    ui->list_table2->setColumnCount(5);
    for (int i = 0; i < result.length(); i++) {
      for (int j = 0; j < result.at(i).length(); j++) {
        if (j == 0 || j == 1 || j == 2) {
          ui->list_table2->setItem(
              i, j, new NumberTableWidgetItem(result.at(i).at(j)));
        } else {
          ui->list_table2->setItem(
              i, j, new StringTableWidgetItem(result.at(i).at(j)));
        }
      }
    }
    // Populate the Filtered List
    if (!reference_junction_list.isEmpty()) {
      QVector<QStringList> squashed_result = db_query.get_squashed_list(
          db_files_list.at(index - 1), ui->searchtext->text(),
          ui->accession_list->currentText());
      QVector<QStringList> filtered_result;
      foreach (QStringList junc, squashed_result) {
        if (reference_junction_list.contains(junc[0])) {
          filtered_result.append(junc);
        }
      }
      ui->filtered_table2->setRowCount(filtered_result.length());
      ui->filtered_table2->setColumnCount(4);
      for (int i = 0; i < filtered_result.length(); i++) {
        for (int j = 0; j < filtered_result.at(i).length(); j++) {
          if (j == 0 || j == 1) {
            ui->filtered_table2->setItem(
                i, j, new NumberTableWidgetItem(filtered_result.at(i).at(j)));
          } else {
            ui->filtered_table2->setItem(
                i, j, new StringTableWidgetItem(filtered_result.at(i).at(j)));
          }
        }
      }
    } else {
      ui->filtered_table2->setRowCount(0);
      ui->filtered_table2->clearContents();
    }
    // Draw Graph
    QVector<double> ticks1;
    QVector<double> ticks2;
    QVector<double> stats =
        db_query.get_stats(db_files_list.at(index - 1), ui->searchtext->text(),
                           ui->accession_list->currentText());
    ticks1 << 1 << 2 << 3 << 4 << 5;
    plot2_bar->setData(ticks1, stats.mid(0, 5));
    ticks2 << 6 << 7 << 8;
    plot2_bardark->setData(ticks2, stats.mid(5, 3));
    ui->plot2->yAxis->rescale(true);
    ui->plot2->replot();
  } else {
    ui->list_table2->setRowCount(0);
    ui->list_table2->clearContents();
    ui->filtered_table2->setRowCount(0);
    ui->filtered_table2->clearContents();
  }
  ui->list_table2->horizontalHeader()->setSectionResizeMode(
      QHeaderView::ResizeToContents);
  ui->filtered_table2->horizontalHeader()->setSectionResizeMode(
      QHeaderView::ResizeToContents);
}

void MainWindow::on_selection3_currentIndexChanged(int index) {
  if (index > 0) {
    // Populate the List
    QVector<QStringList> result = db_query.get_junction_list(
        db_files_list.at(index - 1), ui->searchtext->text(),
        ui->accession_list->currentText());
    ui->list_table3->setRowCount(result.length());
    ui->list_table3->setColumnCount(5);
    for (int i = 0; i < result.length(); i++) {
      for (int j = 0; j < result.at(i).length(); j++) {
        if (j == 0 || j == 1 || j == 2) {
          ui->list_table3->setItem(
              i, j, new NumberTableWidgetItem(result.at(i).at(j)));
        } else {
          ui->list_table3->setItem(
              i, j, new StringTableWidgetItem(result.at(i).at(j)));
        }
      }
    }
    if (!reference_junction_list.isEmpty()) {
      QVector<QStringList> squashed_result = db_query.get_squashed_list(
          db_files_list.at(index - 1), ui->searchtext->text(),
          ui->accession_list->currentText());
      QVector<QStringList> filtered_result;
      foreach (QStringList junc, squashed_result) {
        if (reference_junction_list.contains(junc[0])) {
          filtered_result.append(junc);
        }
      }
      ui->filtered_table3->setRowCount(filtered_result.length());
      ui->filtered_table3->setColumnCount(4);
      for (int i = 0; i < filtered_result.length(); i++) {
        for (int j = 0; j < filtered_result.at(i).length(); j++) {
          if (j == 0 || j == 1) {
            ui->filtered_table3->setItem(
                i, j, new NumberTableWidgetItem(filtered_result.at(i).at(j)));
          } else {
            ui->filtered_table3->setItem(
                i, j, new StringTableWidgetItem(filtered_result.at(i).at(j)));
          }
        }
      }
    } else {
      ui->filtered_table3->setRowCount(0);
      ui->filtered_table3->clearContents();
    }
    // Draw Graph
    QVector<double> ticks1;
    QVector<double> ticks2;
    QVector<double> stats =
        db_query.get_stats(db_files_list.at(index - 1), ui->searchtext->text(),
                           ui->accession_list->currentText());
    ticks1 << 1 << 2 << 3 << 4 << 5;
    plot3_bar->setData(ticks1, stats.mid(0, 5));
    ticks2 << 6 << 7 << 8;
    plot3_bardark->setData(ticks2, stats.mid(5, 3));
    ui->plot3->yAxis->rescale(true);
    ui->plot3->replot();
  } else {
    ui->list_table3->setRowCount(0);
    ui->list_table3->clearContents();
    ui->filtered_table3->setRowCount(0);
    ui->filtered_table3->clearContents();
  }
  ui->list_table3->horizontalHeader()->setSectionResizeMode(
      QHeaderView::ResizeToContents);
  ui->filtered_table3->horizontalHeader()->setSectionResizeMode(
      QHeaderView::ResizeToContents);
}

void MainWindow::setup_plots() {
  QVector<QCustomPlot *> plots;
  plot1_bar = new QCPBars(ui->plot1->xAxis, ui->plot1->yAxis);
  plot2_bar = new QCPBars(ui->plot2->xAxis, ui->plot2->yAxis);
  plot3_bar = new QCPBars(ui->plot3->xAxis, ui->plot3->yAxis);
  plot1_bardark = new QCPBars(ui->plot1->xAxis, ui->plot1->yAxis);
  plot2_bardark = new QCPBars(ui->plot2->xAxis, ui->plot2->yAxis);
  plot3_bardark = new QCPBars(ui->plot3->xAxis, ui->plot3->yAxis);
  plot1_bar->setWidth(0.5);
  plot2_bar->setWidth(0.5);
  plot3_bar->setWidth(0.5);
  plot1_bardark->setWidth(0.5);
  plot2_bardark->setWidth(0.5);
  plot3_bardark->setWidth(0.5);
  plot1_bar->setBrush(QColor(2, 195, 154));
  plot1_bar->setPen(QPen(QColor(2, 195, 154).darker(100)));
  plot2_bar->setBrush(QColor(253, 56, 147));
  plot2_bar->setPen(QPen(QColor(253, 56, 147).darker(100)));
  plot3_bar->setBrush(QColor(19, 150, 252));
  plot3_bar->setPen(QPen(QColor(19, 150, 252).darker(100)));
  plot1_bardark->setBrush(QColor(2, 195, 154).darker(200));
  plot1_bardark->setPen(QPen(QColor(2, 195, 154).darker(300)));
  plot2_bardark->setBrush(QColor(253, 56, 147).darker(200));
  plot2_bardark->setPen(QPen(QColor(253, 56, 147).darker(300)));
  plot3_bardark->setBrush(QColor(19, 150, 252).darker(200));
  plot3_bardark->setPen(QPen(QColor(19, 150, 252).darker(300)));
  plots.append(ui->plot1);
  plots.append(ui->plot2);
  plots.append(ui->plot3);
  for (int i = 0; i < plots.count(); i++) {
    QCustomPlot *plot = plots.at(i);
    plot->xAxis->setVisible(true);
    plot->xAxis->grid()->setVisible(false);
    plot->xAxis2->setVisible(false);
    plot->yAxis->setVisible(true);
    plot->yAxis2->setVisible(false);
    QVector<double> ticks;
    QVector<QString> labels;
    ticks << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8;
    labels << "In Frame"
           << "In Orf"
           << "In Frame + In Orf"
           << "Out of Frame"
           << "Intron"
           << "Backwards"
           << "Upstream"
           << "Downstream";
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(ticks, labels);
    plot->xAxis->setTicker(textTicker);
    plot->xAxis->setTickLabelRotation(45);
    plot->xAxis->setRange(0, 9);
    plot->yAxis->setLabel("counts");
    //    plot->yAxis2->setLabel("Log Counts");
    //    QSharedPointer<QCPAxisTickerLog> logTicker(new QCPAxisTickerLog);
    //    plot->yAxis->setTicker(logTicker);
    //    plot->yAxis2->setTicker(logTicker);
    //    plot->yAxis->setNumberFormat("eb");
    //    plot->yAxis->setNumberPrecision(0);
    //    plot->yAxis2->setNumberFormat("eb");
    //    plot->yAxis2->setNumberPrecision(0);
    //    plot->yAxis->setScaleType(QCPAxis::stLogarithmic);
    //    plot->yAxis2->setScaleType(QCPAxis::stLogarithmic);
  }
}

void MainWindow::on_save_csv_clicked() {}
