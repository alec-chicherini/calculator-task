#pragma once
#include <QDebug>
#include <QMessageBox>
#include <QString>
#include <intx/intx.hpp>

const QString kMinus("-");
const QString kPlus("+");
const QString kMultiply("×");
const QString kDivision("÷");
const QString kEqual("=");
const QString kDot(",");
const QString kDelete("<-");
const QString kSign("+/-");

struct IntX {
  intx::uint128 value;
  bool is_negative{false};
  int after_dot{0};
};

void ShowMessageBoxOutOfRange() {
  QMessageBox msgBox;
  msgBox.setText("Ошибка. Результат операции вне uint128.");
  msgBox.exec();
}

IntX operator+(const IntX& lhs, const IntX& rhs) noexcept {
  IntX result;
  auto chek_if_more_than_128 = [&] {
    if (result.value < lhs.value) {
      result.value = intx::uint128{0};
      result.is_negative = false;
      result.after_dot = 0;
      ShowMessageBoxOutOfRange();
    }
  };
  if (lhs.is_negative && rhs.is_negative) {
    result.value = lhs.value + rhs.value;
    result.is_negative = true;
    chek_if_more_than_128();
  } else if ((lhs.is_negative == false) && (rhs.is_negative == false)) {
    result.value = lhs.value + rhs.value;
    chek_if_more_than_128();
  } else if ((lhs.is_negative) && (rhs.is_negative == false)) {
    result.value = rhs.value - lhs.value;
  } else if ((lhs.is_negative == false) && (rhs.is_negative)) {
    result.value = lhs.value - rhs.value;
  }
  result.after_dot = std::max(lhs.after_dot, rhs.after_dot);
  return result;
}

IntX operator-(const IntX& lhs, const IntX& rhs) noexcept {
  IntX result;
  if (lhs.is_negative && rhs.is_negative) {
    auto res_ = rhs - lhs;
    result.value = res_.value;
  } else if ((lhs.is_negative == false) && (rhs.is_negative == false)) {
    if (lhs.value >= rhs.value) {
      result.value = lhs.value - rhs.value;
    } else {
      result.value = rhs.value - lhs.value;
      result.is_negative = true;
    }
  } else if ((lhs.is_negative) && (rhs.is_negative == false)) {
    auto res_ = lhs + rhs;
    result.value = res_.value;
    result.is_negative = true;
  } else if ((lhs.is_negative == false) && (rhs.is_negative)) {
    auto res_ = lhs + rhs;
    result.value = res_.value;
  }
  result.after_dot = std::max(lhs.after_dot, rhs.after_dot);
  return result;
}

IntX operator*(const IntX& lhs, const IntX& rhs) noexcept {
  IntX result;
  auto chek_if_more_than_128 = [&] {
    if (result.value < lhs.value) {
      result.value = intx::uint128{0};
      result.is_negative = false;
      result.after_dot = 0;
      ShowMessageBoxOutOfRange();
    }
  };

  if (lhs.is_negative && rhs.is_negative) {
    result.is_negative = false;
  } else if ((lhs.is_negative == false) && (rhs.is_negative == false)) {
    result.is_negative = false;
  } else if ((lhs.is_negative) && (rhs.is_negative == false)) {
    result.is_negative = true;
  } else if ((lhs.is_negative == false) && (rhs.is_negative)) {
    result.is_negative = true;
  }
  result.after_dot = lhs.after_dot + rhs.after_dot;
  result.value = lhs.value * rhs.value;
  chek_if_more_than_128();
  return result;
}

IntX operator/(const IntX& lhs, const IntX& rhs) noexcept {
  IntX result;
  if (rhs.value != intx::uint128{0}) {
    result.value = lhs.value / rhs.value;
  } else {
    QMessageBox msgBox;
    msgBox.setText("Ошибка.Деление на ноль.");
    msgBox.exec();
    result.value = intx::uint128{0};
    result.is_negative = false;
  }
  return result;
}

static IntX IntXFromString(QString number) {
  IntX result;
  qDebug() << "IntXFromString number " << number;

  if ((number.isEmpty() == false) && (number[0] == kMinus)) {
    result.is_negative = true;
    number.remove(kMinus);
  } else {
    result.is_negative = false;
  }

  if (number.contains(kDot) == false) {
    result.after_dot = 0;
  } else {
    auto index_of_dot = number.indexOf(kDot);
    QString right = number.mid(index_of_dot + 1);
    result.after_dot = right.size();
    number.remove(kDot);
  }

  number.prepend("0x");

  qDebug() << "IntXFromString number " << number;
  result.value = intx::from_string<intx::uint128>(number.toStdString());
  return result;
};

static QString IntXToString(IntX int_x) {
  QString result;
  std::string string_from_intx = intx::hex<128>(int_x.value);
  result = QString::fromStdString(string_from_intx);
  qDebug() << "IntXToString string_from_intx " << result;

  if (result.size() < int_x.after_dot) {
    result = QString("%1%2")
                 .arg(QString().fill('0', int_x.after_dot - result.size()))
                 .arg(result);
  }

  result.insert(result.size() - int_x.after_dot, kDot);
  if (*result.begin() == kDot) {
    result.prepend("0");
  }
  // while (*result.rbegin() == '0') {
  //   result.chop(1);
  // }
  if (*result.rbegin() == ',') {
    result.chop(1);
  }
  // if (result.isEmpty()) {
  //   result = "0";
  // }

  if (int_x.is_negative) {
    result.prepend(kMinus);
  }

  return result.toUpper();
};