/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "QtQuick/QQuickItem"

class CustomRenderItem : public QQuickItem
{
    Q_OBJECT

public:
    static void register_qml();
    
    explicit CustomRenderItem(QQuickItem* parent = nullptr);

    QSGNode* updatePaintNode(QSGNode* node, UpdatePaintNodeData*) override;

protected:
    bool is_gapi_inited = false; 
};

