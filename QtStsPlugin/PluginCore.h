/*****************************************************************************
QtStsPlugin - Qt-based library for using StellwerkSim's plugin interface
Copyright (C) 2020  Jan Siebeneich

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published
by the Free Software Foundation, version 3 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*****************************************************************************/
#pragma once

#include "qtstsplugin_global.h"
#include <QObject>
#include <QString>
#include <QList>
#include <QPair>
#include <memory>
#include "Train.h"
#include "Timetable.h"

class QXmlStreamReader;
class QXmlStreamAttributes;
class QByteArray;

namespace QtSts {

	class QTSTSPLUGIN_EXPORT PluginCore : public QObject
	{
		Q_OBJECT

	public:
		using TrainListItem = QPair<int, QString>;
		using TrainList = QList<TrainListItem>;

		PluginCore(const QString& pluginName,
			const QString& pluginAuthor,
			const QString& pluginVersion,
			const QString& pluginDescription,
			QObject* parent = nullptr);
		~PluginCore() override;

		constexpr bool isStreamActive() const { return m_streamActive; }
		constexpr bool isRegistered() const { return m_registered; }
		constexpr int rtt() const { return m_rtt; }
		constexpr int timeoffset() const { return m_timeoffset; }
		constexpr int simbuild() const { return m_simbuild; }
		constexpr int signalBoxId() const { return m_signalBoxId; }
		const QString& signalBoxName() const { return m_signalBoxName; }
		constexpr int heat() const { return m_heat; }
		constexpr int stitzAllgemein() const { return m_stitzAllgemein; }
		constexpr int stitzRegion() const { return m_stitzRegion; }
		const TrainList& trainList() const { return m_trainList; }

	public Q_SLOTS:
		void requestSimTime();
		void requestSignalBoxInfo();
		void requestHeat();
		void requestStitz();
		void requestTrainList();
		void requestTrainInfo(int trainId);
		void requestTimeTable(int trainId);
		void registerEvent(int trainId, QtSts::TrainEvent event);
		void receivedFromSts(const QByteArray& data);

	Q_SIGNALS:
		void registered();
		void sendToSts(const QByteArray& data);
		void statusMessageReceived(int code, const QString& text);
		void timeReceived(int offset, int rtt);
		void signalBoxInfoReceived(int simbuild, int aid, const QString& name);
		void heatReceived(int heat);
		void stitzReceived(int allgemein, int region);
		void trainListReceived(const QList<QPair<int, QString>>& trainList);
		void trainDetailsReceived(QtSts::Train train, QtSts::TrainEvent event);
		void timetableReceived(QtSts::Timetable timetable);

	private:
		void handleStartElement();
		void handleCharacters();
		void handleEndElement();
		void sendRegister();
		void sendSimpleCommand(const QString& command);
		void parseSimTime(const QXmlStreamAttributes& attributes);
		void parseSignalBoxInfo(const QXmlStreamAttributes& attributes);
		void parseHeat(const QXmlStreamAttributes& attributes);
		void parseStitz(const QXmlStreamAttributes& attributes);
		void parseTrain(const QXmlStreamAttributes& attributes);
		void parseTrainDetails(const QXmlStreamAttributes& attributes);
		void parseTimetable(const QXmlStreamAttributes& attributes);
		void parseTimetableEntry(const QXmlStreamAttributes& attributes);

		std::unique_ptr<QXmlStreamReader> m_xmlReader;
		QString m_pluginName;
		QString m_pluginAuthor;
		QString m_pluginVersion;
		QString m_pluginDescription;
		QString m_signalBoxName;
		bool m_streamActive;
		bool m_registered;
		int m_inStatus;
		int m_rtt;
		int m_timeoffset;
		int m_signalBoxId;
		int m_simbuild;
		int m_heat;
		int m_stitzAllgemein;
		int m_stitzRegion;
		TrainList m_trainList;
		Timetable m_timetable;
	};

}