#ifndef GUI_H
#define GUI_H

#include <QMainWindow>
#include <QThread>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QTextEdit>
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
};
#endif // GUI_H
