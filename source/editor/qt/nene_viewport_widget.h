/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <QWidget>
#include <QtCore/QTimer>

class NeneViewportWidget : public QWidget
{
	Q_OBJECT
	
public:
	NeneViewportWidget(QWidget *parent);
	~NeneViewportWidget() override;

protected:
	bool event(QEvent *e) override;
	void showEvent(QShowEvent *event) override;
	void paintEvent(QPaintEvent* event) override;
	void resizeEvent(QResizeEvent* event) override;
	bool nativeEvent(const QByteArray& eventType, void* message, qintptr* result) override;
	//
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void keyPressEvent(QKeyEvent* event) override;
	void keyReleaseEvent(QKeyEvent* event) override;

	QPaintEngine* paintEngine() const override;
	
private:
	QTimer m_engine_tick_timer;
	QPoint m_last_mouse_position;
	bool m_b_mouse_ever_moved = false;
};