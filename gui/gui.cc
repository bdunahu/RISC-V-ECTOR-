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

#include "gui.h"
#include "./ui_gui.h"
#include "cachewaysselector.h"
#include "digitlabeldelegate.h"
#include "messages.h"
#include "registerview.h"
#include "storageview.h"
#include "util.h"
#include <QHeaderView>
#include <QPixmap>
#include <QString>
#include <QTableView>

GUI::GUI(QWidget *parent) : QMainWindow(parent), ui(new Ui::GUI)
{
	ui->setupUi(this);

	/* setup the status bar */
	ui->statusBar->setFixedHeight(20);

	this->avatar = new QLabel(this);
	this->status_label = new QLabel("", this);
	QLabel *risc_vector =
		new QLabel("RISC V[ECTOR], CS535 UMASS AMHERST", this);

	avatar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	avatar->setObjectName("avatar_label");
	status_label->setObjectName("msg_label");
	risc_vector->setObjectName("info_label");
	ui->statusBar->setSizeGripEnabled(false);

	this->set_status(get_waiting, "idle.png");
	ui->statusBar->addWidget(avatar);
	ui->statusBar->addWidget(status_label);
	ui->statusBar->addPermanentWidget(risc_vector);

	worker = new Worker();
	worker->moveToThread(&workerThread);

	// find all the labels
	QList<DigitLabel *> labels = this->findChildren<DigitLabel *>();
	for (DigitLabel *label : labels) {
		connect(this, &GUI::hex_toggled, label, &DigitLabel::on_hex_toggle);
	}

	// display clock cycles and PC
	connect(worker, &Worker::clock_cycles, this, &GUI::on_worker_refresh_gui);

	connect(worker, &Worker::if_info, this, &GUI::onWorkerFetchInfo);

	connect(worker, &Worker::id_info, this, &GUI::onWorkerDecodeInfo);

	connect(worker, &Worker::ex_info, this, &GUI::onWorkerExecuteInfo);

	connect(worker, &Worker::mm_info, this, &GUI::onWorkerMemoryInfo);

	connect(worker, &Worker::wb_info, this, &GUI::onWorkerWriteBackInfo);

	connect(worker, &Worker::steps_done, this, &GUI::onWorkerStepsDone);

	// Display cache
	connect(worker, &Worker::storage, this, &GUI::onWorkerShowStorage);

	// Display registers
	connect(
		worker, &Worker::register_storage, this, &GUI::onWorkerShowRegisters);

	// Configure pipeline
	connect(
		this, &GUI::sendConfigure, worker, &Worker::configure,
		Qt::QueuedConnection);

	// Advance controller by some steps
	connect(
		this, &GUI::sendRunSteps, worker, &Worker::runSteps,
		Qt::QueuedConnection);

	// Update the step button with step amount
	connect(ui->step_slider, &QSlider::valueChanged, this, [=](int index) {
		int value = step_values[index];
		ui->step_btn->setText(QString("Step %1").arg(value));
	});

	// Proper cleanup when worker finishes
	connect(worker, &Worker::finished, &workerThread, &QThread::quit);
	connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);

	workerThread.start(); // Start the worker thread
}

GUI::~GUI()
{
	workerThread.quit();
	workerThread.wait(); // Ensure proper cleanup
	delete ui;
}

void GUI::on_worker_refresh_gui(int cycles, int pc)
{
	ui->p_counter->set_value(pc);
	ui->cycle_counter->set_value(cycles);
}

void GUI::onWorkerFetchInfo(const InstrDTO *i)
{
	if (i) {
		ui->fetch_squashed->setText(QString::number(i->is_squashed));
		ui->fetch_bits->set_value(i->slot_A);
	} else {
		ui->fetch_squashed->clear();
		ui->fetch_bits->clear();
	}
}

void GUI::onWorkerDecodeInfo(const InstrDTO *i)
{
	if (i) {
		ui->decode_squashed->setText(QString::number(i->is_squashed));
		ui->decode_bits->set_value(i->slot_A);
	} else {
		ui->decode_squashed->clear();
		ui->decode_bits->clear();
	}
}

void GUI::onWorkerExecuteInfo(const InstrDTO *i)
{
	if (i) {
		ui->execute_mnemonic->setText(mnemonicToString(i->mnemonic));
		ui->execute_squashed->setText(QString::number(i->is_squashed));
		ui->execute_s1->set_value(i->operands.integer.slot_one);
		ui->execute_s2->set_value(i->operands.integer.slot_two);
		ui->execute_s3->set_value(i->operands.integer.slot_three);
	} else {
		ui->execute_mnemonic->clear();
		ui->execute_squashed->clear();
		ui->execute_s1->clear();
		ui->execute_s2->clear();
		ui->execute_s3->clear();
	}
}

void GUI::onWorkerMemoryInfo(const InstrDTO *i)
{
	if (i) {
		ui->memory_mnemonic->setText(mnemonicToString(i->mnemonic));
		ui->memory_squashed->setText(QString::number(i->is_squashed));
		ui->memory_s1->set_value(i->operands.integer.slot_one);
		ui->memory_s2->set_value(i->operands.integer.slot_two);
		ui->memory_s3->set_value(i->operands.integer.slot_three);
	} else {
		ui->memory_mnemonic->clear();
		ui->memory_squashed->clear();
		ui->memory_s1->clear();
		ui->memory_s2->clear();
		ui->memory_s3->clear();
	}
}

