#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _control = new UCalculator::Control<UCalculator::PNumber>;
    ui->lineEdit->setFocus(Qt::OtherFocusReason);

    // Привязываем сигналы кнопок к одному слоту
    QList<QPushButton *> buttons = findChildren<QPushButton *>();
    for (QPushButton *button : buttons)
        connect(button, SIGNAL(clicked()), this, SLOT(onButtonClicked()));

    connect(ui->lineEdit, &InputLineEdit::onKeyClicked, this, &MainWindow::DoButtonCommand);
    //connect(this, &MainWindow::set_focus, ui->lineEdit, &InputLineEdit::set_focus);

    ui->horizontalSlider->setRange(2, 16);
    ui->horizontalSlider->setSingleStep(1);
    ui->horizontalSlider->setPageStep(1);
    ui->horizontalSlider->setValue(10);
    ui->horizontalSlider->setTickPosition(QSlider::TicksBelow);
    ui->horizontalSlider->setTickInterval(1);
    ui->P->setText(QString::number(ui->horizontalSlider->value()));

    ui->MR_button->setEnabled(false);
    ui->MC_button->setEnabled(false);
    ui->Button_Madd->setEnabled(false);

    ui->comboBox->addItem("P-ые числа");
    ui->comboBox->addItem("Дробные числа");
    ui->comboBox->addItem("Комплексные числа");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::removeAllUpToOperator(QString &str)
{
    if(str[0] == '-')
        str.remove(0,1);
    // Найти позицию первого вхождения одного из символов +, -, *, /
    static QRegularExpression reg("[+\\-*/]");
    int pos = str.indexOf(reg);

    // Если найдено вхождение, удалить все символы до этой позиции
    if (pos != -1)
        str = str.mid(pos);
    else
        // Если ни один из символов не найден, очистить строку
        str.clear();
}

void MainWindow::onButtonClicked()
{
    QString text = qobject_cast<QPushButton *>(sender())->text();
    DoButtonCommand(text);
}

