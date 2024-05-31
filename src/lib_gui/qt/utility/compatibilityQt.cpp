#include "compatibilityQt.h"

#include <QtGlobal>

namespace utility
{
namespace compatibility
{
QPoint QWheelEvent_globalPos(const QWheelEvent& event)
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
	return event.globalPosition().toPoint();
#else
	return event.globalPos();
#endif
}

QPoint QMouseEvent_globalPos(const QMouseEvent* event)
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
	return event->globalPosition().toPoint();
#else
	return event->globalPos();
#endif
}

int QMouseEvent_x(const QMouseEvent* event)
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
	return qRound(event->position().x());
#else
	return event->x();
#endif
}
}	 // namespace compatibility
}	 // namespace utility
