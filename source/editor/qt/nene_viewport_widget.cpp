/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "nene_viewport_widget.h"

#include <QtGui/qevent.h>
#include <QtGui/QWindow>
#include "engine/engine_loop.h"
#include "engine/input_manager.h"


constexpr int EDITOR_FRAME_PER_SECOND	= 60.0f;
constexpr int EDITOR_MILLISECOND_PER_FRAME = static_cast<int>((1.0f / EDITOR_FRAME_PER_SECOND) * 1000.0f);

NeneViewportWidget::NeneViewportWidget(QWidget* parent)
	: QWidget(parent)
{
	// 
	setPalette(Qt::blue);
	setAutoFillBackground(true);
	
	// tell qt to receive keyboard event
	setFocusPolicy(Qt::StrongFocus);

	// tell qt crate a native window such that `winId()` is valid
	setAttribute(Qt::WA_NativeWindow);
	// tell qt we will paint directly on screen and don't use `QBackingStore` for this widget
	setAttribute(Qt::WA_PaintOnScreen);
	// tell qt don't fill the background color for us
	setAttribute(Qt::WA_NoSystemBackground);
	setAttribute(Qt::WA_OpaquePaintEvent);
	
	//
	if (!nene::engine_loop::is_initialized())
	{
		nene::engine_loop::initialize(reinterpret_cast<void*>(winId()), nene::uint2(static_cast<uint32_t>(size().width()), static_cast<uint32_t>(size().height())));  // NOLINT(performance-no-int-to-ptr)
		connect(&m_engine_tick_timer, &QTimer::timeout, &nene::engine_loop::tick);
		m_engine_tick_timer.start(EDITOR_MILLISECOND_PER_FRAME);
	}
}

NeneViewportWidget::~NeneViewportWidget() = default;

bool NeneViewportWidget::event(QEvent* e)
{
	return QWidget::event(e);
}

void NeneViewportWidget::showEvent(QShowEvent* event)
{
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
	nene::engine_loop::resize(nene::uint2(static_cast<uint32_t>(event->size().width()), static_cast<uint32_t>(event->size().height())));
}

bool NeneViewportWidget::nativeEvent(const QByteArray& eventType, void* message, qintptr* result)
{
	return QWidget::nativeEvent(eventType, message, result);
}

static void notifyNeneMouseEvent(const QMouseEvent& event, nene::mouse_event_type mtype, nene::key_event_type button_type, bool& b_inout_mouse_ever_moved, QPoint& inout_last_mouse_position)
{
	nene::mouse_event mouse_event;
	mouse_event.m_type = mtype;
	switch (event.button())
	{
		case Qt::LeftButton:
			mouse_event.m_button_index = nene::mouse_event::MOUSE_BUTTON_LEFT;
			break;
		case Qt::RightButton:
			mouse_event.m_button_index = nene::mouse_event::MOUSE_BUTTON_RIGHT;
			break;
		case Qt::MiddleButton:
			mouse_event.m_button_index = nene::mouse_event::MOUSE_BUTTON_MIDDLE;
			break;
		case Qt::BackButton:
			mouse_event.m_button_index = nene::mouse_event::MOUSE_BUTTON_SIDE_BACK;
			break;
		case Qt::ForwardButton:
			mouse_event.m_button_index = nene::mouse_event::MOUSE_BUTTON_SIDE_FRONT;
			break;
		default:
			mouse_event.m_button_index = 0;
	}
	if (!b_inout_mouse_ever_moved)
	{
		b_inout_mouse_ever_moved = true;
		inout_last_mouse_position = event.pos();
	}
	auto delta = (event.pos() - inout_last_mouse_position).toPointF();
	inout_last_mouse_position = event.pos();
	mouse_event.m_delta_motion = float2(static_cast<float>(delta.x()), static_cast<float>(delta.y()));
	mouse_event.m_button_type = button_type;
	// TODO
	mouse_event.m_position_in_window = float2::zero();
	nene::input_manager::get().notify(mouse_event);
}

static void notifyNeneKeyEvent(const QKeyEvent& event, nene::key_event_type type)
{
	nene::keyboard_event keyboard_event;
	keyboard_event.m_type = type;
	
	// Convert Qt key to char
	if (event.key() >= Qt::Key_A && event.key() <= Qt::Key_Z)
	{
		// Convert to lowercase
		keyboard_event.m_key = static_cast<char>('a' + (event.key() - Qt::Key_A));
	}
	else if (event.key() >= Qt::Key_0 && event.key() <= Qt::Key_9)
	{
		keyboard_event.m_key = static_cast<char>('0' + (event.key() - Qt::Key_0));
	}
	else
	{
		keyboard_event.m_key = static_cast<char>(event.key());
	}
	
	// Convert Qt modifiers to key_modifier
	nene::key_modifier mod = nene::key_modifier::none;
	if (event.modifiers() & Qt::ControlModifier)
	{
		mod |= nene::key_modifier::ctrl;
	}
	if (event.modifiers() & Qt::AltModifier)
	{
		mod |= nene::key_modifier::alt;
	}
	if (event.modifiers() & Qt::ShiftModifier)
	{
		mod |= nene::key_modifier::shift;
	}
	keyboard_event.m_modifier = mod;
	
	nene::input_manager::get().notify(keyboard_event);
}

void NeneViewportWidget::mousePressEvent(QMouseEvent* event)
{
	notifyNeneMouseEvent(*event, nene::mouse_event_type::on_mouse_button, nene::key_event_type::on_key_press, m_b_mouse_ever_moved, m_last_mouse_position);
}

void NeneViewportWidget::mouseReleaseEvent(QMouseEvent* event)
{
	notifyNeneMouseEvent(*event, nene::mouse_event_type::on_mouse_button, nene::key_event_type::on_key_release, m_b_mouse_ever_moved, m_last_mouse_position);
}

void NeneViewportWidget::mouseMoveEvent(QMouseEvent* event)
{
	notifyNeneMouseEvent(*event, nene::mouse_event_type::on_mouse_move, nene::key_event_type::none, m_b_mouse_ever_moved, m_last_mouse_position);
}

void NeneViewportWidget::keyPressEvent(QKeyEvent* event)
{
	notifyNeneKeyEvent(*event, nene::key_event_type::on_key_press);
}

void NeneViewportWidget::keyReleaseEvent(QKeyEvent* event)
{
	notifyNeneKeyEvent(*event, nene::key_event_type::on_key_release);
}

void NeneViewportWidget::wheelEvent(QWheelEvent* event)
{
	nene::mouse_event mouse_event;
	mouse_event.m_type = nene::mouse_event_type::on_mouse_wheel;
	mouse_event.m_delta_scroll = event->angleDelta().y();
	// 
	nene::input_manager::get().notify(mouse_event);
}
