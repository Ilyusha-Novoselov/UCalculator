#ifndef INPUT_LINE_EDIT_H
#define INPUT_LINE_EDIT_H

#include <QLineEdit>
#include <QKeyEvent>
#include <QPainter>
#include <QTimer>
#include <QRegularExpression>

class InputLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    InputLineEdit(QWidget *parent = nullptr) : QLineEdit(parent), _focus(true) {};

public slots:
    void set_focus(bool focus){ _focus = focus; if(_focus) setFocus(); else clearFocus(); };

signals:
    void onKeyClicked(QString& text);

protected:
    void keyPressEvent(QKeyEvent *event) override
    {
        QString text = event->text();

        if(text.isEmpty())
            return;

        static QRegularExpression re("^[0-9+*/.-]$");
        if(re.match(text).hasMatch())
        {
            emit onKeyClicked(text);
            return;
        }

        if(text[0] >= 'a' && text[0] <= 'f')
        {
            QString command = text.toUpper();
            emit onKeyClicked(command);
            return;
        }

        if(text == "\b")
        {
            QString command = "Backspace";
            emit onKeyClicked(command);
            return;
        }

        if(text == "\r")
        {
            QString command = "=";
            emit onKeyClicked(command);
            return;
        }

        if(text == "\u007F")
        {
            QString command = "CE";
            emit onKeyClicked(command);
            return;
        }
    }

    void focusOutEvent(QFocusEvent *) override {
        if(_focus)
            setFocus();
        else
            clearFocus();
    }
private:
    bool _focus;
};

#endif // INPUT_LINE_EDIT_H
