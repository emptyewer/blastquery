#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  QApplication::setStyle("fusion");
  MainWindow w;
  w.show();

  if (argc == 2) {
    QString path(argv[1]);
    qDebug() << "setting work folder" << path;
    w.initialize_blast_query(path);
  }
  return a.exec();
}
