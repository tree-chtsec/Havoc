#include <core/Helper.h>

auto HavocMessageBox(
    QMessageBox::Icon icon,
    std::string       title,
    std::string       text
) -> void {
    auto box = QMessageBox();

    box.setWindowTitle( title.c_str() );
    box.setText( text.c_str() );
    box.setIcon( icon );
    box.exec();
}


