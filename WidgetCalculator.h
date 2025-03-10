#pragma once
#include <QFlags>
#include <QObject>
#include <QWidget>

class WidgetCalculator : public QWidget {
  Q_OBJECT

 public:
  WidgetCalculator();

  enum ButtonFeature {
    kColorWhite = 0b1,
    kColorGray = 0b01,
    kColorNone = 0b001,
    kBold = 0b0001,
    kEmpty = 0b00001,
    kNumber = 0b000001,
    kOperator = 0b0000001
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
};
Q_DECLARE_OPERATORS_FOR_FLAGS(WidgetCalculator::ButtonFeatures);
