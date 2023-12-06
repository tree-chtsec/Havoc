#ifndef HAVOCCLIENT_HELPER_H
#define HAVOCCLIENT_HELPER_H

#include <Common.h>

namespace Helper {
    auto MessageBox(
        QMessageBox::Icon Icon,
        std::string title,
        std::string text
    ) -> void;

    auto FileRead(
        const QString& FilePath
    ) -> QByteArray;

    auto GrayScaleIcon(
        QImage image
    ) -> QIcon;
};

#endif //HAVOCCLIENT_HELPER_H
