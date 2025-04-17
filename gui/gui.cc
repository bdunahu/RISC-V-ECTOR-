#include "gui.h"
#include "./ui_gui.h"

GUI::GUI(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GUI)
{
    ui->setupUi(this);

    ui->enabl_cache_checkbox->setChecked(true);
    ui->enable_pipeline_checkbox->setChecked(true);

    worker = new Worker();
    worker->moveToThread(&workerThread);

    // Connect worker thread lifecycle
    connect(&workerThread, &QThread::started, worker, &Worker::doWork);

    // Display clock cycles and PC
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
    connect(worker, &Worker::register_storage, this, &GUI::onWorkerShowRegisters);
    
    // Refresh DRAM from worker thread
    connect(this, &GUI::sendRefreshDram, worker, &Worker::refreshDram, Qt::QueuedConnection);

    // Load program from worker thread
    connect(this, &GUI::sendLoadProgram, worker, &Worker::loadProgram, Qt::QueuedConnection);

    // Refresh Cache from worker thread
    connect(this, &GUI::sendRefreshCache, worker, &Worker::refreshCache, Qt::QueuedConnection);

    // Refresh Registers from worker thread
    connect(this, &GUI::sendRefreshRegisters, worker, &Worker::refreshRegisters, Qt::QueuedConnection);

    // Advance controller by #steps
    connect(this, &GUI::sendRunSteps, worker, &Worker::runSteps, Qt::QueuedConnection);

    // Advance controller by 1 step
    connect(this, &GUI::sendRunStep, worker, &Worker::runStep, Qt::QueuedConnection);

    // Proper cleanup when worker finishes
    connect(worker, &Worker::finished, this, &GUI::onWorkerFinished);
    connect(worker, &Worker::finished, &workerThread, &QThread::quit);
    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);

    workerThread.start(); // Start the worker thread
}

GUI::~GUI()
{
    workerThread.quit();
    workerThread.wait();  // Ensure proper cleanup
    delete ui;
}

