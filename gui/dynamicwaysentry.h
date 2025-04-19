#ifndef DYNAMICWAYSENTRY_H
#define DYNAMICWAYSENTRY_H


#include <QLineEdit>
#include <QStringList>
#include <QVBoxLayout>
#include <QVector>
#include <QWidget>

class DynamicWaysEntry : public QWidget
{
  public:
	DynamicWaysEntry(QWidget *parent = nullptr);
	QStringList get_entries() const;
	/**
	 * Parses a string from this entry field, if it is valid.
	 * @param a string
	 * @param -1 if the string is not suitable as a way, an integer compatible
	 * with the cache constructor otherwise.
	 */
	int parse_valid_way(QString t);
  private slots:
	void on_number_enter(const QString &t);

  private:
	QVBoxLayout *l;
	QVector<QLineEdit *> fields;
	QStringList entries;
	void add_field();
	void remove_last_field();
};

#endif // DYNAMICWAYSENTRY_H
