#ifndef HAVOCCLIENT_HELPER_H
#define HAVOCCLIENT_HELPER_H

#include <Common.h>

auto HavocMessageBox(
    QMessageBox::Icon Icon,
    std::string title,
    std::string text
) -> void;

auto HavocFileRead(
    const QString& FilePath
) -> QByteArray;

auto GrayScaleIcon(
    QImage image
) -> QIcon;

#endif //HAVOCCLIENT_HELPER_H
