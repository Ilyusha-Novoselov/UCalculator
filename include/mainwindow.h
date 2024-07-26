#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

#include "pcontrol.h"
#include "fcontrol.h"
#include "ccontrol.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void addHistory();

    void removeAllUpToOperator(QString& str);

public slots:
    void onButtonClicked();

    void DoButtonCommand(QString& command);

signals:
    void set_focus(bool focus);

private slots:
    void on_horizontalSlider_valueChanged(int value);

    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_tabWidget_currentChanged(int index);

private:
    Ui::MainWindow *ui;
    UCalculator::AControl* _control;
    UCalculator::PControl* _control_p;
    UCalculator::FControl* _control_f;
    UCalculator::CControl* _control_c;
    QString _temp; // Хранит заменяемый правый операнд для смены в отображении цельного выражения
    bool _cOpDone = false; // Флаг для показа, введена операция или нет

    static inline std::map<std::string, int> editorMap = {
        {"0", 0}, {".", 16}, {"Backspace", 17}, {"CE", 18}, {"+/-", 19},
        {"|", 16}, {"i*", 16}
    };

    static inline std::map<std::string, int> processorMap = {
        {"+", 2}, {"-", 3}, {"*", 4}, {"/", 5}, {"1/x", 6},
        {"^2", 7}, {"=", 10}
    };

    static inline std::map<std::string, int> memoryMap = {
        {"MS", 1}, {"MR", 2}, {"M+", 3}, {"MC", 4}
    };
};
#endif // MAINWINDOW_H
