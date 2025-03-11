#include <QtHelper.h>
#include <WidgetCalculator.h>

#include <QDebug>
#include <QGridLayout>
#include <QRegExpValidator>

#include <IntXHelper.h>

WidgetCalculator::WidgetCalculator() {
  [[maybe_unused]] bool connected;

  QGridLayout* q_grid_layout_window = new QGridLayout(this);
  QGridLayout* q_grid_widget = new QGridLayout;
  q_grid_layout_window->addLayout(q_grid_widget, 1, 1, Qt::AlignCenter);
  q_grid_widget->setGeometry(QRect());

  line_edit = new QLineEdit();
  line_edit->setReadOnly(true);
  line_edit->setAlignment(Qt::AlignRight);
  line_edit->setFocusPolicy(Qt::NoFocus);
  q_grid_widget->addWidget(line_edit, 0, 0, 1, 6);

  auto add_calc_btn = [&](const QString& btn_name, size_t row, size_t column,
                          ButtonFeatures features) {
    QPushButton* btn;
    btn = new QPushButton(btn_name);
    btn->setFixedSize(60, 60);

    auto set_color = [&](QColor color) {
      QPalette pal = btn->palette();
      pal.setColor(QPalette::Button, color);
      btn->setAutoFillBackground(true);
      btn->setPalette(pal);
      btn->update();
    };

    if (features.testFlag(WidgetCalculator::kEmpty)) {
      btn->setFocusPolicy(Qt::NoFocus);
      btn->setEnabled(false);
    }

    if (features.testFlag(WidgetCalculator::kBold)) {
      btn->setStyleSheet("QPushButton  { font-weight: bold; }");
    }

    if (features.testFlag(WidgetCalculator::kColorWhite)) {
      set_color(Qt::white);
    } else if (features.testFlag(WidgetCalculator::kColorGray)) {
      set_color(Qt::gray);
    }

    connected = QObject::connect(btn, &QPushButton::pressed, this, [=, this]() {
      emit SignalKeyPressed(btn->text());
    });
    IS_CONENCTED_OK

    q_grid_widget->addWidget(btn, row, column);
  };

  /// 1 buttons row
  add_calc_btn("A", 1, 0,
               WidgetCalculator::kColorWhite | WidgetCalculator::kBold);
  add_calc_btn("B", 1, 1,
               WidgetCalculator::kColorWhite | WidgetCalculator::kBold);
  add_calc_btn("7", 1, 2,
               WidgetCalculator::kColorWhite | WidgetCalculator::kBold);
  add_calc_btn("8", 1, 3,
               WidgetCalculator::kColorWhite | WidgetCalculator::kBold);
  add_calc_btn("9", 1, 4,
               WidgetCalculator::kColorWhite | WidgetCalculator::kBold);
  add_calc_btn(kDivision, 1, 5, WidgetCalculator::kColorGray);

  /// 2 buttons row
  add_calc_btn("C", 2, 0,
               WidgetCalculator::kColorWhite | WidgetCalculator::kBold);
  add_calc_btn("D", 2, 1,
               WidgetCalculator::kColorWhite | WidgetCalculator::kBold);
  add_calc_btn("4", 2, 2,
               WidgetCalculator::kColorWhite | WidgetCalculator::kBold);
  add_calc_btn("5", 2, 3,
               WidgetCalculator::kColorWhite | WidgetCalculator::kBold);
  add_calc_btn("6", 2, 4,
               WidgetCalculator::kColorWhite | WidgetCalculator::kBold);
  add_calc_btn(kMultiply, 2, 5, WidgetCalculator::kColorGray);

  /// 3 buttons row
  add_calc_btn("E", 3, 0,
               WidgetCalculator::kColorWhite | WidgetCalculator::kBold);
  add_calc_btn("F", 3, 1,
               WidgetCalculator::kColorWhite | WidgetCalculator::kBold);
  add_calc_btn("1", 3, 2,
               WidgetCalculator::kColorWhite | WidgetCalculator::kBold);
  add_calc_btn("2", 3, 3,
               WidgetCalculator::kColorWhite | WidgetCalculator::kBold);
  add_calc_btn("3", 3, 4,
               WidgetCalculator::kColorWhite | WidgetCalculator::kBold);
  add_calc_btn(kMinus, 3, 5, WidgetCalculator::kColorGray);

  /// 4 buttons row
  add_calc_btn(kSign, 4, 0,
               WidgetCalculator::kColorGray | WidgetCalculator::kBold);
  add_calc_btn("", 4, 1, WidgetCalculator::kEmpty);
  add_calc_btn("0", 4, 2,
               WidgetCalculator::kColorWhite | WidgetCalculator::kBold);
  add_calc_btn(kDot, 4, 3,
               WidgetCalculator::kColorGray | WidgetCalculator::kBold);
  add_calc_btn(kEqual, 4, 4, WidgetCalculator::kColorGray);
  add_calc_btn(kPlus, 4, 5, WidgetCalculator::kColorGray);

  connected = QObject::connect(this, &WidgetCalculator::SignalKeyPressed, this,
                               &WidgetCalculator::ProcessKeyPressed);
  IS_CONENCTED_OK
};

