#ifndef Cliniface_HelpDialog_h
#define Cliniface_HelpDialog_h

#include <QDialog>

namespace Ui { class HelpDialog;}
namespace Cliniface {

class HelpDialog : public QDialog
{ Q_OBJECT
public:
    explicit HelpDialog( QWidget *parent = nullptr);
    ~HelpDialog() override;

private:
    Ui::HelpDialog *ui;
    HelpDialog( const HelpDialog&) = delete;
    void operator=( const HelpDialog&) = delete;
};  // end class

}   // end namespace

#endif
