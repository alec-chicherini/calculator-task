#include <QtHelper.h>
#include <WidgetCalculator.h>

#include <QDebug>
#include <QGridLayout>
#include <QLineEdit>

WidgetCalculator::WidgetCalculator() {
  [[maybe_unused]] bool connected;

  QGridLayout* q_vbox_layout = new QGridLayout(this);
  q_vbox_layout->setGeometry(QRect());

  line_edit = new QLineEdit();
  q_vbox_layout->AddWidget(line_edit, 0, 0, 1, 6);

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
      set_color(QColor::white);
    } else if (features.testFlag(WidgetCalculator::kColorGray)) {
      set_color(QColor::gray);
    }

    connected = QObject::connect(btn, &QPushButton::pressed, this, [=, this]() {
      emit SignalKeyPressed(btn->text());
    });
    IS_CONENCTED_OK

    q_vbox_layout->AddWidget(btn, row, column);
  };
  using enum ButtonFeature;
  /// 1 buttons row
  add_calc_btn("A", 1, 0, kColorWhite | kNumber | kBold);
  add_calc_btn("B", 1, 1, kColorWhite | kNumber | kBold);
  add_calc_btn("7", 1, 2, kColorWhite | kNumber | kBold);
  add_calc_btn("8", 1, 3, kColorWhite | kNumber | kBold);
  add_calc_btn("9", 1, 4, kColorWhite | kNumber | kBold);
  add_calc_btn("÷", 1, 5, kColorGray | kOperator);

  /// 2 buttons row
  add_calc_btn("C", 2, 0, kColorWhite | kNumber | kBold);
  add_calc_btn("D", 2, 1, kColorWhite | kNumber | kBold);
  add_calc_btn("4", 2, 2, kColorWhite | kNumber | kBold);
  add_calc_btn("5", 2, 3, kColorWhite | kNumber | kBold);
  add_calc_btn("6", 2, 4, kColorWhite | kNumber | kBold);
  add_calc_btn("×", 2, 5, kColorGray | kOperator);

  /// 3 buttons row
  add_calc_btn("E", 2, 0, kColorWhite | kNumber | kBold););
  add_calc_btn("F", 2, 1, kColorWhite | kNumber | kBold););
  add_calc_btn("1", 2, 2, kColorWhite | kNumber | kBold););
  add_calc_btn("2", 2, 3, kColorWhite | kNumber | kBold););
  add_calc_btn("3", 2, 4, kColorWhite | kNumber | kBold););
  add_calc_btn("-", 2, 5, kColorGray | kOperator);

  /// 4 buttons row
  add_calc_btn("", 2, 0, kEmpty);
  add_calc_btn("", 2, 1, kEmpty);
  add_calc_btn("0", 2, 2, kColorWhite | kNumber | kBold););
  add_calc_btn(",", 2, 3, kColorGray | kOperator | kBold);
  add_calc_btn("=", 2, 4, kColorGray | kOperator);
  add_calc_btn("+", 2, 5, kColorGray | kOperator);

  connected = QObject::connect(this, &WidgetCalculator::SignalKeyPressed, this,
                               &WidgetCalculator::ProcessKeyPressed);
  IS_CONENCTED_OK
};

void WidgetCalculator::ProcessKeyPressed(const QString& str_btn) {
  qDebug() << "WidgetCalculator::ProcessKeyPressed " << str_btn;
}

void WidgetCalculator::keyPressEvent(QKeyEvent* event) {
  quint32 code = event->nativeScanCode();
  qDebug() << "WidgetCalculator::keyPressEvent " << code;
  QString btn_pressed_keyboard;
  if (code == 36) {
    btn_pressed_keyboard = "Enter";
  } else if (code == 22) {
    btn_pressed_keyboard = "<-";
  }
  emit SignalKeyPressed(btn_pressed_keyboard);
}