#include "joystickwidget.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPropertyAnimation>

JoystickWidget::JoystickWidget(QWidget* parent)
    : QWidget{parent}, m_positionAnimation(new QPropertyAnimation(this, "position")), m_stickRadius(width() * 0.10) {
  m_positionAnimation->setDuration(500);
  m_positionAnimation->setEasingCurve(QEasingCurve::OutQuad);

  connect(this, &JoystickWidget::positionChanged, this, qOverload<>(&JoystickWidget::update));

  // Needed for being able to receive keyPressEvents properly
  setFocusPolicy(Qt::FocusPolicy::StrongFocus);
}

void JoystickWidget::animateLeft() {
  animate(m_stickRadius * 2, height() / 2);
}

void JoystickWidget::animateRight() {
  animate(width() - m_stickRadius * 2, height() / 2);
}

void JoystickWidget::animateUp() {
  animate(width() / 2, m_stickRadius * 2);
}

void JoystickWidget::animateDown() {
  animate(width() / 2, height() - m_stickRadius * 2);
}

void JoystickWidget::animate() {
  m_positionAnimation->setKeyValues({});
  m_positionAnimation->setEndValue(rect().center());
  m_positionAnimation->start();
}

void JoystickWidget::animate(double endX, double endY) {
  m_positionAnimation->setKeyValues({});
  m_positionAnimation->setKeyValueAt(0.5, QPoint(endX, endY));
  m_positionAnimation->setEndValue(rect().center());
  m_positionAnimation->start();
}

void JoystickWidget::resizeEvent(QResizeEvent* event) {
  m_position = rect().center();
  m_positionAnimation->setEndValue(m_position);
  m_stickRadius = width() * 0.10;  // Set knob radius to be 10% of widget width
}

void JoystickWidget::mouseReleaseEvent(QMouseEvent* event) {
  animate();
}

void JoystickWidget::mouseMoveEvent(QMouseEvent* event) {
  m_position = event->pos();

  const QPointF deltaPos = m_position - rect().center();

  const double len = std::sqrt(std::pow(deltaPos.x(), 2) + std::pow(deltaPos.y(), 2));

  const double maxLen = (width() / 2) - m_stickRadius * 2;
  if (len > maxLen) {
    const double angle = std::atan2(deltaPos.y(), deltaPos.x());
    const double x = std::cos(angle) * maxLen;
    const double y = std::sin(angle) * maxLen;
    m_position = rect().center() + QPointF(x, y);
  }

  update();
}

void JoystickWidget::keyPressEvent(QKeyEvent* event) {
  if (event->key() == Qt::Key_Up) {
    animateUp();
  } else if (event->key() == Qt::Key_Down) {
    animateDown();
  } else if (event->key() == Qt::Key_Right) {
    animateRight();
  } else if (event->key() == Qt::Key_Left) {
    animateLeft();
  }
}

void JoystickWidget::paintEvent(QPaintEvent* event) {
  QPainter painter(this);

  QRadialGradient gradient(rect().center(), width() / 2);
  gradient.setColorAt(0, Qt::white);
  gradient.setColorAt(1.0, Qt::lightGray);
  painter.setBrush(gradient);
  painter.drawEllipse(rect());

  gradient = QRadialGradient(m_position, m_stickRadius);
  gradient.setColorAt(0, Qt::darkGray);
  gradient.setColorAt(1.0, Qt::lightGray);
  painter.setBrush(gradient);
  painter.drawEllipse(m_position, m_stickRadius * 2, m_stickRadius * 2);
}
