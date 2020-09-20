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
    void _doOnError( const QString&);
    void _doOnRefreshedManifest();
    void _doOnDownloadProgress( qint64, qint64);
    void _doOnFinishedDownloadingUpdate();
    void _doOnUpdateButtonPushed();
    void _doOnFinishedUpdate();

private:
    Ui::UpdatesDialog *_ui;
    QTools::NetworkUpdater _nupdater;
    QTools::UpdateMeta _cmeta;

    static bool s_checkUpdateAtStart;
};  // end class

}   // end namespace

#endif