void MainWindow::DoButtonCommand(QString &command)
{
    //------------------------------------------------------------------//
    // Не реагировать, если кнопка не активна
    //------------------------------------------------------------------//
    QList<QPushButton *> buttons = findChildren<QPushButton *>();
    for (QPushButton *button : buttons)
        if(command == button->text() && !button->isEnabled())
            return;

    //------------------------------------------------------------------//
    // Вызов очистки всего калькулятора
    //------------------------------------------------------------------//
    if(command == "CC")
    {
        _control->DoReset(true);
        ui->lineEdit->setText("");
        ui->lineEdit_2->setText("");
        ui->horizontalSlider->setValue(10);
        ui->MR_button->setEnabled(false);
        ui->MC_button->setEnabled(false);
        ui->Button_Madd->setEnabled(false);
        _cOpDone = false;
        _temp = "";
        return;
    }

    //------------------------------------------------------------------//
    // Вызов функций памяти
    //------------------------------------------------------------------//
    static QRegularExpression memoryCommand("MC|M+|MR|MS$");
    if(memoryCommand.match(command).hasMatch())
    {
        auto st = _control->get_state();
        if(command == "MC")
        {
            ui->MR_button->setEnabled(false);
            ui->MC_button->setEnabled(false);
            ui->Button_Madd->setEnabled(false);
        }
        else
        {
            ui->MR_button->setEnabled(true);
            ui->MC_button->setEnabled(true);
            ui->Button_Madd->setEnabled(true);
        }

        auto mem = _control->DoMemoryCommand(memoryMap[command.toStdString()]);
        if(command == "MR" && _control->get_p() != mem.get_p())
        {
            _control->set_p(mem.get_p());
            ui->lineEdit_2->setText("");
            ui->lineEdit->setText(QString::fromStdString(mem.GetNumber()));
            _control->set_state(UCalculator::cMemDone);
            ui->horizontalSlider->setValue(mem.get_p());
            _control->set_state(UCalculator::cEditing);
            return;
        }

        ui->lineEdit->setText(QString::fromStdString(_control->DoEditorCommand(20)));
        _control->set_state(st);
        return;
    }

    //------------------------------------------------------------------//
    // Вызов функций калькулятора
    //------------------------------------------------------------------//
    static QRegularExpression Functions("1/x|\\^2$");
    if(Functions.match(command).hasMatch())
    {
        // Для одного операнда
        if(!_cOpDone)
        {
            _control->DoProcessorCommand(9); // Устанавливаем правый операнд
            _control->DoProcessorCommand(processorMap[command.toStdString()]); // Вызываем функцию
            _temp = QString::fromStdString(_control->DoEditorCommand(20));
            ui->lineEdit_2->setText(QString::fromStdString(_control->DoEditorCommand(20)) + "=");
            _control->set_state(UCalculator::FunDone);
            _control->DoProcessorCommand(processorMap["="]); // Вычисляем выражение
            ui->lineEdit->setText(QString::fromStdString(_control->DoEditorCommand(20))); // Берем число из редактора
            _control->set_state(UCalculator::FunDone);
            return;
        }

        bool flag = true; // Флаг, для определения первый раз ли вызывается функция или нет
        // Если повторно вызываем функцию, то правый операнд устанавливать не нужно
        if(_control->get_state() != UCalculator::FunDone && _control->get_state() != UCalculator::cExpDone)
            _control->DoProcessorCommand(9); // Устанавливаем правый операнд
        else
            flag = false;

        _control->DoProcessorCommand(processorMap[command.toStdString()]); // Вызываем функцию
        // Первый раз вызвали
        if(flag)
        {
            _temp = QString::fromStdString(_control->DoEditorCommand(20));
            ui->lineEdit_2->setText(ui->lineEdit_2->text() + QString::fromStdString(_control->DoEditorCommand(20)) + "=");
        }
        // Не первый раз
        else
        {
            ui->lineEdit_2->setText(ui->lineEdit_2->text().replace(_temp, QString::fromStdString(_control->DoEditorCommand(20))));
            _temp = QString::fromStdString(_control->DoEditorCommand(20));
        }
        _control->set_state(UCalculator::FunDone);
        _control->DoProcessorCommand(processorMap["="]); // Вычисляем выражение
        ui->lineEdit->setText(QString::fromStdString(_control->DoEditorCommand(20))); // Берем число из редактора
        _control->set_state(UCalculator::FunDone);
        return;
    }

    //------------------------------------------------------------------//
    // Вызов команды редактора
    //------------------------------------------------------------------//
    static QRegularExpression editorCommand("^[0-9A-F.]|Backspace|CE|\\+/-$");
    if(editorCommand.match(command).hasMatch())
    {
        // Очищаем, если выражение было вычислено
        if(_control->get_state() == UCalculator::cExpDone)
        {
            ui->lineEdit->setText("");
            ui->lineEdit_2->setText("");
            _control->DoEditorCommand(18);
            _control->DoProcessorCommand(0);
            _cOpDone = false;
        }

        int num = Converter::Converter_P_10::char_to_num(static_cast<char>(command[0].unicode()));
        if(num >= 1 && num <= 15 && command.length() == 1)
            ui->lineEdit->setText(QString::fromStdString(_control->DoEditorCommand(num)));
        else
            ui->lineEdit->setText(QString::fromStdString(_control->DoEditorCommand(editorMap[command.toStdString()])));
        return;
    }

    //------------------------------------------------------------------//
    // Вызов команды процессора
    //------------------------------------------------------------------//
    static QRegularExpression processorCommand("^[+*/=-]$");
    if(processorCommand.match(command).hasMatch())
    {
        auto st = _control->get_state();
        if(_control->get_state() != UCalculator::cExpDone)
            _temp = QString::fromStdString(_control->DoEditorCommand(20));
        _control->set_state(st);
        _cOpDone = true;
        if(command == "=")
        {
            // Для повторной операции
            if(_control->get_state() == UCalculator::cExpDone)
            {
                QString str = ui->lineEdit_2->text();
               removeAllUpToOperator(str);
                ui->lineEdit_2->setText(QString::fromStdString(_control->DoEditorCommand(20)) + str);
               _control->DoProcessorCommand(processorMap[command.toStdString()]); // Вычисляем выражение
                ui->lineEdit->setText(QString::fromStdString(_control->DoEditorCommand(20)));
               _control->set_state(UCalculator::cExpDone);
            }
            else
            {
                ui->lineEdit_2->setText(ui->lineEdit_2->text() + QString::fromStdString(_control->DoEditorCommand(20)) + "=");
                _control->DoProcessorCommand(9); // Устанавливаем правый операнд
                _control->DoProcessorCommand(processorMap[command.toStdString()]); // Вычисляем выражение
                ui->lineEdit->setText(QString::fromStdString(_control->DoEditorCommand(20)));
                _control->set_state(UCalculator::cExpDone);
            }
        }
        else
        {
            if(_control->get_state() == UCalculator::cStart)
            {
                ui->lineEdit_2->setText("0" + command);
                _control->DoProcessorCommand(processorMap[command.toStdString()]); // Устанавливаем нужную операцию
                return;
            }

            // Если захотим сменить знак
            if(_control->get_state() == UCalculator::cOpDone)
            {
                QString str = ui->lineEdit_2->text();
                str.chop(1);
                ui->lineEdit_2->setText(str + command);
                _control->DoProcessorCommand(processorMap[command.toStdString()]); // Устанавливаем нужную операцию
                _control->set_state(UCalculator::cOpDone);
            }
            else
            {
                ui->lineEdit_2->setText(QString::fromStdString(_control->DoEditorCommand(20)) + command);
                _control->DoProcessorCommand(8);// Устанавливаем левый операнд
                _control->DoProcessorCommand(processorMap[command.toStdString()]); // Устанавливаем нужную операцию
                _control->DoEditorCommand(18); // Очищаем редактор
                _control->set_state(UCalculator::cOpDone);
            }
        }
        return;
    }
}


