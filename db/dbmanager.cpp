#include "dbmanager.h"

DbManager::DbManager() {}

DbManager::~DbManager() {}

void DbManager::initialize_db(const QString path, const QString name) {
  QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", name);
  db.setDatabaseName(path);
  if (!db.open()) {
    qDebug() << "Error: connection with database fail";
  }
}
