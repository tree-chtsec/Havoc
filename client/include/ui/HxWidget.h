#ifndef HAVOCCLIENT_HXWIDGET_H
#define HAVOCCLIENT_HXWIDGET_H

#include <Common.h>

class HxWidget : public QWidget {
    explicit HxWidget();
    ~HxWidget();

    auto addWidget(
        HxWidget* widget
    ) -> void;

    auto addWidget(
        const json& widget
    ) -> void;

    auto addLabel(
        const json& widget
    ) -> void;
};

#endif //HAVOCCLIENT_HXWIDGET_H