void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    ui->P->setText(QString::number(value));

    // Смена системы счисления результата (только для завершенных выражений)
    if(_control->get_state() == UCalculator::cExpDone)
    {
        // Визуальная смена
        QString str = ui->lineEdit_2->text();
        str.chop(1);
        if(str[0] == '-')
            str.remove(0, 1);
        static QRegularExpression reg("[+*/-]");
        int pos = str.indexOf(reg);
        QString subString_1, subString_2;
        if (pos != -1)
        {
            subString_1 = str.left(pos);
            subString_2 = str.mid(pos + 1);
        }
        QString str_1 = ui->lineEdit_2->text();
        if(str_1[0] == '-')
            subString_1 = "-" + subString_1;
        UCalculator::PNumber lop(subString_1.toStdString(), _control->get_p());
        UCalculator::PNumber rop(subString_2.toStdString(), _control->get_p());
        UCalculator::PNumber result(ui->lineEdit->text().toStdString(), _control->get_p());

        result.turnTo(value);
        rop.turnTo(value);
        lop.turnTo(value);
        _temp = QString::fromStdString(rop.GetNumber());

        ui->lineEdit_2->setText(QString::fromStdString(lop.GetNumber()) + str[pos] + QString::fromStdString(rop.GetNumber()) + "=");
        ui->lineEdit->setText(QString::fromStdString(result.GetNumber()));

        // Смена в процессоре
        _control->set_p(value);
        _control->DoProcessorCommand(11);

        _control->set_state(UCalculator::cExpDone);
    }
    else
    {
        if(_control->get_state() != UCalculator::cMemDone)
        {
            _control->DoReset();
            ui->lineEdit->setText("");
            ui->lineEdit_2->setText("");
        }
    }

    _control->set_p(value);

    static QRegularExpression textButton("^[0-9A-F]$");
    QList<QPushButton *> buttons = findChildren<QPushButton *>();
    for (QPushButton *button : buttons)
    {
        if(button->objectName() == "C_button")
            continue;
        QString str = button->text();
        if(textButton.match(str).hasMatch())
        {
            if(Converter::Converter_P_10::char_to_num(static_cast<char>(str[0].unicode())) >= value)
                button->setEnabled(false);
            else
                button->setEnabled(true);
        }
    }
}

