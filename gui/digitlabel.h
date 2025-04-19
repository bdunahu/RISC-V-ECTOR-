#ifndef DIGITLABEL_H
#define DIGITLABEL_H

#include <QLabel>

class DigitLabel : public QLabel
{
	Q_OBJECT

  public:
	/**
	 * Constructor.
	 * @return a newly allocated DigitLabel.
	 */
	explicit DigitLabel(QWidget *parent = nullptr);

	/**
	 * Sets the empty flag.
	 */
	void clear();
	/**
	 * @param the value to set `this->v' with.
	 */
	void set_value(int v);

  public slots:
	/**
	 * Toggles the base this label displays in, by setting `this->is_hex'.
	 */
	void toggle_mode();

  private:
	/**
	 * Refreshes the display of this label, taking base into consideration..
	 */
	void update_display();

	/**
	 * The decimal value associated with this label.
	 */
	int v = 0;
	/**
	 * To display in hexidecimal or not.
	 */
	bool is_hex = true;
	/**
	 * To display in hexidecimal or not.
	 */
	bool is_cleared = true;
};

#endif // DIGITLABEL_H
