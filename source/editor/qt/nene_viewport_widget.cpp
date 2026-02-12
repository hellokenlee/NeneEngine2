/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "nene_viewport_widget.h"

#include <QtGui/qevent.h>
#include <QtGui/QWindow>
#include "engine/engine_loop.h"


constexpr int EDITOR_FRAME_PER_SECOND    = 60.0f;
constexpr int EDITOR_MILLISECOND_PER_FRAME = static_cast<int>((1.0f / EDITOR_FRAME_PER_SECOND) * 1000.0f);

NeneViewportWidget::NeneViewportWidget(QWidget* parent)
	: QWidget(parent)
{
	// 
	setPalette(Qt::blue);
    setAutoFillBackground(true);

	// tell qt crate a native window such that `winId()` is valid
	setAttribute(Qt::WA_NativeWindow);
	// tell qt we will paint directly on screen and don't use `QBackingStore` for this widget
	setAttribute(Qt::WA_PaintOnScreen);
	// tell qt don't fill the background color for us
	setAttribute(Qt::WA_NoSystemBackground);
	setAttribute(Qt::WA_OpaquePaintEvent);
}

NeneViewportWidget::~NeneViewportWidget() = default;

bool NeneViewportWidget::event(QEvent* e)
{
	return QWidget::event(e);
}

void NeneViewportWidget::showEvent(QShowEvent* event)
{
	if (!nene::engine_loop::is_initialized())
	{
		nene::engine_loop::initialize(reinterpret_cast<void*>(winId()), uint2(static_cast<uint32_t>(size().width()), static_cast<uint32_t>(size().height())));
		connect(&m_engine_tick_timer, &QTimer::timeout, &nene::engine_loop::tick);
		m_engine_tick_timer.start(EDITOR_MILLISECOND_PER_FRAME);
	}
	
	QWidget::showEvent(event);
}

void NeneViewportWidget::paintEvent(QPaintEvent* event)
{
	// do not paint with qt
}

QPaintEngine* NeneViewportWidget::paintEngine() const
{
	// make sure we do not want qt to paint for us
	return nullptr;
}

void NeneViewportWidget::resizeEvent(QResizeEvent* event)
{
	//
	QWidget::resizeEvent(event);
	// tell engine to resize the swap chain
	if (nene::engine_loop::is_initialized())
	{
		nene::engine_loop::resize(uint2(static_cast<uint32_t>(event->size().width()), static_cast<uint32_t>(event->size().height())));
	}
}

bool NeneViewportWidget::nativeEvent(const QByteArray& eventType, void* message, qintptr* result)
{
	return QWidget::nativeEvent(eventType, message, result);
}
