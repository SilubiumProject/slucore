#include "tokendescdialog.h"
#include "ui_tokendescdialog.h"

#include "tokenfilterproxy.h"
#include "styleSheet.h"

#include <QModelIndex>
#include <QPushButton>

TokenDescDialog::TokenDescDialog(const QModelIndex &idx, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TokenDescDialog)
{
    ui->setupUi(this);

    // Set stylesheet
    SetObjectStyleSheet(this, StyleSheetNames::ScrollBarDark);
    ui->buttonBox->button(QDialogButtonBox::Close)->setText(tr("&CLOSE"));
    setWindowTitle(tr("Details for %1").arg(idx.data(TokenTransactionTableModel::TxHashRole).toString()));
    QString desc = idx.data(TokenTransactionTableModel::LongDescriptionRole).toString();
    ui->detailText->setHtml(desc);
}

TokenDescDialog::~TokenDescDialog()
{
    delete ui;
}
