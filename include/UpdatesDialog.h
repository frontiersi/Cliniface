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

#include <QTools/NetworkUpdater.h>
#include <QTemporaryFile>
#include <QDialog>

namespace Ui { class UpdatesDialog;}
namespace Cliniface {

class UpdatesDialog : public QDialog
{ Q_OBJECT
public:
    explicit UpdatesDialog(QWidget *parent = nullptr);
    ~UpdatesDialog();

    // Check silently for an update and only open this dialog if one exists.
    void checkForUpdate();

    static void setCheckUpdateAtStart( bool);
    static bool checkUpdateAtStart();

protected:
    void closeEvent( QCloseEvent*) override;
    QSize sizeHint() const override;

public slots:
    void open() override;

private slots:
    void reject() override;
    void _doOnReplyFinished( bool);
    void _doOnDownloadProgress( qint64, qint64);
    void _doOnUpdateButtonPushed();
    void _doOnFinishedUpdate( QString);

private:
    Ui::UpdatesDialog *_ui;
    QTools::NetworkUpdater _nupdater;
    QTemporaryFile *_updateFile;
    QTools::UpdateMeta _cmeta;
    bool _allowChecking;
    bool _gettingManifest;

    void _checkRefreshedManifest();
    void _startUpdate();
    void _handleError( const QString&);
    void _deleteDownloadedUpdateFile();
    static bool s_checkUpdateAtStart;
};  // end class

}   // end namespace

#endif