void WidgetCalculator::ProcessKeyPressed(const QString& str_btn) {
  // qDebug() << "WidgetCalculator::ProcessKeyPressed " << str_btn;

  auto is_operation = [](const QString& btn_value) {
    if (btn_value == kPlus || btn_value == kMultiply ||
        btn_value == kDivision || btn_value == kEqual || btn_value == kMinus)
      return true;
    else
      return false;
  };

  if (str_btn == kSign) {
    QString string_number(line_edit->text());
    if (string_number.isEmpty() == false) {
      if (string_number[0] == kMinus) {
        string_number.remove(kMinus);
        line_edit->setText(string_number);
      }else{
        string_number.prepend(kMinus);
        line_edit->setText(string_number);
      }
    }
  }
  else if (is_operation(str_btn)) {
    IntX result;
    IntX rhs = IntXFromString(line_edit->text());
    if (buffer_value.size() != 0) {
      IntX lhs = IntXFromString(buffer_value);

      if (buffer_operation == kEqual) {
        result = rhs;
      } else if (buffer_operation == kPlus) {
        result = lhs + rhs;
      } else if (buffer_operation == kMinus) {
        result = lhs - rhs;
      } else if (buffer_operation == kMultiply) {
        result = lhs * rhs;
      } else if (buffer_operation == kDivision) {
          result = lhs / rhs;
      }
    } else {
      result = rhs;
    }

    buffer_value = IntXToString(result);
    if (buffer_value.size() != 0) {
      line_edit->setText(buffer_value);
    }

    buffer_operation = str_btn;

    qDebug() << "buffer_value = " << buffer_value;
    qDebug() << "buffer_operation = " << buffer_operation;

  } else if (str_btn == kDelete) {
    QString current_string(line_edit->text());
    current_string.remove(current_string.size() - 1, 1);
    line_edit->setText(current_string);
  } else if (line_edit->text().size() == 0 &&
             (str_btn == kDot)) {
    line_edit->setText(str_btn);
  } else {
    if (is_operation(previous_pressed_btn)) {
      line_edit->setText("");
    }
    QString possible_string(line_edit->text());
    possible_string.append(str_btn);
    QRegExp rx("[-]?([0-9A-F]{1,24}([,][0-9A-F]{0,8})?|[,][0-9A-F]{0,8})");
    QRegExpValidator validator(rx, this);
    int pos;
    QValidator::State result = validator.validate(possible_string, pos);

    if (result == QValidator::Acceptable) line_edit->setText(possible_string);
  }
  previous_pressed_btn = str_btn;
}

void WidgetCalculator::keyPressEvent(QKeyEvent* event) {
  quint32 code = event->nativeScanCode();
  QString btn_pressed_keyboard;
  if (code == 22) {
    btn_pressed_keyboard = "<-";
  } else if (code == 10 || code == 87) {
    btn_pressed_keyboard = "1";
  } else if (code == 11 || code == 88) {
    btn_pressed_keyboard = "2";
  } else if (code == 12 || code == 89) {
    btn_pressed_keyboard = "3";
  } else if (code == 13 || code == 83) {
    btn_pressed_keyboard = "4";
  } else if (code == 14 || code == 84) {
    btn_pressed_keyboard = "5";
  } else if (code == 15 || code == 85) {
    btn_pressed_keyboard = "6";
  } else if (code == 16 || code == 79) {
    btn_pressed_keyboard = "7";
  } else if (code == 17 || code == 80) {
    btn_pressed_keyboard = "8";
  } else if (code == 18 || code == 81) {
    btn_pressed_keyboard = "9";
  } else if (code == 19 || code == 90) {
    btn_pressed_keyboard = "0";
  } else if (code == 86) {
    btn_pressed_keyboard = kPlus;
  } else if (code == 108 || code == 21 || code == 36) {
    btn_pressed_keyboard = kEqual;
  } else if (code == 112 || code == 61 || code == 51) {
    btn_pressed_keyboard = kDivision;
  } else if (code == 20 || code == 82) {
    btn_pressed_keyboard = kMinus;
  } else if (code == 63) {
    btn_pressed_keyboard = kMultiply;
  } else if (code == 91 || code == 59 || code == 60) {
    btn_pressed_keyboard = kDot;
  } else if (code == 38) {
    btn_pressed_keyboard = "A";
  } else if (code == 56) {
    btn_pressed_keyboard = "B";
  } else if (code == 54) {
    btn_pressed_keyboard = "C";
  } else if (code == 40) {
    btn_pressed_keyboard = "D";
  } else if (code == 26) {
    btn_pressed_keyboard = "E";
  } else if (code == 41) {
    btn_pressed_keyboard = "F";
  }
  emit SignalKeyPressed(btn_pressed_keyboard);
}