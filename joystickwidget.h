#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <QWidget>

class QResizeEvent;
class QMouseEvent;
class QPaintEvent;
class QPropertyAnimation;

class JoystickWidget : public QWidget {
  Q_OBJECT
  Q_PROPERTY(QPointF position READ getPosition WRITE setPosition NOTIFY positionChanged FINAL)
public:
  explicit JoystickWidget(QWidget* parent = nullptr);

  void animateLeft();
  void animateRight();
  void animateUp();
  void animateDown();

  void animate();
  void animate(double endX, double endY);

  virtual void resizeEvent(QResizeEvent* event) override;
  virtual void mouseReleaseEvent(QMouseEvent* event) override;
  virtual void mouseMoveEvent(QMouseEvent* event) override;
  virtual void keyPressEvent(QKeyEvent* event) override;
  virtual void paintEvent(QPaintEvent* event) override;

  const QPointF& getPosition() { return m_position; }
  void setPosition(const QPointF& position) {
    m_position = position;
    emit positionChanged();
  }

signals:
  void positionChanged();

private:
  QPropertyAnimation* m_positionAnimation;
  QPointF m_position;
  double m_stickRadius;
};

#endif  // JOYSTICK_H
