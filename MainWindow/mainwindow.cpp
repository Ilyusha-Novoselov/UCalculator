#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _control_p = new UCalculator::PControl;
    _control_f = new UCalculator::FControl;
    _control_c = new UCalculator::CControl;

    _control = _control_p;

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

    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setHorizontalHeaderLabels({"Lop", "Op", "Rop", "Result", "P"});
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QString htmlText = R"(
        <html>
            <body style='white-space: pre; font-family: "Courier New", Courier, monospace; font-size: 12pt;'>
<b>Калькулятор p1 p2:</b>

<b>Функции кнопок:</b>
-[0-F] Ввод в нужной с.сч.
-BS удалить символ справа
-CE очистить поле редактора
-. разделитель
-= вычислить результат
"+-*/" арифметические операции
"^2" возведение в квадрат
"1/x" найти обратное число
"+/-" сменить знак числа

<b>Функция ползунка:</b>
-Выбор с.сч. от 2 до 16

<b>Клавиатура:</b>
-Доступен ввод для символов
[0-F] и [.]
-BS удалить символ справа
-DEL очистить поле ввода

<b>История:</b>
-Lop - левый операнд
-Op - операция
-Rop - правый операнд
-Result - результат операции
-P - система счисления
            </body>
        </html>)";
    ui->textEdit->setHtml(htmlText);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addHistory()
{
    if(ui->comboBox->currentText() == "P-ые числа")
    {
        auto history = _control_p->get_history();
        if(!history.empty())
        {
            int rowCount = ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(rowCount);
            ui->tableWidget->setItem(rowCount, 0, new QTableWidgetItem(QString::fromStdString(history.rbegin()->lop.GetNumber())));
            ui->tableWidget->setItem(rowCount, 1, new QTableWidgetItem(QString::fromStdString(history.rbegin()->operation)));
            ui->tableWidget->setItem(rowCount, 2, new QTableWidgetItem(QString::fromStdString(history.rbegin()->rop.GetNumber())));
            ui->tableWidget->setItem(rowCount, 3, new QTableWidgetItem(QString::fromStdString(history.rbegin()->result.GetNumber())));
            ui->tableWidget->setItem(rowCount, 4, new QTableWidgetItem(QString::number(history.rbegin()->p)));
        }
    }

    if(ui->comboBox->currentText() == "Дробные числа")
    {
        auto history = _control_f->get_history();
        if(!history.empty())
        {
            int rowCount = ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(rowCount);
            ui->tableWidget->setItem(rowCount, 0, new QTableWidgetItem(QString::fromStdString(history.rbegin()->lop.GetNumber())));
            ui->tableWidget->setItem(rowCount, 1, new QTableWidgetItem(QString::fromStdString(history.rbegin()->operation)));
            ui->tableWidget->setItem(rowCount, 2, new QTableWidgetItem(QString::fromStdString(history.rbegin()->rop.GetNumber())));
            ui->tableWidget->setItem(rowCount, 3, new QTableWidgetItem(QString::fromStdString(history.rbegin()->result.GetNumber())));
            ui->tableWidget->setItem(rowCount, 4, new QTableWidgetItem(QString::number(history.rbegin()->p)));
        }
    }

    if(ui->comboBox->currentText() == "Комплексные числа")
    {
        auto history = _control_c->get_history();
        if(!history.empty())
        {
            int rowCount = ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(rowCount);
            ui->tableWidget->setItem(rowCount, 0, new QTableWidgetItem(QString::fromStdString(history.rbegin()->lop.GetNumber())));
            ui->tableWidget->setItem(rowCount, 1, new QTableWidgetItem(QString::fromStdString(history.rbegin()->operation)));
            ui->tableWidget->setItem(rowCount, 2, new QTableWidgetItem(QString::fromStdString(history.rbegin()->rop.GetNumber())));
            ui->tableWidget->setItem(rowCount, 3, new QTableWidgetItem(QString::fromStdString(history.rbegin()->result.GetNumber())));
            ui->tableWidget->setItem(rowCount, 4, new QTableWidgetItem(QString::number(history.rbegin()->p)));
        }
    }
}

void MainWindow::removeAllUpToOperator(QString &str)
{
    if(ui->comboBox->currentText() == "Комплексные числа")
    {
        // Находим позицию первого слагаемого
        static QRegularExpression reg("[+-]");
        int first_plus_index = str.indexOf(reg, 1);

        // Если нашли знак, находим следующий знак (после первого слагаемого)
        if (first_plus_index != -1) {
            int second_plus_index = str.indexOf(reg, first_plus_index + 1);
            if (second_plus_index != -1) {
                if(str[second_plus_index - 1] == '*')
                    second_plus_index = str.indexOf(reg, second_plus_index + 1);;
                // Удаляем первое слагаемое
                str.remove(0, second_plus_index);
            }
        }
    }
    else
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
}

void MainWindow::onButtonClicked()
{
    QString text = qobject_cast<QPushButton *>(sender())->text();
    DoButtonCommand(text);
}

