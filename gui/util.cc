#include "util.h"
#include <QString>

int cache_size_mapper(int total_levels, int level)
{
	const int y_min = 4;
	const int y_max = 12;
	double f, r;

	if (total_levels <= 1)
		return 8;

	f = level / total_levels;
	r = y_min + f * (y_max - y_min);

	return r;
}

QString format_toggled_value(int value, bool is_hex, bool is_cleared)
{
	if (is_cleared)
		return QString();
	return is_hex ? QString::asprintf("%X", value) : QString::number(value);
}
