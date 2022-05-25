/************************************************************************
 * Copyright (C) 2022 SIS Research Ltd & Richard Palmer
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

#ifndef Cliniface_ImageBrowser_h
#define Cliniface_ImageBrowser_h

#include <FaceTools/FaceTypes.h>
#include <ProgressDialog.h>
#include <QFileInfoList>
#include <QMainWindow>
#include <QModelIndex>
#include <QFileDialog>
#include <QtSql>
#include <QDir>

namespace Ui { class ImageBrowser;}
namespace Cliniface {

// Inherits from QMainWindow so this can work as a standalone app.
class ImageBrowser : public QMainWindow
{ Q_OBJECT
public:
    explicit ImageBrowser( QWidget *parent = nullptr);
    ~ImageBrowser() override;

    void init();    // Call to parse initial images into database at startup after main window opened

    void updateSelected( const QString&);

    // Load the examples into the database at initialisation?
    static void setParseExamples( bool);

signals:
    void onLoad( const QString&);

private slots:
    void _doOnCapturedFromDateChanged();
    void _doOnCapturedToDateChanged();
    void _doOnBirthdateFromDateChanged();
    void _doOnBirthdateToDateChanged();
    void _doFilterOnCriteria();
    void _doOnClearFilters();
    void _doOnClickedResetDatabase();
    void _doOnCurrentItemChanged( const QModelIndex&, const QModelIndex&);
    void _doOnItemActivated( const QModelIndex&);
    void _doOnClickedAddDirectory();
    void _doOnLoadedMetadata( const QFileInfoList&, const QList<FaceTools::FM*>&);
    void _doOnLoadMetadataCancelled();
    void _doOnFound3DFs( const QDir&, const QFileInfoList&);

private:
    static bool s_parseExamples;
    static const QString s_dbfile;

    Ui::ImageBrowser *_ui;
    QFileDialog *_fdialog;
    ProgressDialog *_pdialog;
    QSqlRelationalTableModel *_model;
    bool _refreshing;
    bool _filtering;

    void _finishLoadMetadata();
    void _add3DFsDir( const QString&);
    void _setThumbnail( const QPixmap &pmap=QPixmap());
    int _tableRowForFilePath( const QString&) const;
    void _setInterfaceEnabled( bool);
    size_t _testAndSetEnableLoad();

    ImageBrowser( const ImageBrowser&) = delete;
    void operator=( const ImageBrowser&) = delete;
};  // end class

}  // end namespace

#endif
