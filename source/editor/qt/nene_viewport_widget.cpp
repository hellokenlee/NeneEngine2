/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "nene_viewport_widget.h"
#include "engine/engine_loop.h"

#include <QtGui/QWindow>


bool QNeneViewportWidget::event(QEvent* e)
{
	return QWidget::event(e);
}

void QNeneViewportWidget::showEvent(QShowEvent* event)
{
	if (!engine_loop::is_initialized())
	{
		engine_loop::initialize(reinterpret_cast<void*>(winId()));
	}
	
	QWidget::showEvent(event);
}

void QNeneViewportWidget::paintEvent(QPaintEvent* event)
{
	// do not paint with qt
}

void QNeneViewportWidget::resizeEvent(QResizeEvent* event)
{
	// TODO: tell engine to resize the swapchain
	QWidget::resizeEvent(event);
}

bool QNeneViewportWidget::nativeEvent(const QByteArray& eventType, void* message, qintptr* result)
{
	return QWidget::nativeEvent(eventType, message, result);
}

QPaintEngine* QNeneViewportWidget::paintEngine() const
{
	// make sure we do not want qt to paint for use
	return nullptr;
}
