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
#include <memory>

class QXmlStreamReader;
class QByteArray;

namespace QtSts {

	class QTSTSPLUGIN_EXPORT PluginCore : public QObject
	{
		Q_OBJECT

	public:
		PluginCore(const QString& pluginName,
			const QString& pluginAuthor,
			const QString& pluginVersion,
			const QString& pluginDescription,
			QObject* parent = nullptr);
		~PluginCore() override;

		constexpr bool isConnected() const { return m_connected; }

	public Q_SLOTS:
		void parseData(const QByteArray& data);

	Q_SIGNALS:
		void communicationReady();
		void communicationStopped();

	private:
		void handleStartElement();
		void handleEndElement();

		std::unique_ptr<QXmlStreamReader> m_xmlReader;
		QString m_pluginName;
		QString m_pluginAuthor;
		QString m_pluginVersion;
		QString m_pluginDescription;
		bool m_connected;
		int m_inStatus;
	};

}