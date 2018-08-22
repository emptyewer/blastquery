#ifndef DBMANAGER_H
#define DBMANAGER_H

#include "imports.h"
#include <QSqlDatabase>

class DbManager {

public:
  DbManager();
  ~DbManager();
  static void initialize_db(const QString path, const QString name);

private:
};

#endif // DBMANAGER_H