void GUI::onWorkerWriteBackInfo(const InstrDTO *i)
{
	if (i) {
		ui->write_mnemonic->setText(mnemonicToString(i->mnemonic));
		ui->write_squashed->setText(QString::number(i->is_squashed));
		ui->write_s1->set_value(i->operands.integer.slot_one);
		ui->write_s2->set_value(i->operands.integer.slot_two);
		ui->write_s3->set_value(i->operands.integer.slot_three);
	} else {
		ui->write_mnemonic->clear();
		ui->write_squashed->clear();
		ui->write_s1->clear();
		ui->write_s2->clear();
		ui->write_s3->clear();
	}
}

void GUI::onWorkerStepsDone() { this->set_status(get_waiting, "idle"); }

void GUI::onWorkerShowStorage(const QVector<QVector<int>> &data, int i)
{
	this->tab_boxes.at(i)->set_data(data);
}

void GUI::onWorkerShowRegisters(
	const QVector<signed int> &gprs, const QVector<QVector<signed int>> &vrs)
{
	RegisterView *rv;

	rv = dynamic_cast<RegisterView *>(this->tab_boxes.at(0));
	rv->set_data(gprs, vrs);
}

void GUI::on_upload_intructions_btn_clicked()
{
	// why ui->register_table, or now ui->storage
	QString filePath = QFileDialog::getOpenFileName(
		ui->storage, "Open Binary File", QDir::homePath(),
		"Binary Files (*.bin *.rv);;All Files (*.*)");
	QFile file(filePath);
	if (filePath.isEmpty() || !file.open(QIODevice::ReadOnly)) {
		this->set_status(get_no_instructions, "angry");
		return;
	}

	this->p.clear();
	while (!file.atEnd()) {
		char bytes[4];
		if (file.read(bytes, 4) == 4) {
			uint32_t word = (static_cast<uint8_t>(bytes[0]) << 24) |
							(static_cast<uint8_t>(bytes[1]) << 16) |
							(static_cast<uint8_t>(bytes[2]) << 8) |
							(static_cast<uint8_t>(bytes[3]));

			this->p.push_back(static_cast<signed int>(word));
		}
	}

	if (this->p.empty())
		this->set_status(get_no_instructions, "angry");
	else
		this->set_status(get_load_file, "happy");

	file.close();
}

void GUI::on_upload_program_state_btn_clicked()
{
	// TODO:Upload and set program state ( have to decide how to use this)
	qDebug() << "upload program state button is clicked.";
}

void GUI::on_enable_pipeline_checkbox_checkStateChanged(
	const Qt::CheckState &arg1)
{
	this->is_pipelined = (arg1 == Qt::CheckState::Checked) ? true : false;
}

void GUI::on_base_toggle_checkbox_checkStateChanged(const Qt::CheckState &state)
{
	this->is_hex = (state == Qt::CheckState::Checked) ? false : true;
	emit this->hex_toggled(this->is_hex);
}

void GUI::on_step_btn_clicked()
{
	// try to configure first
	if (!this->ready)
		this->on_config_clicked();
	// try again
	if (!this->ready)
		return;

	this->set_status(get_running, "busy");
	int steps = step_values[ui->step_slider->value()];
	emit sendRunSteps(steps);
}

void GUI::on_save_program_state_btn_clicked()
{
	// TODO: save program state
	qDebug() << "save program state button is clicked.";
}

void GUI::on_config_clicked()
{
	std::vector<unsigned int> ways;
	QStringList entries;
	CacheWaysSelector *cws = ui->cache_ways_selector;

	for (int i : cws->values()) {

		// invalid
		if (i == -1)
			continue;

		ways.push_back((unsigned int)i);
	}

	if (this->p.empty()) {
		this->set_status(get_no_instructions, "angry");
		return;
	}

	this->ready = true;

	// say something snarky
	if (!is_pipelined)
		this->set_status(get_no_pipeline, "angry");
	else if (ways.size() == 0)
		this->set_status(get_no_cache, "angry");
	else
		this->set_status(get_initialize, "happy");

	this->curr_cache_levels = ways.size();

	emit sendConfigure(ways, this->p, is_pipelined);
	make_tabs(2 + ways.size());
}

void GUI::make_tabs(int num)
{
	int i;
	StorageView *e;
	QTableView *t;
	QString n;
	DigitLabelDelegate *d;

	ui->storage->clear();

	qDeleteAll(this->tab_boxes);
	this->tab_boxes.clear();

	for (i = 0; i < num; ++i) {
		if (i == 0) {
			n = "Registers";
			e = new RegisterView(GPR_NUM + V_NUM, V_R_LIMIT, this);
		} else if (i == num - 1) {
			n = "DRAM";
			e = new StorageView(MEM_LINES, LINE_SIZE, this);
		} else {
			n = QString("L%1").arg(i);
			e = new StorageView(
				(1 << cache_size_mapper(this->curr_cache_levels - 1, i - 1)),
				LINE_SIZE, this);
		}

		t = new QTableView(ui->storage);
		t->setModel(e);
		d = new DigitLabelDelegate(t);

		connect(this, &GUI::hex_toggled, e, &StorageView::set_hex_display);
		connect(
			this, &GUI::hex_toggled, d, &DigitLabelDelegate::set_hex_display);

		t->setItemDelegate(d);
		t->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

		ui->storage->addTab(t, n);
		this->tab_boxes.push_back(e);
	}
}

void GUI::set_status(
	const std::function<std::string()> &func, const QString &img)
{
	this->status_label->setText(
		"-> \"" + QString::fromStdString(func()) + "\"");

	QString img_path = ":resources/" + img;
	QPixmap pixmap(img_path);

	if (!pixmap) {
		return;
	}

	this->avatar->setPixmap(pixmap);
	this->avatar->show();
}
