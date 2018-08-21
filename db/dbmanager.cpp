#include "dbmanager.h"

DbManager::DbManager() {}

DbManager::~DbManager() {}

void DbManager::initialize_db(const QString path) {
  db = QSqlDatabase::addDatabase("QSQLITE");
  db.setDatabaseName(path);

  if (!db.open()) {
    qDebug() << "Error: connection with database fail";
  } else {
    qDebug() << "Database: connection ok";
  }
}

QStringList DbManager::get_gene_accession_list() {
  QSqlQuery query("SELECT gene_name,nm_number FROM gene");
  int gname = query.record().indexOf("gene_name");
  int accession = query.record().indexOf("nm_number");
  QStringList list;
  while (query.next()) {
    QString gene_name = query.value(gname).toString();
    QString accession_number = query.value(accession).toString();
    list.append(gene_name);
    list.append(accession_number);
  }
  return list;
}

QStringList DbManager::get_accession_list_for_gene(QString gene) {
  QSqlQuery query("SELECT nm_number FROM gene WHERE gene_name=\'" + gene +
                  "\'");
  QStringList list;
  while (query.next()) {
    QString accession_number = query.value(0).toString();
    list.append(accession_number);
  }
  return list;
}

QStringList DbManager::get_info_for_accession_number(QString accession_number) {
  QSqlQuery query("SELECT * FROM gene WHERE nm_number=\'" + accession_number +
                  "\'");
  QStringList result;
  if (!query.isValid()) {
    query.first();
    QSqlRecord record = query.record();
    for (int i = 0; i < record.count(); i++) {
      result << record.value(i).toString();
    }
  }
  return result;
}
