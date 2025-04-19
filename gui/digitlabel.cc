#include "digitlabel.h"

DigitLabel::DigitLabel(QWidget *parent) : QLabel(parent)
{
	this->update_display();
}

void DigitLabel::clear()
{
	this->is_cleared = true;
	setText(QString());
}

void DigitLabel::set_value(int v)
{
	this->is_cleared = false;
	if (this->v != v) {
		this->v = v;
		update_display();
	}
}

void DigitLabel::toggle_mode()
{
	this->is_hex = !this->is_hex;
	this->update_display();
}

void DigitLabel::update_display()
{
	QString t;
	if (this->is_cleared) {
		setText(QString());
	} else {
		t = (this->is_hex) ? QString::number(this->v, 6).toUpper()
						   : QString::number(this->v);
		setText(t);
	}
}
