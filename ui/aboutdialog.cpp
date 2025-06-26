#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    setWindowFlag(Qt::MSWindowsFixedSizeDialogHint);  // не тянется
    /* делает кнопку Close рабочей */
    connect(ui->buttonBox, &QDialogButtonBox::rejected,
            this,          &QDialog::reject);
}

AboutDialog::~AboutDialog() { delete ui; }
