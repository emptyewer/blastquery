#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  db_manager = DbManager();
  io_manager = IO();
  old_search_text = "";
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_actionChoose_Work_Folder_triggered() {
  work_folder = QFileDialog::getExistingDirectory(
      this, tr("Open Directory"), QDir::home().dirName(),
      QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
  if (!work_folder.isEmpty() && !work_folder.isNull()) {
    ui->path_label->setText("Work folder path : " + work_folder);
    QStringList db_file_list = io_manager.get_blast_query_db_list(work_folder);
    if (!db_file_list.isEmpty()) {
      db_manager.initialize_db(db_file_list.at(0));
      populate_completer(db_manager.get_gene_accession_list());
    } else {
      QMessageBox::warning(this, tr("Blast Query ++"),
                           tr("No blast query databases found in work folder "
                              "path. Please select a valid work folder."));
    }
  }
}

void MainWindow::populate_completer(QStringList list) {
  QCompleter *completer = new QCompleter(list, this);
  completer->setCaseSensitivity(Qt::CaseInsensitive);
  ui->searchtext->setCompleter(completer);
}

void MainWindow::on_pushButton_clicked() {
  on_actionChoose_Work_Folder_triggered();
}

void MainWindow::on_searchtext_editingFinished() {
  on_searchtext_returnPressed();
}

void MainWindow::on_searchtext_returnPressed() {
  QString search_text = ui->searchtext->text();
  if (old_search_text != search_text) {
    QStringListModel *model = new QStringListModel();
    QStringList accession_list =
        db_manager.get_accession_list_for_gene(search_text);
    model->setStringList(accession_list);
    ui->accession_list->setModel(model);
    ui->accession_list->repaint();
    old_search_text = search_text;
  }
}

void MainWindow::on_accession_list_currentIndexChanged(const QString &arg1) {
  QStringList gene_info = db_manager.get_info_for_accession_number(arg1);
  ui->gene_textbox->setHtml(io_manager.create_gene_html_text(gene_info));
}