void displayArrayHTML(QTextEdit *textEdit, const std::array<int, GPR_NUM> &data) {
    textEdit->setReadOnly(false);
    QString tableText = "<table border='1' cellspacing='0' cellpadding='8' style='border-collapse: collapse; width: 100%; border: 2px solid black;'>";

    tableText += "<tr>";
    int index = 0;
    for (int value : data) {
        tableText += QString("<td align='center' style='border: 2px solid black; min-width: 60px; padding: 10px;'>"
                             "%1 <sup style='font-size: 10px; font-weight: bold; color: black;'>%2</sup>"
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

void displayTableHTML(QTextEdit *textEdit, const std::vector<std::array<signed int, LINE_SIZE>> &data) {
    textEdit->setReadOnly(false);
    QString tableText = "<table border='1' cellspacing='0' cellpadding='8' style='border-collapse: collapse; width: 100%; border: 2px solid black;'>";

    int index = 0;
    for (const auto &row : data) {
        tableText += "<tr>";
        for (signed int value : row) {
            tableText += QString("<td align='center' style='border: 2px solid black; min-width: 60px; padding: 10px;'>"
                                 "%1 <sup style='font-size: 10px; font-weight: bold; color: black;'>%2</sup>"
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

std::vector<signed int> browseAndRetrieveFile(QWidget* parent) {
    QString filePath = QFileDialog::getOpenFileName(parent, "Open Binary File", QDir::homePath(), "Binary Files (*.bin *.rv);;All Files (*.*)");
    std::vector<signed int> program;
    
    if (filePath.isEmpty()) return program;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(parent, "File Upload", "Unable to open file!");
        return program;
    }

    while (!file.atEnd()) {
        int32_t word = 0;
        if (file.read(reinterpret_cast<char*>(&word), sizeof(int32_t)) == sizeof(int32_t)) {
            program.push_back(static_cast<signed int>(word));
        }
    }

    file.close();

    return program;
}

void GUI::onWorkerClockCycles(int cycles, int pc) {
    QFont font = ui->cycles_label->font(); 
    font.setBold(true);
    font.setItalic(true);
    font.setPointSize(14);           
    ui->cycles_label->setFont(font);
    ui->cycles_label->setText("Clock Cycles: " + QString::number(cycles) + "\t\t" + "PC: " + QString::number(pc));
}

void GUI::onWorkerFetchInfo(const std::vector<int> info) {
    //QString::asprintf("%04X", value)
    if(!info.empty()) {
        ui->fetch_pc->setText(QString::number(info[0]));
        ui->fetch_instruction_bits->setText(QString::asprintf("%04X", info[1]));
    } else {
        ui->fetch_pc->clear();
        ui->fetch_instruction_bits->clear();
    }
}

void GUI::onWorkerDecodeInfo(const std::vector<int> info) {
    if(!info.empty()) {
        // ui->decode_mnemonic->setText(mnemonicToString((Mnemonic)info[0]));
        ui->decode_pc->setText(QString::number(info[0]));
        ui->decode_s1->setText(QString::asprintf("%04X", info[1]));
        // ui->decode_s2->setText(QString::asprintf("%04X", info[3]));
        // ui->decode_s3->setText(QString::asprintf("%04X", info[4]));
    } else {
        // ui->decode_mnemonic->clear();
        ui->decode_pc->clear();
        ui->decode_s1->clear();
        // ui->decode_s2->clear();
        // ui->decode_s3->clear();
    }
}

void GUI::onWorkerExecuteInfo(const std::vector<int> info) {
    if(!info.empty()) {
        ui->execute_mnemonic->setText(mnemonicToString((Mnemonic)info[0]));
        ui->execute_pc->setText(QString::number(info[1]));
        ui->execute_s1->setText(QString::asprintf("%04X", info[2]));
        ui->execute_s2->setText(QString::asprintf("%04X", info[3]));
        ui->execute_s3->setText(QString::asprintf("%04X", info[4]));
    } else {
        ui->execute_mnemonic->clear();
        ui->execute_pc->clear();
        ui->execute_s1->clear();
        ui->execute_s2->clear();
        ui->execute_s3->clear();
    }
}

void GUI::onWorkerMemoryInfo(const std::vector<int> info) {
    if(!info.empty()) {
        ui->memory_mnemonic->setText(mnemonicToString((Mnemonic)info[0]));
        ui->memory_pc->setText(QString::number(info[1]));
        ui->memory_s1->setText(QString::asprintf("%04X", info[2]));
        ui->memory_s2->setText(QString::asprintf("%04X", info[3]));
        ui->memory_s3->setText(QString::asprintf("%04X", info[4]));
    } else {
        ui->memory_mnemonic->clear();
        ui->memory_pc->clear();
        ui->memory_s1->clear();
        ui->memory_s2->clear();
        ui->memory_s3->clear();
    }
}

void GUI::onWorkerWriteBackInfo(const std::vector<int> info) {
    if(!info.empty()) {
        ui->wb_mnemonic->setText(mnemonicToString((Mnemonic)info[0]));
        ui->wb_pc->setText(QString::number(info[1]));
        ui->wb_s1->setText(QString::asprintf("%04X", info[2]));
        ui->wb_s2->setText(QString::asprintf("%04X", info[3]));
        ui->wb_s3->setText(QString::asprintf("%04X", info[4]));
    } else {
        ui->wb_mnemonic->clear();
        ui->wb_pc->clear();
        ui->wb_s1->clear();
        ui->wb_s2->clear();
        ui->wb_s3->clear();
    }
}

void GUI::onWorkerShowDram(const std::vector<std::array<signed int, LINE_SIZE>> data) {
    displayTableHTML(ui->dram_table, data);
}

void GUI::onWorkerShowCache(const std::vector<std::array<signed int, LINE_SIZE>> data) {
    displayTableHTML(ui->cache_table, data);
}

void GUI::onWorkerShowRegisters(const std::array<int, GPR_NUM> &data) {
    displayArrayHTML(ui->register_table, data);
}

void GUI::onWorkerFinished() {
    qDebug() << "Worker has finished processing.";
}

void GUI::on_upload_intructions_btn_clicked()
{
    qDebug() << "Upload intructions button clicked.";
    std::vector<signed int> program;
    program = browseAndRetrieveFile(ui->register_table);
    if(program.empty()){
        QMessageBox::critical(ui->register_table, "File Upload", "Invalid Program File!");
    }
    emit sendLoadProgram(program);
    emit sendRefreshDram();
    QMessageBox::information(ui->register_table, "File Upload", "Instructions loaded successfully!");
}


void GUI::on_upload_program_state_btn_clicked()
{
    //TODO:Upload and set program state ( have to decide how to use this)
    qDebug() << "upload program state button is clicked.";
}


void GUI::on_refresh_dram_btn_clicked()
{
    qDebug() << "Refresh DRAM button clicked.";
    emit sendRefreshDram();

}


void GUI::on_refresh_cache_btn_clicked()
{
    qDebug() << "Refresh cache button clicked.";
    emit sendRefreshCache();
}


void GUI::on_refresh_registers_btn_clicked()
{
    qDebug() << "Refresh registers button clicked.";
    emit sendRefreshRegisters();
}


void GUI::on_enable_pipeline_checkbox_checkStateChanged(const Qt::CheckState &arg1)
{
    //TODO: handle pipeline enabling
    if(arg1 == Qt::CheckState::Checked) {
        qDebug() << "enable pipeline checkbox checked.";
    } else {
        qDebug() << "enable pipeline checkbox unchecked.";
    }
}


void GUI::on_enabl_cache_checkbox_checkStateChanged(const Qt::CheckState &arg1)
{
    //TODO: handle cache enabling
    if(arg1 == Qt::CheckState::Checked) {   
        qDebug() << "enable cache checkbox checked.";
    } else {
        qDebug() << "enable cache checkbox unchecked.";   
    }

}


void GUI::on_run_steps_btn_clicked()
{
    qDebug() << "Run steps button clicked.";
    emit sendRunSteps(ui->number_steps_inp->text().toInt());
}


void GUI::on_step_btn_clicked()
{
    qDebug() << "Run step button clicked.";
    emit sendRunStep();
}


void GUI::on_save_program_state_btn_clicked()
{
    //TODO: save program state
    qDebug() << "save program state button is clicked.";
}

