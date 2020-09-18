/************************************************************************
 * Copyright (C) 2020 SIS Research Ltd & Richard Palmer
 *
 * Cliniface is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Cliniface is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ************************************************************************/

#ifndef Cliniface_UpdatesDialog_h
#define Cliniface_UpdatesDialog_h

#include <FaceTools/Action/ActionFindUpdate.h>
#include <QTemporaryFile>
#include <QDialog>

using namespace FaceTools::Action;

namespace Ui { class UpdatesDialog;}
namespace Cliniface {

class UpdatesDialog : public QDialog
{ Q_OBJECT
public:
    explicit UpdatesDialog(QWidget *parent = nullptr);
    ~UpdatesDialog();

    QAction *checkForUpdateAction() { return _finder->qaction();}

    static void setCheckUpdateAtStart( bool);
    static bool checkUpdateAtStart();

protected:
    void closeEvent( QCloseEvent*) override;
    QSize sizeHint() const override;

private slots:
    void reject() override;
    void _doOnFoundUpdate( bool);
    void _doOnDownloadProgress( qint64, qint64);
    void _doOnUpdateButtonPushed();
    void _doOnUpdateDownloaded();
    void _doOnFinishedUpdate( QString);
    void _doOnError( const QString&);

private:
    Ui::UpdatesDialog *_ui;
    ActionFindUpdate *_finder;
    QTemporaryFile *_updateFile;
    void _deleteDownloadedUpdateFile();

    static bool s_checkUpdateAtStart;
};  // end class

}   // end namespace

#endif
