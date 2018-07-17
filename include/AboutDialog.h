#ifndef Cliniface_AboutDialog_h
#define Cliniface_AboutDialog_h

#include <QDialog>

namespace Ui { class AboutDialog;}
namespace Cliniface {

class AboutDialog : public QDialog
{ Q_OBJECT
public:
    explicit AboutDialog(QWidget *parent = nullptr);
    ~AboutDialog() override;

private:
    Ui::AboutDialog *ui;

    void insertHeader();
    void appendPara( const QString&);
    void finishContent();
    void insertContent();
    AboutDialog( const AboutDialog&) = delete;
    void operator=( const AboutDialog&) = delete;
};  // end class

}   // end namespace

#endif
