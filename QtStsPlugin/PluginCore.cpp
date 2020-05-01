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
#include "PluginCore.h"

#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QByteArray>
#include <QTime>

namespace QtSts {
	static const QString stsANLAGENINFO(QStringLiteral("anlageninfo"));
	static const QString stsNAME(QStringLiteral("name"));
	static const QString stsSENDER(QStringLiteral("sender"));
	static const QString stsSIMZEIT(QStringLiteral("simzeit"));
	static const QString stsSTATUS(QStringLiteral("status"));

	static constexpr int stsSTATUS_NOT_REGISTERED = 300;
	static constexpr int stsSTATUS_REGISTERED_OK = 220;
}

QtSts::PluginCore::PluginCore(const QString& pluginName,
	const QString& pluginAuthor,
	const QString& pluginVersion,
	const QString& pluginDescription,
	QObject *parent)
	: QObject(parent)
	, m_xmlReader(new QXmlStreamReader)
	, m_pluginName(pluginName)
	, m_pluginAuthor(pluginAuthor)
	, m_pluginVersion(pluginVersion)
	, m_pluginDescription(pluginDescription)
	, m_signalBoxName()
	, m_streamActive(false)
	, m_registered(false)
	, m_inStatus(0)
	, m_rtt(0)
	, m_timeoffset(0)
	, m_signalBoxId(0)
	, m_simbuild(0)
{
}

QtSts::PluginCore::~PluginCore() = default;

void QtSts::PluginCore::requestSimTime()
{
	const int now = QTime::currentTime().msecsSinceStartOfDay();

	QByteArray data;
	QXmlStreamWriter xml(&data);
	xml.writeStartElement(stsSIMZEIT);
	xml.writeAttribute(stsSENDER, QString::number(now));
	xml.writeEndElement();
	xml.writeEndDocument();

	Q_EMIT sendToSts(data);
}

void QtSts::PluginCore::requestSignalBoxInfo()
{
	sendSimpleCommand(stsANLAGENINFO);
}

void QtSts::PluginCore::receivedFromSts(const QByteArray& data)
{
	m_xmlReader->addData(data);

	QXmlStreamReader::TokenType token = QXmlStreamReader::NoToken;
	while (!m_xmlReader->atEnd())
	{
		token = m_xmlReader->readNext();
		switch (token)
		{
		case QXmlStreamReader::StartDocument:
			m_streamActive = true;
			break;
		case QXmlStreamReader::StartElement:
			handleStartElement();
			break;
		case QXmlStreamReader::Characters:
			handleCharacters();
			break;
		case QXmlStreamReader::EndElement:
			handleEndElement();
			break;
		case QXmlStreamReader::EndDocument:
			m_xmlReader->clear();
			m_registered = false;
			m_streamActive = false;
			m_rtt = 0;
			m_timeoffset = 0;
			m_signalBoxId = 0;
			m_simbuild = 0;
			m_signalBoxName.clear();
			break;
		case QXmlStreamReader::Invalid:
			//handleInvalid();
			break;
		default:
			break;
		}
	}
}

void QtSts::PluginCore::handleStartElement()
{
	const QStringRef rName = m_xmlReader->name();
	const QXmlStreamAttributes attributes = m_xmlReader->attributes();

	auto nameIs = [&rName](const QString& value) ->bool {
		return (rName.compare(value, Qt::CaseInsensitive) == 0);
	};

	if (nameIs(stsSTATUS))
	{
		const QStringRef rCode = attributes.value(QStringLiteral("code"));
		bool ok = false;
		const int status = rCode.toInt(&ok);
		if (ok)
		{
			m_inStatus = status;
		}
	}
	else if (nameIs(stsSIMZEIT))
	{
		parseSimTime(attributes);
	}
	else if (nameIs(stsANLAGENINFO))
	{
		parseSignalBoxInfo(attributes);
	}
}

void QtSts::PluginCore::handleCharacters()
{
	const QString text = m_xmlReader->text().toString();
	if (m_inStatus != 0)
	{
		Q_EMIT statusMessageReceived(m_inStatus, text);
	}
}

void QtSts::PluginCore::handleEndElement()
{
	const QStringRef rName = m_xmlReader->name();

	auto nameIs = [&rName](const QString& value) ->bool {
		return (rName.compare(value, Qt::CaseInsensitive) == 0);
	};

	if (nameIs(stsSTATUS))
	{
		switch (m_inStatus)
		{
		case stsSTATUS_NOT_REGISTERED:
			// Registration required.
			sendRegister();
			break;
		case stsSTATUS_REGISTERED_OK:
			// Registered ok. Ready.
			m_registered = true;
			Q_EMIT registered();
			break;
		default:
			break;
		}
		m_inStatus = 0;
	}
	else if (nameIs(stsSIMZEIT))
	{
		Q_EMIT timeReceived(m_timeoffset, m_rtt);
	}
	else if (nameIs(stsANLAGENINFO))
	{
		Q_EMIT signalBoxInfoReceived(m_simbuild, m_signalBoxId, m_signalBoxName);
	}
}

void QtSts::PluginCore::sendRegister()
{
	QXmlStreamAttributes attributes;
	attributes.append(stsNAME, m_pluginName);
	attributes.append(QStringLiteral("autor"), m_pluginAuthor);
	attributes.append(QStringLiteral("version"), m_pluginVersion);
	attributes.append(QStringLiteral("protokoll"), QStringLiteral("1"));
	attributes.append(QStringLiteral("text"), m_pluginDescription);

	QByteArray data;
	QXmlStreamWriter xml(&data);
	xml.writeStartElement(QStringLiteral("register"));
	xml.writeAttributes(attributes);
	xml.writeEndElement();
	xml.writeEndDocument();

	sendToSts(data);
}

void QtSts::PluginCore::sendSimpleCommand(const QString& command)
{
	QByteArray data;
	QXmlStreamWriter xml(&data);
	xml.writeStartElement(command);
	xml.writeEndElement();
	xml.writeEndDocument();

	Q_EMIT sendToSts(data);
}

void QtSts::PluginCore::parseSimTime(const QXmlStreamAttributes& attributes)
{
	const int now = QTime::currentTime().msecsSinceStartOfDay();

	const QStringRef rSender = attributes.value(stsSENDER);
	const QStringRef rTime = attributes.value(QStringLiteral("zeit"));
	if (!rSender.isNull() && !rTime.isNull())
	{
		const int sender = rSender.toInt();
		int simTime = rTime.toInt();
		m_rtt = now - sender;
		const int rttOffset = m_rtt / 2;
		simTime += rttOffset;
		m_timeoffset = simTime - now;
	}
}

void QtSts::PluginCore::parseSignalBoxInfo(const QXmlStreamAttributes& attributes)
{
	const QStringRef rName = attributes.value(stsNAME);
	if (!rName.isNull())
		m_signalBoxName = rName.toString();
	const QStringRef rAid = attributes.value(QStringLiteral("aid"));
	if (!rAid.isNull())
		m_signalBoxId = rAid.toUInt();
	const QStringRef rSimbuild = attributes.value(QStringLiteral("simbuild"));
	if (!rSimbuild.isNull())
		m_simbuild = rSimbuild.toUInt();
}
