/***************************************************************************
    qgsvaluemapwidgetfactory.cpp
     --------------------------------------
    Date                 : 5.1.2014
    Copyright            : (C) 2014 Matthias Kuhn
    Email                : matthias at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgsvaluemapwidgetfactory.h"

#include "qgsvaluemapwidgetwrapper.h"
#include "qgsvaluemapsearchwidgetwrapper.h"
#include "qgsdefaultsearchwidgetwrapper.h"
#include "qgsvaluemapconfigdlg.h"

#include <QSettings>

QgsValueMapWidgetFactory::QgsValueMapWidgetFactory( const QString& name )
    : QgsEditorWidgetFactory( name )
{
}


QgsEditorWidgetWrapper* QgsValueMapWidgetFactory::create( QgsVectorLayer* vl, int fieldIdx, QWidget* editor, QWidget* parent ) const
{
  return new QgsValueMapWidgetWrapper( vl, fieldIdx, editor, parent );
}


QgsSearchWidgetWrapper* QgsValueMapWidgetFactory::createSearchWidget( QgsVectorLayer* vl, int fieldIdx, QWidget* parent ) const
{
  return new QgsValueMapSearchWidgetWrapper( vl, fieldIdx, parent );
}

QgsEditorConfigWidget* QgsValueMapWidgetFactory::configWidget( QgsVectorLayer* vl, int fieldIdx, QWidget* parent ) const
{
  return new QgsValueMapConfigDlg( vl, fieldIdx, parent );
}

QgsEditorWidgetConfig QgsValueMapWidgetFactory::readConfig( const QDomElement& configElement, QgsVectorLayer* layer, int fieldIdx )
{
  Q_UNUSED( layer )
  Q_UNUSED( fieldIdx )

  QgsEditorWidgetConfig cfg;

  QDomNodeList nodes = configElement.elementsByTagName( QStringLiteral( "value" ) );

  for ( int i = 0; i < nodes.size(); ++i )
  {
    QDomElement elem = nodes.at( i ).toElement();
    cfg.insert( elem.attribute( QStringLiteral( "key" ) ), elem.attribute( QStringLiteral( "value" ) ) );
  }

  return cfg;
}

void QgsValueMapWidgetFactory::writeConfig( const QgsEditorWidgetConfig& config, QDomElement& configElement, QDomDocument& doc, const QgsVectorLayer* layer, int fieldIdx )
{
  Q_UNUSED( layer )
  Q_UNUSED( fieldIdx )

  QgsEditorWidgetConfig::ConstIterator it = config.constBegin();

  while ( it != config.constEnd() )
  {
    QDomElement elem = doc.createElement( QStringLiteral( "value" ) );

    elem.setAttribute( QStringLiteral( "key" ), it.key() );
    elem.setAttribute( QStringLiteral( "value" ), it.value().toString() );

    configElement.appendChild( elem );

    ++it;
  }
}

QString QgsValueMapWidgetFactory::representValue( QgsVectorLayer* vl, int fieldIdx, const QgsEditorWidgetConfig& config, const QVariant& cache, const QVariant& value ) const
{
  Q_UNUSED( cache )

  QString valueInternalText;
  if ( value.isNull() )
    valueInternalText = QStringLiteral( VALUEMAP_NULL_TEXT );
  else
    valueInternalText = value.toString();

  return config.key( valueInternalText, QVariant( QStringLiteral( "(%1)" ).arg( vl->fields().at( fieldIdx ).displayString( value ) ) ).toString() );
}

QVariant QgsValueMapWidgetFactory::sortValue( QgsVectorLayer* vl, int fieldIdx, const QgsEditorWidgetConfig& config, const QVariant& cache, const QVariant& value ) const
{
  return representValue( vl, fieldIdx, config, cache, value );
}

Qt::AlignmentFlag QgsValueMapWidgetFactory::alignmentFlag( QgsVectorLayer* vl, int fieldIdx, const QgsEditorWidgetConfig& config ) const
{
  Q_UNUSED( vl );
  Q_UNUSED( fieldIdx );
  Q_UNUSED( config );

  return Qt::AlignLeft;
}

QHash<const char*, int> QgsValueMapWidgetFactory::supportedWidgetTypes()
{
  QHash<const char*, int> map = QHash<const char*, int>();
  map.insert( QComboBox::staticMetaObject.className(), 10 );
  return map;
}
