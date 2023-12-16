#ifndef HAVOCCLIENT_HCWIDGET_H
#define HAVOCCLIENT_HCWIDGET_H

#include <Common.h>

class HcWidget : public QWidget {
    explicit HcWidget();
    ~HcWidget();

    auto addWidget(
            HcWidget* widget
    ) -> void;

    auto addWidget(
        const json& widget
    ) -> void;

    auto addLabel(
        const json& widget
    ) -> void;
};

#endif //HAVOCCLIENT_HCWIDGET_H