void MainWindow::DoButtonCommand(QString &command)
{
    try{
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
    // Вызов функций комплексного числа
    //------------------------------------------------------------------//
    if(command == "Mdl" || command == "Phi")
    {
        auto num = _control->DoEditorCommand(20);

        UCalculator::CNumber number(num, _control->get_p());

        if(command == "Mdl")
        {
            auto z = number.Modulus();
            ui->lineEdit_2->setText(QString::fromStdString(z.GetNumber()));
        }

        if(command == "Phi")
        {
            auto z = number.Argument();
            ui->lineEdit_2->setText(QString::fromStdString(z.GetNumber()));
        }
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
        if(command == "MR" && mem != _control->get_p())
        {
            ui->lineEdit_2->setText("");
            ui->lineEdit->setText(QString::fromStdString(_control->DoEditorCommand(20)));
            _control->set_state(UCalculator::cMemDone);
            ui->horizontalSlider->setValue(mem);
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
    if(editorCommand.match(command).hasMatch() || command == "|" || command == "i*")
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
                addHistory();
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
        addHistory();
        return;
    }
    }
    catch(...)
    {
        QMessageBox::warning(nullptr, "Предупреждение", "Ошибка ввода!");
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

        if(ui->comboBox->currentText() == "P-ые числа")
        {
            UCalculator::PNumber lop(subString_1.toStdString(), _control->get_p());
            UCalculator::PNumber rop(subString_2.toStdString(), _control->get_p());
            UCalculator::PNumber result(ui->lineEdit->text().toStdString(), _control->get_p());

            result.turnTo(value);
            rop.turnTo(value);
            lop.turnTo(value);
            _temp = QString::fromStdString(rop.GetNumber());

            ui->lineEdit_2->setText(QString::fromStdString(lop.GetNumber()) + str[pos] + QString::fromStdString(rop.GetNumber()) + "=");
            ui->lineEdit->setText(QString::fromStdString(result.GetNumber()));
        }
        if(ui->comboBox->currentText() == "Дробные числа")
        {
            UCalculator::FNumber lop(subString_1.toStdString(), _control->get_p());
            UCalculator::FNumber rop(subString_2.toStdString(), _control->get_p());
            UCalculator::FNumber result(ui->lineEdit->text().toStdString(), _control->get_p());

            result.turnTo(value);
            rop.turnTo(value);
            lop.turnTo(value);
            _temp = QString::fromStdString(rop.GetNumber());

            ui->lineEdit_2->setText(QString::fromStdString(lop.GetNumber()) + str[pos] + QString::fromStdString(rop.GetNumber()) + "=");
            ui->lineEdit->setText(QString::fromStdString(result.GetNumber()));
        }
        if(ui->comboBox->currentText() == "Комплексные числа")
        {
            // Находим позицию первого слагаемого
            static QRegularExpression reg("[+-]");
            int first_plus_index = str.indexOf(reg, 1);

            // Если нашли знак, находим следующий знак (после первого слагаемого)
            if (first_plus_index != -1) {
                int second_plus_index = str.indexOf(reg, first_plus_index + 1);
                if (second_plus_index != -1) {
                    if(str[second_plus_index - 1] == '*')
                        second_plus_index = str.indexOf(reg, second_plus_index + 1);;
                    subString_1 = str.mid(0, second_plus_index);
                    subString_2 = str.mid(second_plus_index + 1, str.length() - second_plus_index);
                }
            }

            UCalculator::CNumber lop(subString_1.toStdString(), _control->get_p());
            UCalculator::CNumber rop(subString_2.toStdString(), _control->get_p());
            UCalculator::CNumber result(ui->lineEdit->text().toStdString(), _control->get_p());

            result.turnTo(value);
            rop.turnTo(value);
            lop.turnTo(value);
            _temp = QString::fromStdString(rop.GetNumber());

            ui->lineEdit_2->setText(QString::fromStdString(lop.GetNumber()) + str[pos] + QString::fromStdString(rop.GetNumber()) + "=");
            ui->lineEdit->setText(QString::fromStdString(result.GetNumber()));
        }


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


void MainWindow::on_comboBox_currentTextChanged(const QString &arg1)
{
    if(arg1 == "P-ые числа")
    {
        _control->DoReset(true);
        _control = _control_p;
        ui->Button_dot->setEnabled(true);
        ui->Button_frac->setEnabled(false);
        ui->Button_im->setEnabled(false);
        ui->Button_mdl->setEnabled(false);
        ui->Button_phi->setEnabled(false);
        _temp = "";
        _cOpDone = false;
        ui->horizontalSlider->setValue(10);
        ui->horizontalSlider->setValue(10);
        ui->lineEdit->setText("");
        ui->lineEdit_2->setText("");
    }

    if(arg1 == "Дробные числа")
    {
        _control->DoReset(true);
        _control = _control_f;
        ui->Button_dot->setEnabled(false);
        ui->Button_frac->setEnabled(true);
        ui->Button_im->setEnabled(false);
        ui->Button_mdl->setEnabled(false);
        ui->Button_phi->setEnabled(false);
        _temp = "";
        _cOpDone = false;
        ui->horizontalSlider->setValue(10);
        ui->horizontalSlider->setValue(10);
        ui->lineEdit->setText("");
        ui->lineEdit_2->setText("");
    }

    if(arg1 == "Комплексные числа")
    {
        _control->DoReset(true);
        _control = _control_c;
        ui->Button_dot->setEnabled(false);
        ui->Button_frac->setEnabled(false);
        ui->Button_im->setEnabled(true);
        ui->Button_mdl->setEnabled(true);
        ui->Button_phi->setEnabled(true);
        _temp = "";
        _cOpDone = false;
        ui->horizontalSlider->setValue(10);
        ui->horizontalSlider->setValue(10);
        ui->lineEdit->setText("");
        ui->lineEdit_2->setText("");
    }
}

