#include "PluginCore.h"

#include <QXmlStreamReader>
#include <QByteArray>

namespace QtSts {
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
	, m_connected(false)
	, m_inStatus(0)
{
}

QtSts::PluginCore::~PluginCore() = default;

void QtSts::PluginCore::handleStartElement()
{
	const QStringRef rName = m_xmlReader->name();
	const QXmlStreamAttributes attributes = m_xmlReader->attributes();

	auto nameIs = [&rName](const QString& value) ->bool {
		return (rName.compare(value) == 0);
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
}

void QtSts::PluginCore::handleEndElement()
{
	const QStringRef rName = m_xmlReader->name();

	auto nameIs = [&rName](const QString& value) ->bool {
		return (rName.compare(value) == 0);
	};

	if (nameIs(stsSTATUS))
	{
		switch (m_inStatus)
		{
		case stsSTATUS_NOT_REGISTERED:
			// Registration required.
			//sendRegister();
			break;
		case stsSTATUS_REGISTERED_OK:
			// Registered ok. Ready.
			Q_EMIT communicationReady();
			break;
		default:
			break;
		}
		m_inStatus = 0;
	}
}

void QtSts::PluginCore::parseData(const QByteArray& data)
{
	m_xmlReader->addData(data);

	QXmlStreamReader::TokenType token = QXmlStreamReader::NoToken;
	while (!m_xmlReader->atEnd())
	{
		token = m_xmlReader->readNext();
		switch (token)
		{
		case QXmlStreamReader::StartDocument:
			m_connected = true;
			break;
		case QXmlStreamReader::StartElement:
			handleStartElement();
			break;
		case QXmlStreamReader::Characters:
			//handleCharacters();
			break;
		case QXmlStreamReader::EndElement:
			handleEndElement();
			break;
		case QXmlStreamReader::EndDocument:
			m_xmlReader->clear();
			m_connected = false;
			break;
		case QXmlStreamReader::Invalid:
			//handleInvalid();
			break;
		default:
			break;
		}
	}
}
