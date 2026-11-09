#include "MainWindow.h"

#include <QFileDialog>
#include <QFontDatabase>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QStatusBar>
#include <QTableWidget>
#include <QVBoxLayout>
#include <exception>
#include <QApplication>

namespace {
const QColor PC_COLOR("#9be29b");
const QColor CHANGED_COLOR("#ffe680");
}

QString MainWindow::hex(int value, int width) {
    return QString("%1").arg(value, width, 16, QChar('0')).toUpper();
}


QString MainWindow::describe(unsigned short ir) {
    const int op = ir >> 12;
    const int d2 = (ir >> 8) & 0xF;
    const int d3 = (ir >> 4) & 0xF;
    const int d4 = ir & 0xF;
    const int xy = ir & 0xFF;
    const QString R = hex(d2, 1);
    const QString S = hex(d3, 1);
    const QString T = hex(d4, 1);
    const QString addr = hex(xy, 2);
    const QString word = hex(ir, 4);
    QString text;

    switch (op) {
    case 0x1: text = QString("LOAD R%1 <- memory[%2]").arg(R, addr); break;
    case 0x2: text = QString("LOAD R%1 <- value %2").arg(R, addr); break;
    case 0x3:
        if (xy == 0xFF) text = QString("STORE R%1 -> screen").arg(R);
        else            text = QString("STORE R%1 -> memory[%2]").arg(R, addr);
        break;
    case 0x4:
        if (d2 != 0) return word + "  -  invalid instruction";
        text = QString("MOVE R%1 -> R%2").arg(S, T);
        break;
    case 0x5: text = QString("ADD (integer) R%1 = R%2 + R%3").arg(R, S, T); break;
    case 0x6: text = QString("ADD (float) R%1 = R%2 + R%3").arg(R, S, T); break;
    case 0x7: text = QString("OR  R%1 = R%2 | R%3").arg(R, S, T); break;
    case 0x8: text = QString("AND R%1 = R%2 & R%3").arg(R, S, T); break;
    case 0x9: text = QString("XOR R%1 = R%2 ^ R%3").arg(R, S, T); break;
    case 0xA:
        if (d3 != 0) return word + "  -  invalid instruction";
        text = QString("ROTATE R%1 right by %2").arg(R).arg(d4);
        break;
    case 0xB:
        if (xy % 2 != 0) return word + "  -  invalid instruction";
        text = QString("JUMP to %1 if R%2 == R0").arg(addr, R);
        break;
    case 0xC:
        if ((ir & 0x0FFF) != 0) return word + "  -  invalid instruction";
        text = "HALT";
        break;
    default:
        return word + "  -  invalid instruction";
    }
    return word + "  -  " + text;
}

// ---------------------------------------------------------------- setup

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    buildUi();
    takeSnapshot();
    refresh();
    statusBar()->showMessage("Load a program to begin.");
}

