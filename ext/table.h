#ifndef TABLE_H
#define TABLE_H

#include <QTableWidgetItem>

class NumberTableWidgetItem : public QTableWidgetItem {
public:
  NumberTableWidgetItem(const QString txt = QString("0"))
      : QTableWidgetItem(txt) {}
  bool operator<(const QTableWidgetItem &other) const {
    if (text() == "")
      return text().toDouble() > other.text().toDouble();
    else
      return text().toDouble() < other.text().toDouble();
  }
};

class StringTableWidgetItem : public QTableWidgetItem {
public:
  StringTableWidgetItem(const QString txt) : QTableWidgetItem(txt) {
    setTextAlignment(Qt::AlignCenter);
  }
};

#endif // TABLE_H
