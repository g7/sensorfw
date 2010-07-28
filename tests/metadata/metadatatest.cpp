/**
   @file metadatatest.cpp
   @brief Automatic tests for sensor client interfaces

   <p>
   Copyright (C) 2009-2010 Nokia Corporation

   @author Timo Rongas <ext-timo.2.rongas@nokia.com>

   This file is part of Sensord.

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

#include "qt-api/sensormanagerinterface.h"
#include "qt-api/accelerometersensor_i.h"
#include "qt-api/magnetometersensor_i.h"
#include "qt-api/alssensor_i.h"
#include "qt-api/compasssensor_i.h"
#include "qt-api/rotationsensor_i.h"
#include "qt-api/tapsensor_i.h"
#include "qt-api/proximitysensor_i.h"
#include "qt-api/orientationsensor_i.h"

#include "metadatatest.h"


void MetaDataTest::initTestCase()
{
    SensorManagerInterface& remoteSensorManager = SensorManagerInterface::instance();
    QVERIFY( remoteSensorManager.isValid() );

    // Load plugins (should test running depend on plug-in load result?)
    remoteSensorManager.loadPlugin("accelerometersensor");
    remoteSensorManager.loadPlugin("magnetometersensor");
    remoteSensorManager.loadPlugin("alssensor");
    remoteSensorManager.loadPlugin("orientationsensor");
    remoteSensorManager.loadPlugin("tapsensor");
    remoteSensorManager.loadPlugin("rotationsensor");
    remoteSensorManager.loadPlugin("compasssensor");
    remoteSensorManager.loadPlugin("proximitysensor");

    remoteSensorManager.registerSensorInterface<AccelerometerSensorChannelInterface>("accelerometersensor");
    remoteSensorManager.registerSensorInterface<MagnetometerSensorChannelInterface>("magnetometersensor");
    remoteSensorManager.registerSensorInterface<ALSSensorChannelInterface>("alssensor");
    remoteSensorManager.registerSensorInterface<OrientationSensorChannelInterface>("orientationsensor");
    remoteSensorManager.registerSensorInterface<TapSensorChannelInterface>("tapsensor");
    remoteSensorManager.registerSensorInterface<RotationSensorChannelInterface>("rotationsensor");
    remoteSensorManager.registerSensorInterface<CompassSensorChannelInterface>("compasssensor");
    remoteSensorManager.registerSensorInterface<ProximitySensorChannelInterface>("proximitysensor");
}

void MetaDataTest::init()
{
    //qDebug() << "Run before each test";
    //TODO: Verify that sensord has not crashed.
}

void MetaDataTest::cleanup()
{
    //qDebug() << "Run after each test";
    //TODO: Verify that sensord has not crashed.
}

void MetaDataTest::cleanupTestCase()
{
    //qDebug() << "Run after all test cases";
}

void MetaDataTest::testDescription()
{
    QString sensorName("accelerometersensor");
    SensorManagerInterface& sm = SensorManagerInterface::instance();
    QVERIFY( sm.isValid() );

    // Get control session
    AccelerometerSensorChannelInterface* sensorIfc = AccelerometerSensorChannelInterface::controlInterface(sensorName);
    QVERIFY2(sensorIfc && sensorIfc->isValid(), "Failed to get control session");

    QString desc = sensorIfc->description();

    QVERIFY2(desc.size() > 0, "No description received for sensor");
    qDebug() << sensorName << "reported description: " << desc;

    delete sensorIfc;
}


void MetaDataTest::testAvailableRanges()
{
    QString sensorName("accelerometersensor");
    SensorManagerInterface& sm = SensorManagerInterface::instance();
    QVERIFY( sm.isValid() );

    // Get control session
    AccelerometerSensorChannelInterface* sensorIfc = AccelerometerSensorChannelInterface::controlInterface(sensorName);
    QVERIFY2(sensorIfc && sensorIfc->isValid(), "Failed to get control session");

    // Request list of available data ranges
    QList<DataRange> dataRangeList = sensorIfc->getAvailableDataRanges();
    QVERIFY2(dataRangeList.size() > 0, "No data ranges received from sensor");
    qDebug() << "Received ranges:";
    for (int i = 0; i < dataRangeList.size(); i++) {
        DataRange range = dataRangeList.at(i);
        qDebug() << i+1 << ". [" << range.min << ", " << range.max << "], " << range.resolution;
    }

    delete sensorIfc;
}

void MetaDataTest::testGetCurrentRange()
{
    QString sensorName("accelerometersensor");
    SensorManagerInterface& sm = SensorManagerInterface::instance();
    QVERIFY( sm.isValid() );

    // Get control session
    AccelerometerSensorChannelInterface* sensorIfc = AccelerometerSensorChannelInterface::controlInterface(sensorName);
    QVERIFY2(sensorIfc && sensorIfc->isValid(), "Failed to get control session");

    // Request list of available data ranges
    QList<DataRange> dataRangeList = sensorIfc->getAvailableDataRanges();
    QVERIFY2(dataRangeList.size() > 0, "No data ranges received from sensor");

    // Verify that default == the first
    DataRange range = sensorIfc->getCurrentDataRange();
    QVERIFY2(range == dataRangeList.at(0), "Default range is not the first option.");

    // Request a different range
    sensorIfc->requestDataRange(dataRangeList.last());

    // Verify that current is the requested one (needs at least 2 range options)
    range = sensorIfc->getCurrentDataRange();
    QVERIFY2(range == dataRangeList.last() || dataRangeList.size() < 2, "Requested rate not set correctly.");

    // Remove request
    sensorIfc->removeDataRangeRequest();

    // Verify that current == default (needs at least 2 range options)
    range = sensorIfc->getCurrentDataRange();
    QVERIFY2(range == dataRangeList.at(0) || dataRangeList.size() < 2, "Default range is not the first option.");

    delete sensorIfc;
}

void MetaDataTest::testConcurrentRangeRequests()
{
    // Test behavior of two different sessions making concurrent requests
}

void MetaDataTest::testChangeNotifications()
{
    DummyHelper dummy;

    QString sensorName("accelerometersensor");
    SensorManagerInterface& sm = SensorManagerInterface::instance();
    QVERIFY( sm.isValid() );

    // Get control session
    AccelerometerSensorChannelInterface* sensorIfc = AccelerometerSensorChannelInterface::controlInterface(sensorName);
    QVERIFY2(sensorIfc && sensorIfc->isValid(), "Failed to get control session");

    // Request list of available data ranges
    QList<DataRange> dataRangeList = sensorIfc->getAvailableDataRanges();
    QVERIFY2(dataRangeList.size() > 0, "No data ranges received from sensor");

    if (dataRangeList.size() > 1)
    {
        // Connect change notification signal
        connect(sensorIfc, SIGNAL(propertyChanged(const QString&)), &dummy, SLOT(propertyChanged(const QString&)));

        // Make some requests
        sensorIfc->requestDataRange(dataRangeList.last());
        sensorIfc->start();
        sensorIfc->setInterval(50);
        sensorIfc->stop();

        // Allow signals some time ...
        QTest::qWait(200);

        QVERIFY2(dummy.hasReceived("datarange"), "Property notification for \"DataRange\" not received");
        QVERIFY2(dummy.hasReceived("interval"), "Property notification for \"Interval\" not received");
    }

    delete sensorIfc;
}

void MetaDataTest::testAvailableIntervals()
{
    // TODO: This type of test should be done for each sensor separately

    QString sensorName("accelerometersensor");
    SensorManagerInterface& sm = SensorManagerInterface::instance();
    QVERIFY( sm.isValid() );

    // Get control session
    AccelerometerSensorChannelInterface* sensorIfc = AccelerometerSensorChannelInterface::controlInterface(sensorName);
    QVERIFY2(sensorIfc && sensorIfc->isValid(), "Failed to get control session");

    // Request list of available intervals
    QList<DataRange> intervalList = sensorIfc->getAvailableIntervals();

    delete sensorIfc;

    QVERIFY2(intervalList.size() > 0, "No intervals received from sensor");
}

void MetaDataTest::printMetaData()
{
    QList<QString> sensorNameList;
    QList<AbstractSensorChannelInterface*> sensorList;

    // Initiate sensors
    sensorNameList << "accelerometer";
    sensorList << const_cast<AccelerometerSensorChannelInterface*>(AccelerometerSensorChannelInterface::listenInterface("accelerometersensor"));

    sensorNameList << "magnetometer";
    sensorList << const_cast<MagnetometerSensorChannelInterface*>(MagnetometerSensorChannelInterface::listenInterface("magnetometersensor"));

    sensorNameList << "als";
    sensorList << const_cast<ALSSensorChannelInterface*>(ALSSensorChannelInterface::listenInterface("alssensor"));

    sensorNameList << "proximity";
    sensorList << const_cast<ProximitySensorChannelInterface*>(ProximitySensorChannelInterface::listenInterface("proximitysensor"));

    sensorNameList << "orientation";
    sensorList << const_cast<OrientationSensorChannelInterface*>(OrientationSensorChannelInterface::listenInterface("orientationsensor"));

    sensorNameList << "tap";
    sensorList << const_cast<TapSensorChannelInterface*>(TapSensorChannelInterface::listenInterface("tapsensor"));

    sensorNameList << "compass";
    sensorList << const_cast<CompassSensorChannelInterface*>(CompassSensorChannelInterface::listenInterface("compasssensor"));

    sensorNameList << "rotation";
    sensorList << const_cast<RotationSensorChannelInterface*>(RotationSensorChannelInterface::listenInterface("rotationsensor"));

    AbstractSensorChannelInterface* sensor;
    while (sensorList.size() > 0)
    {
        qDebug() << "[" << sensorNameList.takeFirst() << "]:";
        sensor = sensorList.takeFirst();
        if (sensor != NULL)
        {
            qDebug() << "   Description    :" << sensor->description().toAscii().data();

            qDebug() << "   Interval       :" << sensor->interval();
            qDebug() << "   Possible rates :";
            QList<DataRange> rateList = sensor->getAvailableIntervals();
            for (int i = 1; i <= rateList.size(); i++)
            {
                DataRange r = rateList.at(i-1);
                if (r.min == r.max)
                {
                    qDebug() << QString("                    %1. %2").arg(i).arg(r.min).toAscii().data();
                } else {
                    qDebug() << QString("                    %1. [%2, %3]").arg(i).arg(r.min).arg(r.max).toAscii().data();
                }
            }

            DataRange r = sensor->getCurrentDataRange();
            qDebug() << QString("   Data Range     : [%1, %2], resolution %3").arg(r.min).arg(r.max).arg(r.resolution).toAscii().data();
            qDebug() << "   Possible ranges:";
            QList<DataRange> rangeList = sensor->getAvailableDataRanges();
            for (int i = 1; i <= rangeList.size(); i++)
            {
                r = rangeList.at(i-1);
                qDebug() << QString("                    %1. [%2, %3], resolution %4").arg(i).arg(r.min).arg(r.max).arg(r.resolution).toAscii().data();
            }

            delete sensor;
        } else {
            qDebug() << "   <sensor initialisation failed>";
        }
        qDebug() << "";
    }
}

QTEST_MAIN(MetaDataTest)