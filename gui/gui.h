#ifndef GUI_H
#define GUI_H

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

	/**
	 * Uses `func' to set the current status.
	 * @param a function which returns a string.
	 */
	void set_status(const std::function<std::string()> &func);

  signals:
	void sendRunSteps(int steps);
	void sendConfigure(
		std::vector<unsigned int> ways,
		vector<int> program,
		bool is_pipelined);

  private slots:
	void on_worker_refresh_gui(int value, int pc);

	void onWorkerFetchInfo(const std::vector<int> info);

	void onWorkerDecodeInfo(const std::vector<int> info);

	void onWorkerExecuteInfo(const std::vector<int> info);

	void onWorkerMemoryInfo(const std::vector<int> info);

	void onWorkerWriteBackInfo(const std::vector<int> info);

	void
	onWorkerShowDram(const std::vector<std::array<signed int, LINE_SIZE>> data);

	void onWorkerShowCache(
		const std::vector<std::array<signed int, LINE_SIZE>> data);

	void onWorkerShowRegisters(const std::array<int, GPR_NUM> &data);

	void onWorkerFinished();

	void on_upload_intructions_btn_clicked();

	void on_upload_program_state_btn_clicked();

	void
	on_enable_pipeline_checkbox_checkStateChanged(const Qt::CheckState &arg1);

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
	bool ready;

	/**
	 * The message displayed on the status bar.
	 */
	QLabel *status_label;

	/**
	 * The currently loaded program.
	 */
	std::vector<signed int> p;

	/**
	 * If this stage is pipelined or not.
	 */
	bool is_pipelined = true;

	/**
	 * The possible step slider values.
	 */
	QVector<int> step_values = {1, 5, 20, 50, 250, 1000, 10000};

	QThread workerThread;

	Worker *worker;

	const std::map<Mnemonic, QString> mnemonicNameMap = {
		{Mnemonic::ADD, "ADD"},		  {Mnemonic::SUB, "SUB"},
		{Mnemonic::MUL, "MUL"},		  {Mnemonic::QUOT, "QUOT"},
		{Mnemonic::SFTR, "SFTR"},	  {Mnemonic::SFTL, "SFTL"},
		{Mnemonic::AND, "AND"},		  {Mnemonic::OR, "OR"},
		{Mnemonic::NOT, "NOT"},		  {Mnemonic::XOR, "XOR"},
		{Mnemonic::ADDV, "ADDV"},	  {Mnemonic::SUBV, "SUBV"},
		{Mnemonic::MULV, "MULV"},	  {Mnemonic::DIVV, "DIVV"},
		{Mnemonic::CMP, "CMP"},		  {Mnemonic::CEV, "CEV"},
		{Mnemonic::LOAD, "LOAD"},	  {Mnemonic::LOADV, "LOADV"},
		{Mnemonic::ADDI, "ADDI"},	  {Mnemonic::SUBI, "SUBI"},
		{Mnemonic::SFTRI, "SFTRI"},	  {Mnemonic::SFTLI, "SFTLI"},
		{Mnemonic::ANDI, "ANDI"},	  {Mnemonic::ORI, "ORI"},
		{Mnemonic::XORI, "XORI"},	  {Mnemonic::STORE, "STORE"},
		{Mnemonic::STOREV, "STOREV"}, {Mnemonic::JMP, "JMP"},
		{Mnemonic::JRL, "JRL"},		  {Mnemonic::JAL, "JAL"},
		{Mnemonic::BEQ, "BEQ"},		  {Mnemonic::BGT, "BGT"},
		{Mnemonic::BUF, "BUF"},		  {Mnemonic::BOF, "BOF"},
		{Mnemonic::PUSH, "PUSH"},	  {Mnemonic::POP, "POP"},
		{Mnemonic::NOP, "NOP"},
	};
	QString mnemonicToString(Mnemonic mnemonic)
	{
		auto it = mnemonicNameMap.find(mnemonic);
		return (it != mnemonicNameMap.end()) ? it->second : "Unknown";
	}
};
#endif // GUI_H
