#pragma once
#include <QFlags>
#include <QObject>
#include <QWidget>
#include <QLineEdit>
#include <QKeyEvent>
#include <QPushButton>
#include <QColor>

class WidgetCalculator : public QWidget {
  Q_OBJECT

 public:
  WidgetCalculator();

  enum ButtonFeature {
    kColorWhite = 0b1,
    kColorGray =  0b10,
    kColorNone =  0b100,
    kBold =       0b1000,
    kEmpty =      0b10000
  };
  Q_DECLARE_FLAGS(ButtonFeatures, ButtonFeature);

 signals:
  void SignalKeyPressed(const QString&);

 private slots:
  void ProcessKeyPressed(const QString&);

 protected:
  virtual void keyPressEvent(QKeyEvent* event) override;

 private:
  QLineEdit* line_edit;
  QString buffer_value;
  QString buffer_operation;
  QString previous_pressed_btn;
};
Q_DECLARE_OPERATORS_FOR_FLAGS(WidgetCalculator::ButtonFeatures);
