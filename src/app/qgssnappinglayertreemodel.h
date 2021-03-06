/***************************************************************************
  qgssnappinglayertreemodel.h - QgsSnappingLayerTreeModel

 ---------------------
 begin                : 31.8.2016
 copyright            : (C) 2016 by Denis Rouzaud
 email                : denis.rouzaud@gmail.com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef QGSSNAPPINGLAYERTREEVIEW_H
#define QGSSNAPPINGLAYERTREEVIEW_H



#include <QSortFilterProxyModel>
#include <QItemDelegate>

#include "qgslayertreemodel.h"
#include "qgssnappingconfig.h"

class QgsMapCanvas;
class QgsProject;


class APP_EXPORT QgsSnappingLayerDelegate : public QItemDelegate
{
    Q_OBJECT

  public:
    explicit QgsSnappingLayerDelegate( QgsMapCanvas* canvas, QObject *parent = nullptr );

    QWidget *createEditor( QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index ) const override;
    void setEditorData( QWidget *editor, const QModelIndex &index ) const override;
    void setModelData( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const override;

  private:
    QgsMapCanvas* mCanvas;
};


class APP_EXPORT QgsSnappingLayerTreeModel : public QSortFilterProxyModel
{
    Q_OBJECT

  public:
    enum Columns
    {
      LayerColumn = 0,
      TypeColumn,
      ToleranceColumn,
      UnitsColumn,
      AvoidIntersectionColumn
    };

    QgsSnappingLayerTreeModel( QgsProject* project, QObject* parent = nullptr );
    ~QgsSnappingLayerTreeModel();

    int columnCount( const QModelIndex& parent ) const override;
    QVariant headerData( int section, Qt::Orientation orientation, int role ) const override;
    Qt::ItemFlags flags( const QModelIndex& idx ) const override;
    QModelIndex index( int row, int column, const QModelIndex &parent = QModelIndex() ) const override;
    QModelIndex parent( const QModelIndex& child ) const override;
    QModelIndex sibling( int row, int column, const QModelIndex &idx ) const override;
    QVariant data( const QModelIndex& index, int role ) const override;
    bool setData( const QModelIndex& index, const QVariant& value, int role ) override;

    QgsLayerTreeModel* layerTreeModel() const;
    void setLayerTreeModel( QgsLayerTreeModel* layerTreeModel );

    QgsVectorLayer* vectorLayer( const QModelIndex& idx ) const;

  protected:
    bool filterAcceptsRow( int sourceRow, const QModelIndex& sourceParent ) const override;

  private slots:
    void onSnappingSettingsChanged();

  private:
    bool nodeShown( QgsLayerTreeNode* node ) const;

    QgsProject* mProject;
    QHash<QgsVectorLayer*, QgsSnappingConfig::IndividualLayerSettings> mIndividualLayerSettings;
    QgsLayerTreeModel* mLayerTreeModel;

    void hasRowchanged( QgsLayerTreeNode* node, const QHash<QgsVectorLayer*, QgsSnappingConfig::IndividualLayerSettings>& oldSettings );
};

#endif // QGSSNAPPINGLAYERTREEVIEW_H
