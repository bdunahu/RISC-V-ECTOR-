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

    // Display dram
    connect(worker, &Worker::dram_storage, this, &GUI::onWorkerShowDram);

    // Display cache
    connect(worker, &Worker::cache_storage, this, &GUI::onWorkerShowCache);

    // Display registers
    connect(worker, &Worker::register_storage, this, &GUI::onWorkerShowRegisters);
    
    // Refresh DRAM from worker thread
    connect(this, &GUI::sendRefreshDram, worker, &Worker::refreshDram, Qt::QueuedConnection);

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

void browseAndUploadFile(QTextEdit *textEdit) {
    QString filePath = QFileDialog::getOpenFileName(nullptr, "Open File", QDir::homePath(), "Text Files (*.txt);;All Files (*.*)");
    
    if (filePath.isEmpty()) {
        return;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        textEdit->setPlainText("Error: Unable to open file!");
        return;
    }

    QTextStream in(&file);
    QString content;
    int lineNumber = 0;

    while (!in.atEnd()) {
        QString line = in.readLine();
        
        content += QString("<div id='line_%1' style='display: flex; justify-content: space-between; align-items: center;'>"
                           "<span style='font-size: 10px; font-weight: bold; color: gray;'>%1.</span>"
                           "<span>%2</span>"
                           "</div><hr>")
                   .arg(lineNumber)  
                   .arg(line);       
        
        lineNumber++;
    }

    file.close();

    textEdit->setReadOnly(false);  
    textEdit->setHtml(content);
    textEdit->setReadOnly(true);
}

void GUI::onWorkerClockCycles(int cycles, int pc) {
    ui->cycles_label->setText("Clock Cycles: " + QString::number(cycles) + "\t\t" + "PC: " + QString::number(pc));
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
    browseAndUploadFile(ui->instruction_table);

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

