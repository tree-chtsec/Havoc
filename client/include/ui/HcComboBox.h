#ifndef HAVOCCLIENT_HCCOMBOBOX_H
#define HAVOCCLIENT_HCCOMBOBOX_H

#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QHBoxLayout>

class HcComboBox : public QWidget {
    QHBoxLayout* Layout = nullptr;
    QLabel*      Label  = nullptr;
public:
    QComboBox*   Combo  = nullptr;

    explicit HcComboBox( QWidget* parent = nullptr );
    ~HcComboBox();

    auto setLabelText(
        const QString& text
    ) -> void;
};


#endif //HAVOCCLIENT_HCCOMBOBOX_H
