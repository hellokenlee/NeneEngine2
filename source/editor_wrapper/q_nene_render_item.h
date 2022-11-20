#pragma once

#include "QtQuick/QQuickItem"

class CustomRenderItem : public QQuickItem
{
public:
    Q_OBJECT
        QML_ELEMENT

public:
    CustomRenderItem(QQuickItem* parent = nullptr);
    QSGNode* updatePaintNode(QSGNode* node, UpdatePaintNodeData*) override;
};