void MainWindow::applyTheme() {
    QApplication::setStyle("Fusion");
    setStyleSheet(R"(
        QMainWindow, QWidget { background:#1e1e2e; color:#cdd6f4; font-size:10pt; }

        QGroupBox { border:1px solid #45475a; border-radius:6px;
                    margin-top:12px; padding-top:8px; font-weight:bold; }
        QGroupBox::title { subcontrol-origin:margin; left:10px;
                           padding:0 4px; color:#89b4fa; }

        QTableWidget { background:#181825; gridline-color:#313244;
                       border:1px solid #45475a;
                       selection-background-color:#45475a; selection-color:#ffffff; }
        QHeaderView::section { background:#313244; color:#89b4fa;
                               border:none; padding:2px; font-weight:bold; }
        QTableCornerButton::section { background:#313244; border:none; }

        QPlainTextEdit, QLineEdit { background:#181825; border:1px solid #45475a;
                                    border-radius:4px; padding:3px; }
        QLineEdit:focus, QPlainTextEdit:focus { border:1px solid #89b4fa; }

        QPushButton { background:#313244; border:1px solid #45475a;
                      border-radius:5px; padding:6px 14px; }
        QPushButton:hover { background:#45475a; }
        QPushButton:pressed { background:#585b70; }
        QPushButton:disabled { background:#232334; color:#6c7086; border-color:#313244; }

        QPushButton#loadBtn { background:#89b4fa; color:#11111b; font-weight:bold; }
        QPushButton#loadBtn:hover { background:#a6c8ff; }
        QPushButton#stepBtn, QPushButton#runBtn { background:#a6e3a1; color:#11111b; font-weight:bold; }
        QPushButton#stepBtn:hover, QPushButton#runBtn:hover { background:#bff0ba; }
        QPushButton#stepBtn:disabled, QPushButton#runBtn:disabled { background:#2a2b3c; color:#6c7086; }
        QPushButton#clearBtn { background:#f38ba8; color:#11111b; }
        QPushButton#clearBtn:hover { background:#f7a8bd; }

        QStatusBar { background:#181825; color:#a6adc8; }
        QToolTip { background:#313244; color:#cdd6f4; border:1px solid #45475a; }
    )");
}

void MainWindow::buildUi() {
    applyTheme();
    setWindowTitle("Vole Machine Simulator");
    resize(1150, 680);

    const QFont mono = QFontDatabase::systemFont(QFontDatabase::FixedFont);

    // ---------- top control bar
    loadBtn = new QPushButton("Load Program...");
    addressEdit = new QLineEdit("00");
    addressEdit->setMaximumWidth(50);
    addressEdit->setAlignment(Qt::AlignCenter);
    addressEdit->setFont(mono);
    addressEdit->setValidator(new QRegularExpressionValidator(
        QRegularExpression("[0-9A-Fa-f]{1,2}"), this));
    addressEdit->setToolTip("Start address in hex (must be even, 00 - FE)");
    stepBtn = new QPushButton("Step (F10)");
    runBtn = new QPushButton("Run (F5)");
    clearBtn = new QPushButton("Clear Memory");
    helpBtn = new QPushButton("Help");
    loadBtn->setObjectName("loadBtn");
    stepBtn->setObjectName("stepBtn");
    runBtn->setObjectName("runBtn");
    clearBtn->setObjectName("clearBtn");
    stepBtn->setShortcut(QKeySequence(Qt::Key_F10));
    runBtn->setShortcut(QKeySequence(Qt::Key_F5));
    stateLabel = new QLabel;
    stateLabel->setAlignment(Qt::AlignCenter);
    stateLabel->setMinimumWidth(90);

    auto* topBar = new QHBoxLayout;
    topBar->addWidget(loadBtn);
    topBar->addWidget(new QLabel("at address (hex):"));
    topBar->addWidget(addressEdit);
    topBar->addSpacing(20);
    topBar->addWidget(stepBtn);
    topBar->addWidget(runBtn);
    topBar->addSpacing(20);
    topBar->addWidget(clearBtn);
    topBar->addStretch();
    topBar->addWidget(stateLabel);
    topBar->addWidget(helpBtn);

    // ---------- CPU panel (PC, IR, registers)
    pcLabel = new QLabel;
    irLabel = new QLabel;
    pcLabel->setFont(mono);
    irLabel->setFont(mono);
    auto* pcirGrid = new QGridLayout;
    pcirGrid->addWidget(new QLabel("PC"), 0, 0);
    pcirGrid->addWidget(pcLabel, 0, 1);
    pcirGrid->addWidget(new QLabel("IR"), 1, 0);
    pcirGrid->addWidget(irLabel, 1, 1);
    pcirGrid->setColumnStretch(2, 1);

    regTable = new QTableWidget(16, 2);
    regTable->setHorizontalHeaderLabels({"Hex", "Dec"});
    regTable->setFont(mono);
    for (int i = 0; i < 16; ++i) {
        regTable->setVerticalHeaderItem(i, new QTableWidgetItem("R" + hex(i, 1)));
        auto* h = new QTableWidgetItem("00");
        auto* d = new QTableWidgetItem("0");
        h->setTextAlignment(Qt::AlignCenter);
        d->setTextAlignment(Qt::AlignCenter);
        d->setFlags(d->flags() & ~Qt::ItemIsEditable);
        regTable->setItem(i, 0, h);
        regTable->setItem(i, 1, d);
    }
    regTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    regTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    regTable->setToolTip("Double-click a Hex value to overwrite the register");

    auto* cpuBox = new QGroupBox("CPU");
    auto* cpuLayout = new QVBoxLayout(cpuBox);
    cpuLayout->addLayout(pcirGrid);
    cpuLayout->addWidget(regTable);
    cpuBox->setFixedWidth(190);

    // ---------- Memory panel
    memTable = new QTableWidget(16, 16);
    memTable->setFont(mono);
    QStringList heads;
    for (int i = 0; i < 16; ++i) heads << hex(i, 1);
    memTable->setHorizontalHeaderLabels(heads);
    memTable->setVerticalHeaderLabels(heads);
    for (int r = 0; r < 16; ++r) {
        for (int c = 0; c < 16; ++c) {
            auto* item = new QTableWidgetItem("00");
            item->setTextAlignment(Qt::AlignCenter);
            memTable->setItem(r, c, item);
        }
    }
    memTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    memTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    memTable->setToolTip("Double-click a cell to overwrite it");

    auto* memBox = new QGroupBox("Main Memory  (address = row digit + column digit, e.g. row A / column 3 = A3)");
    auto* memLayout = new QVBoxLayout(memBox);
    memLayout->addWidget(memTable);

    // ---------- Screen + next instruction panel
    nextLabel = new QLabel;
    nextLabel->setFont(mono);
    nextLabel->setWordWrap(true);
    auto* nextBox = new QGroupBox("Next instruction");
    auto* nextLayout = new QVBoxLayout(nextBox);
    nextLayout->addWidget(nextLabel);

    screenView = new QPlainTextEdit;
    screenView->setReadOnly(true);
    screenView->setFont(mono);
    screenView->setPlaceholderText("Program output appears here (stores to address FF).");
    auto* screenBox = new QGroupBox("Screen");
    auto* screenLayout = new QVBoxLayout(screenBox);
    screenLayout->addWidget(screenView);

    auto* rightCol = new QVBoxLayout;
    rightCol->addWidget(nextBox);
    rightCol->addWidget(screenBox, 1);
    auto* rightWidget = new QWidget;
    rightWidget->setLayout(rightCol);
    rightWidget->setFixedWidth(260);
    rightCol->setContentsMargins(0, 0, 0, 0);

    // ---------- assemble
    auto* body = new QHBoxLayout;
    body->addWidget(cpuBox);
    body->addWidget(memBox, 1);
    body->addWidget(rightWidget);

    auto* root = new QVBoxLayout;
    root->addLayout(topBar);
    root->addLayout(body, 1);
    auto* central = new QWidget;
    central->setLayout(root);
    setCentralWidget(central);

    // ---------- connections
    connect(loadBtn, &QPushButton::clicked, this, &MainWindow::onLoad);
    connect(stepBtn, &QPushButton::clicked, this, &MainWindow::onStep);
    connect(runBtn, &QPushButton::clicked, this, &MainWindow::onRun);
    connect(clearBtn, &QPushButton::clicked, this, &MainWindow::onClearMemory);
    connect(helpBtn, &QPushButton::clicked, this, &MainWindow::onHelp);
    connect(memTable, &QTableWidget::itemChanged, this, &MainWindow::onMemoryEdited);
    connect(regTable, &QTableWidget::itemChanged, this, &MainWindow::onRegisterEdited);
}

// ---------------------------------------------------------------- display

void MainWindow::takeSnapshot() {
    for (int i = 0; i < 256; ++i) prevMem[i] = sim.getMemoryCell(i);
    for (int i = 0; i < 16; ++i) prevRegs[i] = sim.getRegister(i);
}

void MainWindow::refresh() {
    refreshing = true;
    const int pc = sim.getPC();
    const bool halted = sim.isHalted();

    // memory
    for (int i = 0; i < 256; ++i) {
        QTableWidgetItem* it = memTable->item(i / 16, i % 16);
        const unsigned char v = sim.getMemoryCell(i);
        it->setText(hex(v, 2));
        if (!halted && (i == pc || i == pc + 1)) {
            it->setBackground(PC_COLOR);
            it->setForeground(QBrush(Qt::black));
        } else if (v != prevMem[i]) {
            it->setBackground(CHANGED_COLOR);
            it->setForeground(QBrush(Qt::black));
        } else {
            it->setBackground(QBrush());
            it->setForeground(v == 0 ? QBrush(QColor("#6c7086")) : QBrush());
        }
    }

    // registers
    for (int i = 0; i < 16; ++i) {
        const unsigned char v = sim.getRegister(i);
        QTableWidgetItem* h = regTable->item(i, 0);
        QTableWidgetItem* d = regTable->item(i, 1);
        h->setText(hex(v, 2));
        d->setText(QString::number(v));
        if (v != prevRegs[i]) {
            h->setBackground(CHANGED_COLOR);
            d->setBackground(CHANGED_COLOR);
            h->setForeground(QBrush(Qt::black));
            d->setForeground(QBrush(Qt::black));
        } else {
            h->setBackground(QBrush());
            d->setBackground(QBrush());
            h->setForeground(QBrush());
            d->setForeground(QBrush());
        }
    }

    // PC / IR / next instruction
    pcLabel->setText(hex(pc, 2));
    irLabel->setText(hex(sim.getIR(), 4));
    if (halted || pc > 254) {
        nextLabel->setText("-");
    } else {
        const unsigned short word =
            static_cast<unsigned short>((sim.getMemoryCell(pc) << 8) | sim.getMemoryCell(pc + 1));
        nextLabel->setText(describe(word));
    }

    // screen
    screenView->setPlainText(QString::fromStdString(sim.getScreenText()));

    // state + buttons
    stateLabel->setText(halted ? "HALTED" : "READY");
    stateLabel->setStyleSheet(halted
                                  ? "background:#d9534f; color:white; padding:3px; border-radius:3px;"
                                  : "background:#3c9d3c; color:white; padding:3px; border-radius:3px;");
    stepBtn->setEnabled(!halted);
    runBtn->setEnabled(!halted);

    refreshing = false;
}

void MainWindow::showError(const QString& message) {
    QMessageBox::warning(this, "Vole Machine Simulator", message);
}

void MainWindow::offerNextAction() {
    QMessageBox box(this);
    box.setWindowTitle("Program finished");
    box.setText("The program has finished running.\nWhat would you like to do?");
    QPushButton* again = box.addButton("Load another program", QMessageBox::AcceptRole);
    QPushButton* quit = box.addButton("Exit", QMessageBox::DestructiveRole);
    box.addButton("Stay here", QMessageBox::RejectRole);
    box.exec();
    if (box.clickedButton() == again) onLoad();
    else if (box.clickedButton() == quit) close();
}

// ---------------------------------------------------------------- actions

void MainWindow::onLoad() {
    bool ok = false;
    const int address = addressEdit->text().trimmed().toInt(&ok, 16);
    if (!ok || address % 2 != 0) {
        showError("The start address must be an even hex number (00, 02, 04, ... FE).");
        return;
    }

    const QString path = QFileDialog::getOpenFileName(
        this, "Open a Vole program", QString(), "Vole programs (*.txt);;All files (*)");
    if (path.isEmpty()) return;

    try {
        sim.loadProgram(path.toStdString(), address);
    } catch (const std::exception& e) {
        takeSnapshot();
        refresh();
        showError(QString("Could not load the program:\n%1").arg(e.what()));
        return;
    }
    takeSnapshot();
    refresh();
    statusBar()->showMessage("Program loaded at " + hex(address, 2) +
                             ". Press Step or Run.");
}

void MainWindow::onStep() {
    if (sim.isHalted()) return;
    takeSnapshot();
    try {
        sim.step();
    } catch (const std::exception& e) {
        refresh();
        showError(QString("Execution error: %1").arg(e.what()));
        return;
    }
    refresh();
    if (sim.isHalted()) {
        statusBar()->showMessage("Program finished.");
        offerNextAction();
    } else {
        statusBar()->showMessage("Step executed.");
    }
}

void MainWindow::onRun() {
    if (sim.isHalted()) return;
    takeSnapshot();
    int steps = 0;
    try {
        while (!sim.isHalted() && steps < MAX_STEPS) {
            sim.step();
            ++steps;
        }
    } catch (const std::exception& e) {
        refresh();
        showError(QString("Execution error: %1").arg(e.what()));
        return;
    }
    refresh();
    if (!sim.isHalted()) {
        showError(QString("Stopped after %1 steps - the program may be in an infinite loop.")
                      .arg(MAX_STEPS));
        return;
    }
    statusBar()->showMessage(QString("Program finished after %1 steps.").arg(steps));
    offerNextAction();
}

void MainWindow::onClearMemory() {
    sim.reset();
    takeSnapshot();
    refresh();
    statusBar()->showMessage("Memory, registers and screen cleared.");
}

void MainWindow::onMemoryEdited(QTableWidgetItem* item) {
    if (refreshing) return;
    const int index = item->row() * 16 + item->column();
    bool ok = false;
    const uint value = item->text().trimmed().toUInt(&ok, 16);
    if (!ok || value > 0xFF) {
        showError("Enter a hex value between 00 and FF.");
        refresh();
        return;
    }
    takeSnapshot();
    sim.setMemoryCell(index, static_cast<unsigned char>(value));
    refresh();
}

void MainWindow::onRegisterEdited(QTableWidgetItem* item) {
    if (refreshing || item->column() != 0) return;
    bool ok = false;
    const uint value = item->text().trimmed().toUInt(&ok, 16);
    if (!ok || value > 0xFF) {
        showError("Enter a hex value between 00 and FF.");
        refresh();
        return;
    }
    takeSnapshot();
    sim.setRegister(item->row(), static_cast<unsigned char>(value));
    refresh();
}

void MainWindow::onHelp() {
    QMessageBox::information(this, "How to use the simulator",
                             "1. Type an even start address (hex), press 'Load Program...' and pick a program file.\n"
                             "2. The green cells in memory are the instruction that will run next.\n"
                             "3. 'Step' runs one instruction; 'Run' runs until HALT.\n"
                             "4. Yellow cells/registers changed in the last action.\n"
                             "5. Double-click any memory cell or register Hex value to overwrite it.\n"
                             "6. The Screen box shows everything stored to address FF.\n"
                             "7. 'Clear Memory' resets memory, registers, PC, IR and the screen.");
}