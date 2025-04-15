#ifndef GUI_H
#define GUI_H

#include <QMainWindow>
#include <QThread>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QTextEdit>
#include <QMessageBox>
#include "worker.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class GUI;
}
QT_END_NAMESPACE

class GUI : public QMainWindow
{
    Q_OBJECT

public:
    GUI(QWidget *parent = nullptr);
    ~GUI();

signals:
    void sendRefreshDram();
    void sendRefreshCache();
    void sendRefreshRegisters();
    void sendRunSteps(int steps);
    void sendRunStep();

private slots:
    void onWorkerClockCycles(int value, int pc);

    void onWorkerFetchInfo(const std::vector<int> info);

    void onWorkerDecodeInfo(const std::vector<int> info);

    void onWorkerExecuteInfo(const std::vector<int> info);

    void onWorkerMemoryInfo(const std::vector<int> info);

    void onWorkerWriteBackInfo(const std::vector<int> info);

    void onWorkerShowDram(const std::vector<std::array<signed int, LINE_SIZE>> data);

    void onWorkerShowCache(const std::vector<std::array<signed int, LINE_SIZE>> data);
    
    void onWorkerShowRegisters(const std::array<int, GPR_NUM> &data);
    
    void onWorkerFinished();
    
    void on_upload_intructions_btn_clicked();
    
    void on_upload_program_state_btn_clicked();
    
    void on_refresh_dram_btn_clicked();
    
    void on_refresh_cache_btn_clicked();
    
    void on_refresh_registers_btn_clicked();
    
    void on_enable_pipeline_checkbox_checkStateChanged(const Qt::CheckState &arg1);
    
    void on_enabl_cache_checkbox_checkStateChanged(const Qt::CheckState &arg1);
    
    void on_run_steps_btn_clicked();
    
    void on_step_btn_clicked();
    
    void on_save_program_state_btn_clicked();

private:
    Ui::GUI *ui;
    QThread workerThread;
    Worker *worker;
    const std::map<Mnemonic, QString> mnemonicNameMap = {
        {Mnemonic::ADD, "ADD"},
        {Mnemonic::SUB, "SUB"},
        {Mnemonic::MUL, "MUL"},
        {Mnemonic::QUOT, "QUOT"},
        {Mnemonic::SFTR, "SFTR"},
        {Mnemonic::SFTL, "SFTL"},
        {Mnemonic::AND, "AND"},
        {Mnemonic::OR, "OR"},
        {Mnemonic::NOT, "NOT"},
        {Mnemonic::XOR, "XOR"},
        {Mnemonic::ADDV, "ADDV"},
        {Mnemonic::SUBV, "SUBV"},
        {Mnemonic::MULV, "MULV"},
        {Mnemonic::DIVV, "DIVV"},
        {Mnemonic::CMP, "CMP"},
        {Mnemonic::CEV, "CEV"},
        {Mnemonic::LOAD, "LOAD"},
        {Mnemonic::LOADV, "LOADV"},
        {Mnemonic::ADDI, "ADDI"},
        {Mnemonic::SUBI, "SUBI"},
        {Mnemonic::SFTRI, "SFTRI"},
        {Mnemonic::SFTLI, "SFTLI"},
        {Mnemonic::ANDI, "ANDI"},
        {Mnemonic::ORI, "ORI"},
        {Mnemonic::XORI, "XORI"},
        {Mnemonic::STORE, "STORE"},
        {Mnemonic::STOREV, "STOREV"},
        {Mnemonic::JMP, "JMP"},
        {Mnemonic::JRL, "JRL"},
        {Mnemonic::JAL, "JAL"},
        {Mnemonic::BEQ, "BEQ"},
        {Mnemonic::BGT, "BGT"},
        {Mnemonic::BUF, "BUF"},
        {Mnemonic::BOF, "BOF"},
        {Mnemonic::PUSH, "PUSH"},
        {Mnemonic::POP, "POP"},
        {Mnemonic::NOP, "NOP"},
    };
    QString mnemonicToString(Mnemonic mnemonic) {
        auto it = mnemonicNameMap.find(mnemonic);
        return (it != mnemonicNameMap.end()) ? it->second : "Unknown";
    }
};
#endif // GUI_H
