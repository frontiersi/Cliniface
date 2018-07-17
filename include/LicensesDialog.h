#ifndef Cliniface_LicensesDialog_h
#define Cliniface_LicensesDialog_h

#include <QDialog>

namespace Ui { class LicensesDialog;}
namespace Cliniface {

class LicensesDialog : public QDialog
{ Q_OBJECT
public:
    explicit LicensesDialog( QWidget *parent = nullptr);
    ~LicensesDialog() override;

private:
    Ui::LicensesDialog *ui;
    LicensesDialog( const LicensesDialog&) = delete;
    void operator=( const LicensesDialog&) = delete;
};  // end class

}   // end namespace

#endif
