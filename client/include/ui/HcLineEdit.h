#ifndef HAVOCCLIENT_HCLINEEDIT_H
#define HAVOCCLIENT_HCLINEEDIT_H

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QtUiPlugin/QDesignerExportWidget>

class HcLineEdit : public QWidget {
    QHBoxLayout* Layout = nullptr;
    QLabel*      Label  = nullptr;
public:
    QLineEdit*   Input  = nullptr;

    explicit HcLineEdit( QWidget* parent = nullptr );
    ~HcLineEdit();

    auto setLabelText(
        const QString& text
    ) -> void;

    auto setInputText(
        const QString& text
    ) -> void;

    auto setValidator(
        const QValidator* val
    ) -> void;

    auto text() -> QString;
};

#endif //HAVOCCLIENT_HCLINEEDIT_H
