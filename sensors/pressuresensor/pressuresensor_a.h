/**
   @file pressuresensor_a.h
   @brief D-Bus adaptor for PressureSensor

   <p>
   Copyright (C) 2016 Canonical LTD.

   @author Lorn Potter <lorn.potter@canonical.com>

   This file is part of Sensorfw.

   Sensord is free software; you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License
   version 2.1 as published by the Free Software Foundation.

   Sensord is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with Sensord.  If not, see <http://www.gnu.org/licenses/>.
   </p>
 */

#ifndef PRESSURE_SENSOR_H
#define PRESSURE_SENSOR_H

#include <QtDBus/QtDBus>
#include <QObject>

#include "datatypes/unsigned.h"
#include "abstractsensor_a.h"

class PressureSensorChannelAdaptor : public AbstractSensorChannelAdaptor
{
    Q_OBJECT
    Q_DISABLE_COPY(PressureSensorChannelAdaptor)
    Q_CLASSINFO("D-Bus Interface", "local.PressureSensor")
    Q_PROPERTY(Unsigned pressure READ pressure NOTIFY pressureChanged)

public:
    PressureSensorChannelAdaptor(QObject* parent);

public Q_SLOTS:
    Unsigned pressure() const;

Q_SIGNALS:
    void pressureChanged(const Unsigned& value);
};

#endif
