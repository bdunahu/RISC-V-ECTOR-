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

#include "cachewaysselector.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QVBoxLayout>

CacheWaysSelector::CacheWaysSelector(QWidget *parent) : QWidget(parent)
{
	QVBoxLayout *v;
	QHBoxLayout *l;
	QSpinBox *sb;
	QLabel *b;
	int i;

	v = new QVBoxLayout(this);

	for (i = 1; i <= 6; ++i) {
		l = new QHBoxLayout;

		b = new QLabel(QString("L%1 2^").arg(i), this);

		sb = new QSpinBox;
		sb->setRange(-1, 4);
		sb->setValue(-1);

		l->addWidget(b);
		l->addWidget(sb);

		v->addLayout(l);
		this->sbs.append(sb);
	}
}

QList<int> CacheWaysSelector::values() const
{
	QList<int> r;
	for (const QSpinBox *sb : this->sbs) {
		r.append(sb->value());
	}
	return r;
}
