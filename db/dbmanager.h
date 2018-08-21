#ifndef DBMANAGER_H
#define DBMANAGER_H

#include "imports.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>

class DbManager {

public:
  DbManager();
  ~DbManager();
  void initialize_db(const QString path);
  QStringList get_gene_accession_list();
  QStringList get_accession_list_for_gene(QString gene);
  QStringList get_info_for_accession_number(QString accession_number);

private:
  QSqlDatabase db;
};

#endif // DBMANAGER_H
