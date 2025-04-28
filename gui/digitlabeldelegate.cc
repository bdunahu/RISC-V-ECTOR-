// Simulator for the RISC-V[ECTOR] mini-ISA
// Copyright (C) 2025 Siddarth Suresh
// Copyright (C) 2025 bdunahu

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "digitlabeldelegate.h"
#include "util.h"
#include <QString>

void DigitLabelDelegate::set_hex_display(bool hex)
{
	if (this->is_hex != hex) {
		this->is_hex = hex;
		if (auto v = qobject_cast<QAbstractItemView *>(parent()))
			v->viewport()->update();
	}
}

void DigitLabelDelegate::paint(
	QPainter *painter,
	const QStyleOptionViewItem &option,
	const QModelIndex &index) const
{
	int v;
	QString t;
	QStyleOptionViewItem o;
	QStyle *s;
	QVariant a;
	bool e;

	a = index.data(Qt::DisplayRole);
	v = a.toInt();
	e = a.isNull();
	t = format_toggled_value(v, this->is_hex, e);

	o = option;
	initStyleOption(&o, index);
	o.text = t;

	const QWidget *w = option.widget;
	s = w ? w->style() : QApplication::style();
	s->drawControl(QStyle::CE_ItemViewItem, &o, painter, w);
}
