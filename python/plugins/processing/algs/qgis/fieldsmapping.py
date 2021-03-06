# -*- coding: utf-8 -*-

"""
***************************************************************************
    FieldsMapper.py
    ---------------------
    Date                 : October 2014
    Copyright            : (C) 2014 by Arnaud Morvan
    Email                : arnaud dot morvan at camptocamp dot com
***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************
"""
from __future__ import print_function
from builtins import str

__author__ = 'Arnaud Morvan'
__date__ = 'October 2014'
__copyright__ = '(C) 2014, Arnaud Morvan'

# This will get replaced with a git SHA1 when you do a git archive

__revision__ = '$Format:%H$'


from processing.core.parameters import Parameter


class ParameterFieldsMapping(Parameter):

    default_metadata = {
        'widget_wrapper': 'processing.algs.qgis.ui.FieldsMappingPanel.FieldsMappingWidgetWrapper'
    }

    def __init__(self, name='', description='', parent=None):
        Parameter.__init__(self, name, description)
        self.parent = parent
        self.value = []

    def getValueAsCommandLineParameter(self):
        return '"' + str(self.value) + '"'

    def setValue(self, value):
        if value is None:
            return False
        if isinstance(value, list):
            self.value = value
            return True
        if isinstance(value, str):
            try:
                self.value = eval(value)
                return True
            except Exception as e:
                # fix_print_with_import
                print(str(e))  # display error in console
                return False
        return False
