#include <ui/PageScript.h>

/********************************************************************************
** Form generated from reading UI file 'PageScriptMWyKlN.ui'
**
** Created by: Qt User Interface Compiler version 5.15.11
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef PAGESCRIPTMWYKLN_H
#define PAGESCRIPTMWYKLN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ScriptPage
{
public:
    QGridLayout *gridLayout;
    QSpacerItem *Spacer;
    QSplitter *Splitter;
    QTableWidget *ScriptTableWidget;
    QTabWidget *ScriptTabWidget;
    QWidget *TabScriptLog;
    QWidget *TabScriptInterpreter;
    QLabel *label;
    QPushButton *ButtonLoadScript;

    void setupUi(QWidget *ScriptPage)
    {
        if (ScriptPage->objectName().isEmpty())
            ScriptPage->setObjectName(QString::fromUtf8("ScriptPage"));
        ScriptPage->resize(1197, 840);
        gridLayout = new QGridLayout(ScriptPage);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        Spacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);


        Splitter = new QSplitter(ScriptPage);
        Splitter->setObjectName(QString::fromUtf8("Splitter"));
        Splitter->setOrientation(Qt::Vertical);
        ScriptTableWidget = new QTableWidget(Splitter);
        if (ScriptTableWidget->columnCount() < 1) {
            ScriptTableWidget->setColumnCount(1);
        }
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        ScriptTableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        ScriptTableWidget->setObjectName(QString::fromUtf8("ScriptTableWidget"));
        Splitter->addWidget(ScriptTableWidget);
        ScriptTableWidget->horizontalHeader()->setStretchLastSection(true);
        ScriptTabWidget = new QTabWidget(Splitter);
        ScriptTabWidget->setObjectName(QString::fromUtf8("ScriptTabWidget"));
        TabScriptLog = new QWidget();
        TabScriptLog->setObjectName(QString::fromUtf8("TabScriptLog"));
        ScriptTabWidget->addTab(TabScriptLog, QString());
        TabScriptInterpreter = new QWidget();
        TabScriptInterpreter->setObjectName(QString::fromUtf8("TabScriptInterpreter"));
        ScriptTabWidget->addTab(TabScriptInterpreter, QString());
        Splitter->addWidget(ScriptTabWidget);


        label = new QLabel(ScriptPage);
        label->setObjectName(QString::fromUtf8("label"));


        ButtonLoadScript = new QPushButton(ScriptPage);
        ButtonLoadScript->setObjectName(QString::fromUtf8("ButtonLoadScript"));



        retranslateUi(ScriptPage);

        ScriptTabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(ScriptPage);
    } // setupUi

    void retranslateUi(QWidget *ScriptPage)
    {
        ScriptPage->setWindowTitle(QCoreApplication::translate("ScriptPage", "Form", nullptr));
        QTableWidgetItem *___qtablewidgetitem = ScriptTableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("ScriptPage", "Path", nullptr));
        ScriptTabWidget->setTabText(ScriptTabWidget->indexOf(TabScriptLog), QCoreApplication::translate("ScriptPage", "Page", nullptr));
        ScriptTabWidget->setTabText(ScriptTabWidget->indexOf(TabScriptInterpreter), QCoreApplication::translate("ScriptPage", "Tab 1", nullptr));
        label->setText(QCoreApplication::translate("ScriptPage", "Scripts loaded: 0", nullptr));
        ButtonLoadScript->setText(QCoreApplication::translate("ScriptPage", "Load Script", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ScriptPage: public Ui_ScriptPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // PAGESCRIPTMWYKLN_H


HavocPageScript::HavocPageScript() {
    if ( objectName().isEmpty() ) {
        setObjectName( "ScriptPage" );
    }

    gridLayout = new QGridLayout( this );
    gridLayout->setObjectName( "gridLayout" );

    Spacer = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );


    Splitter = new QSplitter( this );
    Splitter->setObjectName( "Splitter" );
    Splitter->setOrientation( Qt::Vertical );

    ScriptTableWidget = new QTableWidget( Splitter );
    ScriptTableWidget->setColumnCount( 1 );
    ScriptTableWidget->setHorizontalHeaderItem( 0, new QTableWidgetItem( "Path" ) );
    ScriptTableWidget->setObjectName( "ScriptTableWidget" );
    ScriptTableWidget->horizontalHeader()->setStretchLastSection( true );

    ScriptTabWidget = new QTabWidget( Splitter );
    ScriptTabWidget->setObjectName( "ScriptTabWidget" );
    ScriptTabWidget->addTab( TabScriptInterpreter, "Interpreter" );

    TabScriptInterpreter = new QWidget();
    TabScriptInterpreter->setObjectName( "TabScriptInterpreter" );

    Splitter->addWidget( ScriptTableWidget );
    Splitter->addWidget( ScriptTabWidget );

    ButtonLoadScript = new QPushButton( this );
    ButtonLoadScript->setObjectName( "ButtonLoadScript" );
    ButtonLoadScript->setProperty( "HxButton", "true" );

    label = new QLabel( this );
    label->setObjectName( "label" );
    label->setProperty( "HxLabelDisplay", "true" );

    gridLayout->addItem( Spacer, 0, 2, 1, 1 );
    gridLayout->addWidget( Splitter, 1, 0, 1, 4 );
    gridLayout->addWidget( label, 0, 3, 1, 1 );
    gridLayout->addWidget( ButtonLoadScript, 0, 0, 1, 2 );

    retranslateUi();

    QMetaObject::connectSlotsByName( this );
}

auto HavocPageScript::retranslateUi() -> void {
    setWindowTitle( "ScriptPage" );
    ButtonLoadScript->setText( "Load Script" );
    label->setText( "Scripts loaded: 0" );
}