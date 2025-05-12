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

#ifndef GUI_H
#define GUI_H

#include "storageview.h"
#include "worker.h"
#include <QFile>
#include <QFileDialog>
#include <QInputDialog>
#include <QLabel>
#include <QMainWindow>
#include <QTextEdit>
#include <QTextStream>
#include <QThread>
#include <functional>

QT_BEGIN_NAMESPACE
namespace Ui
{
class GUI;
}
QT_END_NAMESPACE

class GUI : public QMainWindow
{
	Q_OBJECT

  public:
	/**
	 * Constructor.
	 * @return A newly allocated GUI object.
	 */
	GUI(QWidget *parent = nullptr);
	~GUI();

  signals:
	void hex_toggled(bool is_hex);
	void sendRunSteps(int steps);
	void sendConfigure(
		std::vector<unsigned int> ways, vector<int> program, bool is_pipelined);

  private slots:
	void on_worker_refresh_gui(int value, int pc);

	void onWorkerFetchInfo(const InstrDTO *);

	void onWorkerDecodeInfo(const InstrDTO *);

	void onWorkerExecuteInfo(const InstrDTO *);

	void onWorkerMemoryInfo(const InstrDTO *);

	void onWorkerWriteBackInfo(const InstrDTO *);

	void onWorkerStepsDone();

	void onWorkerShowStorage(const QVector<QVector<int>> &data, int i);

	void onWorkerShowRegisters(
		const QVector<signed int> &gprs,
		const QVector<QVector<signed int>> &vrs);

	void on_upload_intructions_btn_clicked();

	void on_upload_program_state_btn_clicked();

	void
	on_enable_pipeline_checkbox_checkStateChanged(const Qt::CheckState &arg1);

	void on_base_toggle_checkbox_checkStateChanged(const Qt::CheckState &state);

	void on_step_btn_clicked();

	void on_save_program_state_btn_clicked();

	/**
	 * Validates that the user has provided a valid program and cache.
	 * If so, `this->ready' is set and the user options are passed to the
	 * worker.
	 * If not, rudely complains.
	 */
	void on_config_clicked();

  private:
	Ui::GUI *ui;

	/**
	 * Indicates if the program has been initialized.
	 */
	bool ready = false;

	/**
	 * The current number of cache levels.
	 */
	int curr_cache_levels = 0;

	/**
	 * The list of storage displays.
	 */
	std::vector<StorageView *> tab_boxes;

	/**
	 * Whether or not numerical values are currently displaying in hex.
	 */
	bool is_hex = true;

	/**
	 * The message displayed on the status bar.
	 */
	QLabel *status_label;

	/**
	 * The robot image displayed on the status bar.
	 */
	QLabel *avatar;

	/**
	 * The next simulation's program.
	 */
	std::vector<signed int> p;
	/**
	 * If the next initialized simulation is pipelined or not.
	 */
	bool is_pipelined = true;

	/**
	 * The possible step slider values.
	 */
	QVector<long> step_values = {1,			10,			100,		1000,
								 10000,		100000,		1000000,	10000000,
								 100000000, 1000000000, 10000000000};

	QThread workerThread;

	Worker *worker;

	const std::map<Mnemonic, QString> mnemonicNameMap = {
		{Mnemonic::ADD, "ADD"},		{Mnemonic::SUB, "SUB"},
		{Mnemonic::MUL, "MUL"},		{Mnemonic::QUOT, "QUOT"},
		{Mnemonic::REM, "REM"},		{Mnemonic::SFTR, "SFTR"},
		{Mnemonic::SFTL, "SFTL"},	{Mnemonic::AND, "AND"},
		{Mnemonic::OR, "OR"},		{Mnemonic::NOT, "NOT"},
		{Mnemonic::XOR, "XOR"},		{Mnemonic::ADDV, "ADDV"},
		{Mnemonic::SUBV, "SUBV"},	{Mnemonic::MULV, "MULV"},
		{Mnemonic::DIVV, "DIVV"},	{Mnemonic::CMP, "CMP"},
		{Mnemonic::CEV, "CEV"},		{Mnemonic::LOAD, "LOAD"},
		{Mnemonic::ADDI, "ADDI"},	{Mnemonic::SUBI, "SUBI"},
		{Mnemonic::SFTRI, "SFTRI"}, {Mnemonic::SFTLI, "SFTLI"},
		{Mnemonic::ANDI, "ANDI"},	{Mnemonic::ORI, "ORI"},
		{Mnemonic::XORI, "XORI"},	{Mnemonic::STORE, "STORE"},
		{Mnemonic::SRDL, "SRDL"},	{Mnemonic::SRDS, "SRDS"},
		{Mnemonic::ROTV, "ROTV"},	{Mnemonic::JMP, "JMP"},
		{Mnemonic::JRL, "JRL"},		{Mnemonic::JAL, "JAL"},
		{Mnemonic::BEQ, "BEQ"},		{Mnemonic::BGT, "BGT"},
		{Mnemonic::BUF, "BUF"},		{Mnemonic::BOF, "BOF"},
		{Mnemonic::PUSH, "PUSH"},	{Mnemonic::POP, "POP"},
		{Mnemonic::NOP, "NOP"},		{Mnemonic::RET, "RET"},
	};
	QString mnemonicToString(Mnemonic mnemonic)
	{
		auto it = mnemonicNameMap.find(mnemonic);
		return (it != mnemonicNameMap.end()) ? it->second : "Unknown";
	}

	/**
	 * Helper for 'on_config_clicked'.
	 * Initialized the tab component with enough views for the simulation's
	 * storage devices.
	 * @param the number of tabs required to show registers, DRAM, and cache.
	 */
	void make_tabs(int num);
	/**
	 * Uses `func' to set the current status.
	 * @param a function which returns a string.
	 * @param a path to the desired avatar
	 */
	void set_status(
		const std::function<std::string()> &func,
		const QString &img = "idle.png");
};
#endif // GUI_H
