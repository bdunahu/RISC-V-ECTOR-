#include "dynamicwaysentry.h"
#include <QLabel>
#include <QLineEdit>
#include <QStringList>
#include <QVBoxLayout>
#include <QVector>
#include <QWidget>

DynamicWaysEntry::DynamicWaysEntry(QWidget *parent) : QWidget(parent)
{
	this->l = new QVBoxLayout(this);
	this->l->setAlignment(Qt::AlignTop);
	this->l->setSpacing(6);
	this->l->setContentsMargins(0, 0, 0, 0);
	this->setLayout(l);
	this->add_field();
}

QStringList DynamicWaysEntry::get_entries() const { return this->entries; }

int DynamicWaysEntry::parse_valid_way(QString t)
{
	bool s;
	int i;
	i = t.toInt(&s);
	return (s && i >= 0 && 5 > i) ? i : -1;
}

void DynamicWaysEntry::on_number_enter(const QString &t)
{
	int i;
	QLineEdit *sender_field;

	sender_field = qobject_cast<QLineEdit *>(sender());
	i = fields.indexOf(sender_field);
	entries[i] = t;

	if (i == this->fields.size() - 1 && !t.isEmpty() &&
		(this->parse_valid_way(t) > 0) && fields.size() < 4)
		add_field();

	// TODO, unlink, don't trash everything after
	if (t.isEmpty()) {
		while (this->fields.size() > i + 1) {
			remove_last_field();
		}
		while (entries.size() > fields.size()) {
			entries.removeLast();
		}
	}
}

void DynamicWaysEntry::add_field()
{
	QLineEdit *f;

	f = new QLineEdit(this);
	f->setPlaceholderText("# ways (a power of 2)");

	this->l->addWidget(f);;
	this->fields.append(f);
	this->entries.append(QString());
	connect(
		f, &QLineEdit::textChanged, this, &DynamicWaysEntry::on_number_enter);
}

void DynamicWaysEntry::remove_last_field()
{
	QLineEdit *f;

	if (this->fields.isEmpty())
		return;

	f = this->fields.takeLast();
	this->l->removeWidget(f);
	f->deleteLater();

	if (!this->entries.isEmpty())
		entries.removeLast();
}
