#pragma once
#include <QMainWindow>
#include <array>
#include "Simulator.h"

class QTableWidget;
class QTableWidgetItem;
class QLabel;
class QLineEdit;
class QPlainTextEdit;
class QPushButton;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void onLoad();
    void onStep();
    void onRun();
    void onClearMemory();
    void onHelp();
    void onMemoryEdited(QTableWidgetItem* item);
    void onRegisterEdited(QTableWidgetItem* item);

private:
    void buildUi();
    void applyTheme();
    void refresh();
    void takeSnapshot();
    void showError(const QString& message);
    void offerNextAction();

    static QString hex(int value, int width);
    static QString describe(unsigned short ir);

    static const int MAX_STEPS = 10000;

    Simulator sim;
    std::array<unsigned char, 256> prevMem{};
    std::array<unsigned char, 16> prevRegs{};
    bool refreshing = false;

    // widgets
    QTableWidget* memTable = nullptr;
    QTableWidget* regTable = nullptr;
    QLabel* pcLabel = nullptr;
    QLabel* irLabel = nullptr;
    QLabel* nextLabel = nullptr;
    QLabel* stateLabel = nullptr;
    QPlainTextEdit* screenView = nullptr;
    QLineEdit* addressEdit = nullptr;
    QPushButton* loadBtn = nullptr;
    QPushButton* stepBtn = nullptr;
    QPushButton* runBtn = nullptr;
    QPushButton* clearBtn = nullptr;
    QPushButton* helpBtn = nullptr;
};
