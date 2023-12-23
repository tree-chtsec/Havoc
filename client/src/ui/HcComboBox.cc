#include <ui/HcComboBox.h>

HcComboBox::HcComboBox( QWidget* parent ) : QWidget( parent ) {
    Layout = new QHBoxLayout( this );
    Layout->setSpacing( 0 );
    Layout->setContentsMargins( 0, 0, 0, 0 );

    Label = new QLabel;
    Label->setProperty( "HcLineEdit", "true" );

    Combo = new QComboBox;
    Combo->setProperty( "HcLineEdit", "true" );

    Layout->addWidget( Label );
    Layout->addWidget( Combo );

    setLayout( Layout );
}

HcComboBox::~HcComboBox() = default;

auto HcComboBox::setLabelText(
    const QString& text
) -> void {
    Label->setText( text );
    Layout->setStretch( 1, text.size() );
    Layout->setStretch( 2, 100 - text.size() );
}
