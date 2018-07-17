#include <LicensesDialog.h>
#include <ui_LicensesDialog.h>
using Cliniface::LicensesDialog;


LicensesDialog::LicensesDialog(QWidget *parent) : QDialog(parent), ui(new Ui::LicensesDialog)
{
    ui->setupUi(this);
    this->setModal(false);
    connect( ui->closeButton, &QPushButton::clicked, this, &QDialog::close);
    ui->textBrowser->setOpenExternalLinks(true);
}   // end ctor


LicensesDialog::~LicensesDialog()
{
    delete ui;
}   // end dtor
