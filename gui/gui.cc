#include "gui.h"
#include "./ui_gui.h"
#include "messages.h"

GUI::GUI(QWidget *parent) : QMainWindow(parent), ui(new Ui::GUI)
{
	ui->setupUi(this);

	this->status_label = new QLabel(this->make_status(get_waiting), this);
	QLabel *risc_vector =
		new QLabel("RISC V[ECTOR], CS535 UMASS AMHERST", this);
	status_label->setMinimumWidth(1200);
	ui->statusBar->addWidget(status_label);
	ui->statusBar->addPermanentWidget(risc_vector);

	worker = new Worker();
	worker->moveToThread(&workerThread);

	// Connect worker thread lifecycle
	// connect(&workerThread, &QThread::started, worker, &Worker::doWork);

	// display clock cycles and PC
	connect(worker, &Worker::clock_cycles, this, &GUI::onWorkerClockCycles);

	connect(worker, &Worker::if_info, this, &GUI::onWorkerFetchInfo);

	connect(worker, &Worker::id_info, this, &GUI::onWorkerDecodeInfo);

	connect(worker, &Worker::ex_info, this, &GUI::onWorkerExecuteInfo);

	connect(worker, &Worker::mm_info, this, &GUI::onWorkerMemoryInfo);

	connect(worker, &Worker::wb_info, this, &GUI::onWorkerWriteBackInfo);

	// Display dram
	connect(worker, &Worker::dram_storage, this, &GUI::onWorkerShowDram);

	// Display cache
	connect(worker, &Worker::cache_storage, this, &GUI::onWorkerShowCache);

	// Display registers
	connect(
		worker, &Worker::register_storage, this, &GUI::onWorkerShowRegisters);

	// // Refresh DRAM from worker thread
	// connect(this, &GUI::sendRefreshDram, worker, &Worker::refreshDram,
	// Qt::QueuedConnection);

	// // Refresh Cache from worker thread
	// connect(this, &GUI::sendRefreshCache, worker, &Worker::refreshCache,
	// Qt::QueuedConnection);

	// Refresh Registers from worker thread
	connect(
		this, &GUI::sendRefreshRegisters, worker, &Worker::refreshRegisters,
		Qt::QueuedConnection);

	// Advance controller by somes steps
	connect(
		this, &GUI::sendRunSteps, worker, &Worker::runSteps,
		Qt::QueuedConnection);

	// Update the step button with step amount
	connect(ui->step_slider, &QSlider::valueChanged, this, [=](int index) {
		int value = step_values[index];
		ui->step_btn->setText(QString("Step %1").arg(value));
	});

	// Proper cleanup when worker finishes
	connect(worker, &Worker::finished, this, &GUI::onWorkerFinished);
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

void displayArrayHTML(QTextEdit *textEdit, const std::array<int, GPR_NUM> &data)
{
	textEdit->setReadOnly(false);
	QString tableText = "<table border='1' cellspacing='0' cellpadding='8' "
						"style='border-collapse: collapse; width: 100%; "
						"border: 2px solid black;'>";

	tableText += "<tr>";
	int index = 0;
	for (int value : data) {
		tableText += QString("<td align='center' style='border: 2px solid "
							 "black; min-width: 60px; padding: 10px;'>"
							 "%1 <sup style='font-size: 10px; font-weight: "
							 "bold; color: black;'>%2</sup>"
							 "</td>")
						 .arg(QString::asprintf("%04X", value))
						 .arg(index);
		index++;
	}
	tableText += "</tr>";
	tableText += "</table>";

	textEdit->setHtml(tableText);
	textEdit->setReadOnly(true);
}

void displayTableHTML(
	QTextEdit *textEdit,
	const std::vector<std::array<signed int, LINE_SIZE>> &data)
{
	textEdit->setReadOnly(false);
	QString tableText = "<table border='1' cellspacing='0' cellpadding='8' "
						"style='border-collapse: collapse; width: 100%; "
						"border: 2px solid black;'>";

	int index = 0;
	for (const auto &row : data) {
		tableText += "<tr>";
		for (signed int value : row) {
			tableText += QString("<td align='center' style='border: 2px solid "
								 "black; min-width: 60px; padding: 10px;'>"
								 "%1 <sup style='font-size: 10px; font-weight: "
								 "bold; color: black;'>%2</sup>"
								 "</td>")
							 .arg(QString::asprintf("%04X", value))
							 .arg(index);
			index++;
		}
		tableText += "</tr>";
	}

	tableText += "</table>";

	textEdit->setHtml(tableText);
	textEdit->setReadOnly(true);
}

void GUI::onWorkerClockCycles(int cycles, int pc)
{
	QFont font = ui->cycles_label->font();
	font.setBold(true);
	font.setItalic(true);
	font.setPointSize(14);
	ui->cycles_label->setFont(font);
	ui->cycles_label->setText(
		"Clock Cycles: " + QString::number(cycles) + "\t\t" +
		"PC: " + QString::number(pc));
}

void GUI::onWorkerFetchInfo(const std::vector<int> info)
{
	if (!info.empty()) {
		ui->fetch_squashed->setText(QString::number(info[0]));
		ui->fetch_bits->setText(QString::asprintf("%04X", info[1]));
	} else {
		ui->fetch_squashed->clear();
		ui->fetch_bits->clear();
	}
}

void GUI::onWorkerDecodeInfo(const std::vector<int> info)
{
	if (!info.empty()) {
		ui->decode_squashed->setText(QString::number(info[0]));
		ui->decode_bits->setText(QString::asprintf("%04X", info[1]));
	} else {
		ui->decode_squashed->clear();
		ui->decode_bits->clear();
	}
}

void GUI::onWorkerExecuteInfo(const std::vector<int> info)
{
	if (!info.empty()) {
		ui->execute_mnemonic->setText(mnemonicToString((Mnemonic)info[0]));
		ui->execute_squashed->setText(QString::number(info[1]));
		ui->execute_s1->setText(QString::asprintf("%04X", info[2]));
		ui->execute_s2->setText(QString::asprintf("%04X", info[3]));
		ui->execute_s3->setText(QString::asprintf("%04X", info[4]));
	} else {
		ui->execute_mnemonic->clear();
		ui->execute_squashed->clear();
		ui->execute_s1->clear();
		ui->execute_s2->clear();
		ui->execute_s3->clear();
	}
}

void GUI::onWorkerMemoryInfo(const std::vector<int> info)
{
	if (!info.empty()) {
		ui->memory_mnemonic->setText(mnemonicToString((Mnemonic)info[0]));
		ui->memory_squashed->setText(QString::number(info[1]));
		ui->memory_s1->setText(QString::asprintf("%04X", info[2]));
		ui->memory_s2->setText(QString::asprintf("%04X", info[3]));
		ui->memory_s3->setText(QString::asprintf("%04X", info[4]));
	} else {
		ui->memory_mnemonic->clear();
		ui->memory_squashed->clear();
		ui->memory_s1->clear();
		ui->memory_s2->clear();
		ui->memory_s3->clear();
	}
}

void GUI::onWorkerWriteBackInfo(const std::vector<int> info)
{
	if (!info.empty()) {
		ui->write_mnemonic->setText(mnemonicToString((Mnemonic)info[0]));
		ui->write_s1->setText(QString::asprintf("%04X", info[2]));
		ui->write_s2->setText(QString::asprintf("%04X", info[3]));
		ui->write_s3->setText(QString::asprintf("%04X", info[4]));
	} else {
		ui->write_mnemonic->clear();
		ui->write_s1->clear();
		ui->write_s2->clear();
		ui->write_s3->clear();
	}
}

void GUI::onWorkerShowDram(
	const std::vector<std::array<signed int, LINE_SIZE>> data)
{
	displayTableHTML(ui->dram_table, data);
}

void GUI::onWorkerShowCache(
	const std::vector<std::array<signed int, LINE_SIZE>> data)
{
	displayTableHTML(ui->cache_table, data);
}

void GUI::onWorkerShowRegisters(const std::array<int, GPR_NUM> &data)
{
	displayArrayHTML(ui->register_table, data);
}

void GUI::onWorkerFinished() { qDebug() << "Worker has finished processing."; }

void GUI::on_upload_intructions_btn_clicked()
{
	qDebug() << "Upload intructions button clicked.";

	// why register_table?
	QString filePath = QFileDialog::getOpenFileName(
		ui->register_table, "Open Binary File", QDir::homePath(),
		"Binary Files (*.bin *.rv);;All Files (*.*)");
	QFile file(filePath);
	if (filePath.isEmpty() || !file.open(QIODevice::ReadOnly)) {
		this->status_label->setText(this->make_status(get_bad_file));
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

	this->status_label->setText(this->make_status(get_load_file));

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
	if (arg1 == Qt::CheckState::Checked) {
		qDebug() << "enable pipeline checkbox checked.";
		is_pipelined = true;
	} else {
		qDebug() << "enable pipeline checkbox unchecked.";
		is_pipelined = false;
	}
}

void GUI::on_step_btn_clicked()
{
	qDebug() << "Run step button clicked.";
	int steps = step_values[ui->step_slider->value()];
	emit sendRunSteps(steps);
}

void GUI::on_save_program_state_btn_clicked()
{
	// TODO: save program state
	qDebug() << "save program state button is clicked.";
}

QString GUI::make_status(const std::function<std::string()> &func)
{
	return "CPU SAYS: \"" + QString::fromStdString(func()) + "\"";
}
